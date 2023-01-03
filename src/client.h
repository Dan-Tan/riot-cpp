#pragma once

#include <curl/curl.h>
#include <string>
#include <string_view>
#include <jsoncpp/json/json.h>
#include <map>

enum class URL_FORM;

typedef const std::tuple<URL_FORM, std::vector<std::string>> query_format;

typedef struct query_attempts {
    int rate_denials;
    int internal_errors;
    int service_denials;
} query_attempts;

query_attempts* init_attempt_count();
void free_query_counter(query_attempts *counter);

namespace client {

    class RiotApiClient {
        public:
            RiotApiClient(std::string key, std::string path_to_log, bool log_all, bool overwrite);
            ~RiotApiClient();
            
            static const std::unordered_map<int, std::string> Err_Codes;
            static const std::unordered_map<std::string, query_format> query_types;

        private:
            struct curl_slist *header = nullptr;
            CURL* easy_handle = nullptr;

            std::string path_to_log;
            bool log_all;

            int n_attempts;
            int internal_attempts;
            int service_attempts;
            bool internal_wait_type;
            bool service_wait_type;

            Json::Value get(std::string_view end_url, query_attempts *attempt);

            void handle_rate(bool wait_type);
            bool handle_response(std::string_view address, long response_code, query_attempts *attempt);

            void log_request(std::string_view address_sent, long response_code, 
                    query_attempts *attempts); 

            inline std::string encode_url(std::string query_arg) {
                std::string encoding = curl_easy_escape(this->easy_handle, query_arg.data(), query_arg.length());
                return encoding;
            };
    }; 
}
