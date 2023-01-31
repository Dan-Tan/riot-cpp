#pragma once

#include <curl/curl.h>
#include <string>
#include <string_view>
#include <jsoncpp/json/json.h>
#include <unordered_map>
#include <memory>
#include "../query/query.h"
#include "../handling/handlers.h"

namespace client {

    using opt_args = std::pair<std::string, std::string>;

    class RiotApiClient {
        private: 

        public:
            RiotApiClient(std::string path_to_config);
            ~RiotApiClient();

            Json::Value query(std::string endpoint, std::string end_type, std::vector<std::string> params, std::vector<opt_args> optional_args = {});
            
        private:
            bool get(std::shared_ptr<query::query> request);

            inline std::string encode_url(std::string query_arg) {
                std::string encoding = curl_easy_escape(this->easy_handle, query_arg.data(), query_arg.length());
                return encoding;
            };       


            handler::RequestHandler request_handler;

            static const std::unordered_map<int, std::string> Err_Codes;
            static const std::unordered_map<std::string_view, std::unordered_map<std::string_view, std::shared_ptr<query::QueryType>>> query_types;
            CURL* easy_handle = nullptr;
            struct curl_slist *header = nullptr;
    }; 
}
