#include <chrono>
#include <memory>
#include <thread>
#include <ctime>
#include <algorithm>
#include <stdexcept>
#include <functional>
#include <iostream>
#include <cpr/cpr.h>

#include <fstream>
#include <cstdio>
#include <queue>
#include <regex>
#include <cstring>
#include "client.h"

namespace riotcpp::client {

    using func_type = std::function<std::unique_ptr<json_text>(std::shared_ptr<query::query>)>;

    static std::string extract_key(const std::string& path_to_config) {

        std::ifstream config(path_to_config);

        if (!config.is_open()) {
            throw std::runtime_error("Invalid path to file, or file does not exist");
        }

        const std::regex key_reg("RGAPI[-0123456789abcdefABCDEF]{0,38}\"");

        std::stringstream file_cont;
        file_cont << config.rdbuf();
        std::string filestr = file_cont.str();
        std::smatch base_match;

        if (std::regex_search(filestr, base_match, key_reg)) {
            config.close();
            std::string match = base_match[0].str();
            return match.substr(0, match.size()-1);
        }
        config.close();
        throw std::runtime_error("Configuration file does not exist or has unexpected format");
    }

    RiotApiClient::RiotApiClient(const std::string& path_to_config, std::string&& path_to_log, logging::LEVEL report_level, bool verbose_logging) :
        logger(std::move(path_to_log), report_level, verbose_logging),
        request_handler(&(this->logger)),
        endpoint_call(std::bind_front(&RiotApiClient::query, this)),
        Account(&this->endpoint_call),
        Champion_Mastery(&this->endpoint_call),
        Champion(&this->endpoint_call),
        Clash(&this->endpoint_call),
        League_exp(&this->endpoint_call),
        League(&this->endpoint_call),
        Lol_Challenges(&this->endpoint_call),
        Lol_Status(&this->endpoint_call),
        Lor_Match(&this->endpoint_call),
        Lor_Ranked(&this->endpoint_call),
        Lor_Status(&this->endpoint_call),
        Match(&this->endpoint_call),
        Spectator_Tft(&this->endpoint_call),
        Spectator(&this->endpoint_call),
        Summoner(&this->endpoint_call),
        Tft_League(&this->endpoint_call),
        Tft_Match(&this->endpoint_call),
        Tft_Status(&this->endpoint_call),
        Tft_Summoner(&this->endpoint_call),
        Val_Content(&this->endpoint_call),
        Val_Match(&this->endpoint_call),
        Val_Ranked(&this->endpoint_call),
        Val_Status(&this->endpoint_call) {

        std::string api_key = extract_key(path_to_config);
        this->header = cpr::Header{{"X-RIOT-TOKEN", api_key}};
    }

    RiotApiClient::~RiotApiClient() = default;

    bool RiotApiClient::get(const std::shared_ptr<query::query>& request) {

        request->response_content->clear();

        cpr::Response r = cpr::Get(cpr::Url{request->url.get()}, this->header);

        request->response_content->assign(r.text.begin(), r.text.end());
        request->last_response = r.status_code;

        if (r.error) {
            this->logger << logging::LEVEL::CRITICAL << "cpr failed to send request: " << r.error.message << 0;
            request->last_response = -1; // CPR ERRORS
            return false;
        }

        // null terminant buffer for json parsing
        request->response_content->push_back(0);

        // Extract headers
        for (auto const& [key, val] : r.header) {
            if (key == "Date") {
                strncpy(request->response_header.date, val.c_str(), sizeof(request->response_header.date) - 1);
            } else if (key == "X-App-Rate-Limit") {
                strncpy(request->response_header.app_limit, val.c_str(), sizeof(request->response_header.app_limit) - 1);
            } else if (key == "X-App-Rate-Limit-Count") {
                strncpy(request->response_header.app_limit_count, val.c_str(), sizeof(request->response_header.app_limit_count) - 1);
            } else if (key == "X-Method-Rate-Limit") {
                strncpy(request->response_header.method_limit, val.c_str(), sizeof(request->response_header.method_limit) - 1);
            } else if (key == "X-Method-Rate-Limit-Count") {
                strncpy(request->response_header.method_limit_count, val.c_str(), sizeof(request->response_header.method_limit_count) - 1);
            } else if (key == "Retry-After") {
                strncpy(request->response_header.retry_after, val.c_str(), sizeof(request->response_header.retry_after) - 1);
            }
        }


        this->logger << logging::LEVEL::DEBUG << request->response_header << 0;

        if (request->last_response == 200) { // only parse content to json if request was successful
            this->logger << logging::LEVEL::DEBUG << "Query Successful" << 0;
        }

        return true;
    }
    static inline void wait_until(std::time_t send_time) {
        const std::time_t c_time = std::time(nullptr);
        std::time_t current_time = std::mktime(std::gmtime(&c_time));
        if (current_time >= send_time) {
            return;
        }
        std::this_thread::sleep_for(std::chrono::seconds(send_time - current_time));
    }

    std::unique_ptr<json_text> RiotApiClient::query(const std::shared_ptr<query::query>& request) {

        this->logger << logging::LEVEL::DEBUG << "--Query Call--" << std::string(request->url.get()) << 0;

        while (this->request_handler.review_request(request)) {
            if (request->last_response == 200) {
                return std::move(request->response_content);
            }
            if (!this->request_handler.validate_request(request)) {
                this->logger << logging::LEVEL::WARNING << "Request sent was invalid or the server is unavailable" << 0;
                throw std::runtime_error("Request sent was invalid or the server is unavailable");
            }
            this->logger << logging::LEVEL::DEBUG << "Request Validated" << 0;
            wait_until(request->send_time);
            this->get(request);
        }

        this->logger << logging::LEVEL::ERRORS << "Failed request" << request->method_key << request->last_response << 0;
        return std::move(request->response_content);
    }
}

