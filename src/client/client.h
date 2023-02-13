#pragma once

#include <curl/curl.h>
#include <string>
#include <string_view>
#include <jsoncpp/json/json.h>
#include <unordered_map>
#include <memory>
#include "../query/query.h"
#include "../handling/handlers.h"
#include "../logging/logger.h"
#include <functional>

namespace client {

    using opt_args = std::pair<std::string, std::string>;

    class RiotApiClient {
        private: 
;

        public:
            RiotApiClient(std::string path_to_config, std::string path_to_log, logging::LEVEL report_level = logging::LEVEL::INFO);
            ~RiotApiClient();

            query::ACCOUNT_V1 Account;

            
        private:
            Json::Value query(std::shared_ptr<query::query> request);
            bool get(std::shared_ptr<query::query> request);

            inline std::string encode_url(std::string query_arg) {
                char *encoded= curl_easy_escape(this->easy_handle, query_arg.data(), query_arg.length());
                std::string encoding = encoded;
                curl_free(encoded);
                return encoding;
            };       

            std::unique_ptr<handler::RequestHandler> request_handler;
            std::unique_ptr<logging::Logger> logger;

            CURL* easy_handle = nullptr;
            struct curl_slist *header = nullptr;
    }; 
}
