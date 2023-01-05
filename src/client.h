#pragma once

#include <curl/curl.h>
#include <string>
#include <string_view>
#include <jsoncpp/json/json.h>
#include <unordered_map>
#include <memory>
#include <tuple>
#include "query_type.h"

typedef struct query_attempts {
    int rate_denials;
    int internal_errors;
    int service_denials;
} query_attempts;

namespace client {

    class RiotApiClient {
        public:
            RiotApiClient(std::string key, std::string path_to_log, bool log_all, bool overwrite);
            ~RiotApiClient();

            Json::Value query(std::string endpoint, std::string end_type, std::vector<std::string>);
            
        private:
            static const std::unordered_map<int, std::string> Err_Codes;
            static const std::unordered_map<std::string, std::unordered_map<std::string, std::shared_ptr<query::QueryType>>> query_types;
            struct curl_slist *header = nullptr;
            CURL* easy_handle = nullptr;
            std::vector<char> buffer;

            std::string path_to_log;

            int n_attempts;
            int internal_attempts;
            int service_attempts;
            bool internal_wait_type;
            bool service_wait_type;
            bool log_all;

            Json::Value get(std::string_view end_url, std::shared_ptr<query_attempts> attempt);

            void handle_rate(bool wait_type);
            bool handle_response(std::string_view address, long response_code, std::shared_ptr<query_attempts> attempt);

            void log_request(std::string_view address_sent, long response_code, 
                    std::shared_ptr<query_attempts> attempts, CURLcode* res_ = nullptr); 

            inline std::string encode_url(std::string query_arg) {
                std::string encoding = curl_easy_escape(this->easy_handle, query_arg.data(), query_arg.length());
                return encoding;
            };
    }; 
}
