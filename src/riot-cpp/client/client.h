#pragma once

#include <curl/curl.h>
#include <string>
#include <sstream>
#include <string_view>
#include <unordered_map>
#include <memory>
#include "../query/endpoints.h"
#include "../handling/handlers.h"
#include "../logging/logger.h"
#include <functional>
namespace riotcpp {
namespace client {

    using json_text = std::vector<char>;

    using opt_args = std::pair<std::string, std::string>;

    class RiotApiClient {
        private: 
;

        public:
            RiotApiClient(std::string path_to_config, std::string path_to_log, logging::LEVEL report_level = logging::LEVEL::INFO, bool verbose_logging = false);
            ~RiotApiClient();

            query::Account_v1 Account;

        protected:

            rate::RequestHandler request_handler;
            logging::Logger logger;

        private:
            std::unique_ptr<json_text> query(std::shared_ptr<query::query> request);
            std::function<std::unique_ptr<json_text>(std::shared_ptr<query::query>)> endpoint_call;
            bool get(std::shared_ptr<query::query> request);


            CURL* easy_handle = nullptr;
            struct curl_slist *header = nullptr;
    }; 
}
}
