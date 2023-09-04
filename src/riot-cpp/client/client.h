#pragma once

#include <curl/curl.h>
#include <string>
#include <sstream>
#include <string_view>
#include <unordered_map>
#include <memory>
#include "../query/query.h"
#include "../handling/handlers.h"
#include "../logging/logger.h"
#include <functional>

namespace client {

    using json_text = std::vector<char>;

    using opt_args = std::pair<std::string, std::string>;

    class RiotApiClient {
        private: 
;

        public:
            RiotApiClient(std::string path_to_config, std::string path_to_log, logging::LEVEL report_level = logging::LEVEL::INFO, bool verbose_logging = false);
            ~RiotApiClient();

            query::ACCOUNT_V1 Account;
            query::CHAMPION_MASTERY_V4 Champion_Mastery;
            query::CHAMPION_V3 Champion;
            query::CLASH_V1 Clash;
            query::LEAGUE_EXP_V4 League_Exp;
            query::LEAGUE_V4 League;
            query::LOL_CHALLENGES_V1 Lol_Challenges; 
            query::LOL_STATUS Lol_Status;
            query::LOR_MATCH_V1 Lor_Match;
            query::LOR_RANKED_V1 Lor_Ranked;
            query::LOR_STATUS_V1 Lor_Status;
            query::MATCH_V5 Match;
            query::SUMMONER_V4 Summoner;
            query::SPECTATOR_V4 Spectator;
            query::TFT_LEAGUE_V1 Tft_League;
            query::TFT_MATCH_V1 Tft_Match;
            query::TFT_STATUS_V1 Tft_Status;
            query::TFT_SUMMONER_V1 Tft_Summoner;
            query::VAL_CONTENT_V1 Val_Content;
            query::VAL_MATCH_V1 Val_Match;
            query::VAL_RANKED_V1 Val_Ranked;
            query::VAL_STATUS_V1 Val_Status;

        protected:

            handler::RequestHandler request_handler;
            logging::Logger logger;

        private:
            std::unique_ptr<json_text> query(std::shared_ptr<query::query> request);
            std::function<std::unique_ptr<json_text>(std::shared_ptr<query::query>)> endpoint_call;
            bool get(std::shared_ptr<query::query> request);


            CURL* easy_handle = nullptr;
            struct curl_slist *header = nullptr;
    }; 
}
