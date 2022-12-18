#pragma once

#include <curl/curl.h>
#include <string>

using namespace client {
    class RiotApiClient {
        public:
            RiotApiClient(std::string key);
            ~RiotApiCLient();
            
            std::string get(std::string end_url, std::string region);
            std::string get_BASE_URL(std::string region);
        private:

            struct curl_slist *header = NULL;
            std::string BASE_URL_START = "http://";
            std::string BASE_URL_END = ".api.riotgames.com";
            CURL* easy_handle;

    }
}
