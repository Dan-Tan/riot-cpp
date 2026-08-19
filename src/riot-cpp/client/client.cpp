#include <chrono>
#include <memory>
#include <thread>
#include <ctime>
#include <stdexcept>
#include <functional>
#include <iostream>
#include <cpr/cpr.h>

#include <fstream>
#include <cstdio>
#include <regex>
#include <cstring>
#include "client.h"
#include "../logging/logger.h"

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

    RiotApiClient::RiotApiClient(const std::string& path_to_config, std::string&& path_to_log, spdlog::level::level_enum report_level, bool verbose_logging) :
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

        riotcpp::logging::initialize(path_to_log, report_level);
        std::string api_key = extract_key(path_to_config);
        this->header = cpr::Header{{"X-RIOT-TOKEN", api_key}};
    }

    RiotApiClient::RiotApiClient(std::string api_key, std::string path_to_log, spdlog::level::level_enum report_level, bool verbose_logging, bool is_direct_key) :
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

        riotcpp::logging::initialize(path_to_log, report_level);
        this->header = cpr::Header{{"X-RIOT-TOKEN", api_key}};
    }

    void RiotApiClient::update_api_key(const std::string& new_key) {
        this->header = cpr::Header{{"X-RIOT-TOKEN", new_key}};
    }

    RiotApiClient::~RiotApiClient() = default;

    bool RiotApiClient::get(const std::shared_ptr<query::query>& request) {

        request->response_content->clear();

        riotcpp::logging::log_headers(request->query_id, "Request Headers", this->header);
        cpr::Response resp = cpr::Get(cpr::Url(request->url), this->header);
        riotcpp::logging::log_headers(request->query_id, "Response Headers", resp.header);

        request->response_content->assign(resp.text.begin(), resp.text.end());
        request->last_response = resp.status_code;

        if (resp.error) {
            riotcpp::logging::get()->critical("[Query {}] cpr failed to send request: {}", request->query_id, resp.error.message);
            request->last_response = -1; // CPR ERRORS
            return false;
        }

        // null terminant buffer for json parsing
        request->response_content->push_back(0);

        // Extract headers
        for (auto const& [key, val] : resp.header) {
            if (key == "Date") {
                request->response_header.date = val;
            } else if (key == "X-App-Rate-Limit") {
                request->response_header.app_limit = val;
            } else if (key == "X-App-Rate-Limit-Count") {
                request->response_header.app_limit_count = val;
            } else if (key == "X-Method-Rate-Limit") {
                request->response_header.method_limit = val;
            } else if (key == "X-Method-Rate-Limit-Count") {
                request->response_header.method_limit_count = val;
            } else if (key == "Retry-After") {
                request->response_header.retry_after = val;
            }
        }

        if (request->last_response == 200) { // only parse content to json if request was successful
            riotcpp::logging::get()->info("[Query {}] Request for method '{}' successful (200 OK)", request->query_id, request->method_key);
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

        riotcpp::logging::get()->info("[Query {}] Preparing request for method '{}' to '{}'", request->query_id, request->method_key, request->url);

        // Special handling for the very first request to initialize the rate limiter
        if (!this->request_handler.is_initialized()) {
            logging::get()->debug("[Query {}] Rate limiter not initialized. Sending initial request.", request->query_id);
            this->get(request); // Send the request
            if (request->last_response != -1) { // -1 is a CPR error
                this->request_handler.validate_request(request); // This will initialize the handler
            }
            if (request->last_response == 200) {
                return std::move(request->response_content);
            }
            // If the first request fails, it will fall through to the main loop's error handling.
        }

        // Main request loop for all subsequent (or failed first) requests
        while (true) {
            // 1. CHECK & WAIT
            this->request_handler.check_rate_limits(request);
            wait_until(request->send_time);

            // 2. SEND
            this->get(request);

            // 3. UPDATE rate limit state from response
            if (request->last_response != -1) {
                this->request_handler.validate_request(request);
            }
            
            // 4. REVIEW response and decide action
            if (request->last_response == 200) {
                return std::move(request->response_content); // Success
            }

            // If not 200, check if it's a retryable server error
            if (this->request_handler.review_request(request)) {
                riotcpp::logging::get()->warn("[Query {}] Server returned a retryable error (Code: {}). Retrying...", request->query_id, request->last_response);
                // Loop will continue and retry the request
            } else {
                // Not a retryable error, so we fail permanently.
                riotcpp::logging::get()->error("[Query {}] Unrecoverable error for method '{}'. Response Code: {}", request->query_id, request->method_key, request->last_response);
                return std::move(request->response_content);
            }
        }
    }
} // namespace riotcpp::client
