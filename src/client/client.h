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
            RiotApiClient(std::string path_to_config, std::string path_to_log, logging::LEVEL report_level = logging::LEVEL::INFO, bool verbose_logging = false);
            ~RiotApiClient();

            const query::ACCOUNT_V1 Account;
            const query::CHAMPION_MASTERY_V4 Champion_Mastery;
            const query::CHAMPION_V3 Champion;
            const query::CLASH_V1 Clash;
            const query::LEAGUE_EXP_V4 League_Exp;
            const query::LEAGUE_V4 League;
            const query::LOL_CHALLENGES_V1 Lol_Challenges; 
            const query::LOL_STATUS Lol_Status;
            const query::LOR_MATCH_V1 Lor_Match;
            const query::LOR_RANKED_V1 Lor_Ranked;
            const query::LOR_STATUS_V1 Lor_Status;
            const query::MATCH_V5 Match;
            const query::SUMMONER_V4 Summoner;
            const query::SPECTATOR_V4 Spectator;
            const query::TFT_LEAGUE_V1 Tft_League;
            const query::TFT_MATCH_V1 Tft_Match;
            const query::TFT_STATUS_V1 Tft_Status;
            const query::TFT_SUMMONER_V1 Tft_Summoner;
            const query::VAL_CONTENT_V1 Val_Content;
            const query::VAL_MATCH_V1 Val_Match;
            const query::VAL_RANKED_V1 Val_Ranked;
            const query::VAL_STATUS_V1 Val_Status;

        private:
            Json::Value query(std::shared_ptr<query::query> request);
            std::function<Json::Value(std::shared_ptr<query::query>)> endpoint_call;
            bool get(std::shared_ptr<query::query> request);

            inline std::string encode_url(std::string query_arg) {
                char *encoded= curl_easy_escape(this->easy_handle, query_arg.data(), query_arg.length());
                std::string encoding = encoded;
                curl_free(encoded);
                return encoding;
            };       

            handler::RequestHandler request_handler;
            logging::Logger logger;

            CURL* easy_handle = nullptr;
            struct curl_slist *header = nullptr;
    }; 
}
