#include <chrono>
#include <memory>
#include <thread>
#include <ctime>
#include <algorithm>
#include <stdexcept>
#include <functional>

#include <fstream>
#include <stdio.h>
#include <queue>
#include "client.h"

namespace client {

    using func_type = std::function<Json::Value(std::shared_ptr<query::query>)>;

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
        Json::Reader reader;

        std::ifstream config(path_to_config);

        if (!config.is_open()) {
            std::string err_msg("Failed to open config file");
            err_msg = err_msg + path_to_config;
            throw std::invalid_argument(err_msg);
        }
        
        Json::Value root;
        bool retrieving_key = reader.parse(config, root);

        if (!retrieving_key) {
            throw Json::Exception("Unable to parse config file to Json format");
        }
        else {
            std::string api_key = root["api-key"].asString();
            this->header = curl_slist_append(header, (std::string("X-RIOT-TOKEN: ") + api_key).c_str());
        }
        
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
        if (*new_chars == 'H') {
            return real_size;
        }
        std::vector<char> *new_buffer = static_cast<std::vector<char>*>(buffer);
        new_buffer->insert(new_buffer->end(), &new_chars[0], &new_chars[nmemb]);

        return real_size;
    }

    static size_t WriteCallBack_header(char* buffer, size_t size, size_t nitems, void* user_data) { 
        std::size_t real_size = nitems * size;

        if (*buffer == 'H') {
            return real_size;
        }
        
        Json::Value* new_header = static_cast<Json::Value *>(user_data);
        
        char* colon = std::find(buffer, buffer + nitems, ':');
        if (colon == buffer + nitems) {
            return real_size;
        }
        std::string key(buffer, colon);
        // response has white space after ':' and two excess characters \r\n 
        std::string cont(colon+2, buffer + nitems-2);
        (*new_header)[key] = cont;
        
        return real_size;
    }

    bool RiotApiClient::get(std::shared_ptr<query::query> request) {

        Json::Reader reader;
        std::vector<char> content_buffer = {};

        curl_easy_setopt(this->easy_handle, CURLOPT_URL, request->url.data());
        curl_easy_setopt(this->easy_handle, CURLOPT_HTTPGET, 1);
        curl_easy_setopt(this->easy_handle, CURLOPT_HTTPHEADER, this->header);

        curl_easy_setopt(this->easy_handle, CURLOPT_WRITEFUNCTION, WriteCallBack);
        curl_easy_setopt(this->easy_handle, CURLOPT_WRITEDATA, &content_buffer);

        curl_easy_setopt(this->easy_handle, CURLOPT_HEADERFUNCTION, WriteCallBack_header);
        curl_easy_setopt(this->easy_handle, CURLOPT_HEADERDATA, &request->response_header);
     
        CURLcode res_ = curl_easy_perform(this->easy_handle);

        if (res_ != CURLE_OK) {
            this->logger << logging::LEVEL::CRITICAL << "CURL failed to send request" << 0;
            request->last_response = -1; // CURL ERRORS
            return false;
        }
        // null terminant buffer for json parsing
        content_buffer.push_back(0);

        curl_easy_getinfo(this->easy_handle, CURLINFO_RESPONSE_CODE, &(request->last_response));

        if (!reader.parse(content_buffer.data(), request->response_content)) {
            this->logger<< logging::LEVEL::ERRORS <<  "Failed to parse json content string";
        }
        
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

    Json::Value RiotApiClient::query(std::shared_ptr<query::query> request) {

        this->logger << logging::LEVEL::DEBUG << "--Query Call--" << request->url << 0;

        while (this->request_handler.review_request(request)) {
            if (request->last_response == 200) {
                return request->response_content;
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
        return request->response_content;
    }
}
