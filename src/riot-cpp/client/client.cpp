#include <chrono>
#include <memory>
#include <thread>
#include <ctime>
#include <algorithm>
#include <stdexcept>
#include <functional>
#include <iostream>

#include <fstream>
#include <stdio.h>
#include <queue>
#include <regex>
#include <cstring>
#include "client.h"

namespace client {

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
        } else {
            config.close();
            throw std::runtime_error("Configuration file does not exist or has unexpected format");
        };
    }

    RiotApiClient::RiotApiClient(std::string path_to_config, std::string path_to_log, logging::LEVEL report_level, bool verbose_logging) : 
        logger(path_to_log, report_level, verbose_logging),
        request_handler(&(this->logger)),
        endpoint_call(std::bind_front(&RiotApiClient::query, this)),
        Account(&this->endpoint_call), 
        Champion_Mastery(&this->endpoint_call), 
        Champion(&this->endpoint_call), 
        Clash(&this->endpoint_call), 
        League_Exp(&this->endpoint_call),
        League(&this->endpoint_call),
        Lol_Challenges(&this->endpoint_call),
        Lol_Status(&this->endpoint_call),
        Lor_Match(&this->endpoint_call),
        Lor_Ranked(&this->endpoint_call),
        Lor_Status(&this->endpoint_call),
        Match(&this->endpoint_call),
        Summoner(&this->endpoint_call),
        Spectator(&this->endpoint_call),
        Tft_League(&this->endpoint_call),
        Tft_Match(&this->endpoint_call),
        Tft_Status(&this->endpoint_call),
        Tft_Summoner(&this->endpoint_call),
        Val_Content(&this->endpoint_call),
        Val_Match(&this->endpoint_call),
        Val_Ranked(&this->endpoint_call),
        Val_Status(&this->endpoint_call){
        curl_global_init(CURL_GLOBAL_ALL);

        // initialised libcurl handle and header
        std::string api_key = extract_key(path_to_config);
        this->header = curl_slist_append(header, (std::string("X-RIOT-TOKEN: ") + api_key).c_str());
        
        this->easy_handle = curl_easy_init();
    }

    RiotApiClient::~RiotApiClient() {
        curl_slist_free_all(this->header);
        curl_easy_cleanup(this->easy_handle);
        curl_global_cleanup();
    }

    static size_t WriteCallBack(void* contents, size_t size, size_t nmemb, void* buffer) {
        
        size_t real_size = size * nmemb;
        char *new_chars = (char *)contents;
        
        std::vector<char> *new_buffer = static_cast<std::vector<char>*>(buffer);
        new_buffer->insert(new_buffer->end(), &new_chars[0], &new_chars[nmemb]);

        return real_size;
    }

    static size_t WriteCallBack_header(char* buffer, size_t size, size_t nitems, void* user_data) { 
        std::size_t real_size = nitems * size;

        if (*buffer == 'H') {
            return real_size;
        }

        query::RiotHeader* new_header = static_cast<query::RiotHeader*>(user_data);
        char* write_field;

        switch (buffer[0]) {
            case 'D':
                write_field = &new_header->date[0]; break;
            case 'X':
                switch (buffer[2]) {
                    case 'A':
                        write_field = buffer[17] == 'C' ? &new_header->app_limit_count[0] : &new_header->app_limit[0]; break;
                    case 'M':
                        write_field = buffer[20] == 'C' ? &new_header->method_limit_count[0] : &new_header->method_limit[0]; break;
                    default:
                        return real_size;
                } break;
            case 'R':
                write_field = &new_header->retry_after[0]; break;
            default: // can't be bother parsing headers we dont need/use
                return real_size;
        }

        char* colon = std::find(buffer, buffer + nitems, ':');
        if (colon == buffer + nitems) {
            return real_size;
        }

        strncpy(write_field, colon+2, nitems - (std::size_t)(colon - buffer) - 4);
        write_field[nitems - (std::size_t)(colon - buffer) - 4] = 0;
        
        return real_size;
    }

    bool RiotApiClient::get(std::shared_ptr<query::query> request) {

        request->response_content->clear();

        curl_easy_setopt(this->easy_handle, CURLOPT_URL, request->url.data());
        curl_easy_setopt(this->easy_handle, CURLOPT_HTTPGET, 1);
        curl_easy_setopt(this->easy_handle, CURLOPT_HTTPHEADER, this->header);

        curl_easy_setopt(this->easy_handle, CURLOPT_WRITEFUNCTION, WriteCallBack);
        curl_easy_setopt(this->easy_handle, CURLOPT_WRITEDATA, &(*request->response_content));

        curl_easy_setopt(this->easy_handle, CURLOPT_HEADERFUNCTION, WriteCallBack_header);
        curl_easy_setopt(this->easy_handle, CURLOPT_HEADERDATA, &request->response_header);
     
        CURLcode res_ = curl_easy_perform(this->easy_handle);

        if (res_ != CURLE_OK) {
            this->logger << logging::LEVEL::CRITICAL << "CURL failed to send request" << 0;
            request->last_response = -1; // CURL ERRORS
            return false;
        }
        // null terminant buffer for json parsing
        request->response_content->push_back(0);

        this->logger << logging::LEVEL::DEBUG << request->response_header << 0;

        curl_easy_getinfo(this->easy_handle, CURLINFO_RESPONSE_CODE, &(request->last_response));
        
        if (request->last_response == 200) { // only parse content to json if request was successful
            this->logger << logging::LEVEL::DEBUG << "Query Successful" << 0;
        }

        return true;
    }
    static inline void wait_until(std::time_t send_time) {
        const std::time_t c_time = std::time(NULL);
        std::time_t current_time = std::mktime(std::gmtime(&c_time));
        if (current_time >= send_time) {
            return;
        } else {
            std::this_thread::sleep_for(std::chrono::seconds(send_time - current_time));
            return;
        }
    }

    std::unique_ptr<json_text> RiotApiClient::query(std::shared_ptr<query::query> request) {

        this->logger << logging::LEVEL::DEBUG << "--Query Call--" << request->url << 0;

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
