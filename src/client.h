#pragma once

#include <curl/curl.h>
#include <string>
#include <jsoncpp/json/json.h>

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
            RiotApiClient(std::string key, std::string path_to_log);
            ~RiotApiClient();
            
            std::string get_BASE_URL(std::string region);

            // ACCOUNT_V1
            Json::Value ACCOUNT_V1_puuid(std::string puuid, std::string region);
            Json::Value ACCOUNT_V1_riotid(std::string gamename, std::string tagline, std::string region);
            Json::Value ACCOUNT_V1_game(std::string game, std::string puuid, std::string region);

            // LEAGUE_V4
            Json::Value LEAGUE_V4_challenger(std::string queue, std::string region);
            Json::Value LEAGUE_V4_grandmaster(std::string queue, std::string region);
            Json::Value LEAGUE_V4_master(std::string queue, std::string region);
            Json::Value LEAGUE_V4_queue(std::string queue, std::string tier, std::string division, std::string region);
            Json::Value LEAGUE_V4_summonerid(std::string summoner_id, std::string region);
            Json::Value LEAGUE_V4_leagueid(std::string league_id, std::string region);

            // SUMMONER_V4
            Json::Value SUMMONER_V4_rso_puuid(std::string rso_puuid, std::string region);
            Json::Value SUMMONER_V4_accountid(std::string accound_id, std::string region);
            Json::Value SUMMONER_V4_summoner_name(std::string summoner_name, std::string region);
            Json::Value SUMMONER_V4_puuid(std::string puuid, std::string region);
            Json::Value SUMMONER_V4_summonerid(std::string summoner_id, std::string region);

            // MATCH_V5
            Json::Value MATCH_V5_puuid(std::string puuid, std::string region);
            Json::Value MATCH_V5_matchid(std::string match_id, std::string region);
            Json::Value MATCH_V5_timeline(std::string match_id, std::string region);
            
            // CLASH_V1
            Json::Value CLASH_V1_summonerid(std::string summoner_id, std::string region);
            Json::Value CLASH_V1_teamid(std::string team_id, std::string region);
            Json::Value CLASH_V1_tournament_byteam(std::string team_id, std::string region);
            Json::Value CLASH_V1_tournamentid(std::string tournament_id, std::string region);

        private:
            struct curl_slist *header = NULL;
            std::string BASE_URL_START = "https://";
            std::string BASE_URL_END = ".api.riotgames.com";
            CURL* easy_handle;
            std::string path_to_log;
            int n_attempts;
            int internal_attempts;
            int service_attempts;
            bool internal_wait_type;
            bool service_wait_type;

            Json::Value get(std::string end_url, std::string region, query_attempts *attempt);
            void handle_rate(bool wait_type);
            bool handle_response(std::string address, long response_code, query_attempts *attempt);
            void log_request(std::string address_sent, long response_code, 
                    query_attempts *attempts, bool start);
    };
}
