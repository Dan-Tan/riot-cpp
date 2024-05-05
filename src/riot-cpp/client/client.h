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
        public:
            RiotApiClient(std::string path_to_config, std::string path_to_log, logging::LEVEL report_level = logging::LEVEL::INFO, bool verbose_logging = false);
            ~RiotApiClient();
            
            const query::Account_v1 Account;
            const query::Champion_Mastery_v4 Champion_Mastery;
            const query::Champion_v3 Champion;
            const query::Clash_v1 Clash;
            const query::League_exp_v4 League_exp;
            const query::League_v4 League;
            const query::Lol_Challenges_v1 Lol_Challenges;
            const query::Lol_Status_v4 Lol_Status;
            const query::Lor_Match_v1 Lor_Match;
            const query::Lor_Ranked_v1 Lor_Ranked;
            const query::Lor_Status_v1 Lor_Status;
            const query::Match_v5 Match;
            const query::Spectator_Tft_v5 Spectator_Tft;
            const query::Spectator_v5 Spectator;
            const query::Summoner_v4 Summoner;
            const query::Tft_League_v1 Tft_League;
            const query::Tft_Match_v1 Tft_Match;
            const query::Tft_Status_v1 Tft_Status;
            const query::Tft_Summoner_v1 Tft_Summoner;
            const query::Val_Content_v1 Val_Content;
            const query::Val_Match_v1 Val_Match;
            const query::Val_Ranked_v1 Val_Ranked;
            const query::Val_Status_v1 Val_Status;


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
