#pragma once

#include <curl/curl.h>
#include <string>

namespace client {
    class RiotApiClient {
        public:
            RiotApiClient(std::string key, std::string path_to_log);
            ~RiotApiClient();
            
            Json::Value league_LEAGUE_V4(std::string queue, std::string league, std::string region);
            Json::Value player_LEAGUE_V4(std::string summoner_id, std::string region);
            Json::Value MATCH_V5(std::string match_id, int type, std::string region);
            Json::Value SUMMONER_V4(std::string ending, std::string region);

            std::string get_BASE_URL(std::string region);
        private:
            struct curl_slist *header = NULL;
            std::string BASE_URL_START = "https://";
            std::string BASE_URL_END = ".api.riotgames.com";
            CURL* easy_handle;
            std::string path_to_log;
            int n_attempts;

            Json::Value get(std::string end_url, std::string region, int attempy);
            void handle_rate();
    };
}
