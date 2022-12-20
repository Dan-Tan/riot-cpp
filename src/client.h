#pragma once

#include <curl/curl.h>
#include <string>

namespace client {
    class RiotApiClient {
        public:
            RiotApiClient(std::string key);
            ~RiotApiClient();
            
            std::string get(std::string end_url, std::string region);
            std::string get_BASE_URL(std::string region);
            std::string league_LEAGUE_V4(std::string queue, std::string league, std::string region);
            std::string player_LEAGUE_V4(std::string summoner_id, std::string region);
            std::string MATCH_V5(std::string match_id, int type, std::string region);
            std::string SUMMONER_V4(std::string ending, std::string region);

        private:
            struct curl_slist *header = NULL;
            std::string BASE_URL_START = "https://";
            std::string BASE_URL_END = ".api.riotgames.com";
            CURL* easy_handle;

    };
}
