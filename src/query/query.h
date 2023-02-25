#pragma once
#include <concepts>
#include <jsoncpp/json/json.h>
#include <ctime>
#include <string>
#include <vector>
#include <functional>
#include <curl/curl.h>

namespace query {

    typedef struct query {
        std::string method_key;
        std::string routing_value;
        std::string url;
        std::time_t send_time = 0;
        Json::Value response_content;
        Json::Value response_header;
        long last_response = 0;
        int server_errors = 0;
    } query;

    template <typename T>
    concept param = requires(std::ostream& os, T a){os << a;};
    using query_fp = std::function<Json::Value(std::shared_ptr<query>)>*;
    using const_str_r = const std::string&;
    
    typedef struct {} OPTS;

    typedef struct Endpoint {
        
        public:
            const query_fp _query;
            const std::string _url;
        
        private: 
            template <param R>
            std::stringstream construct_base(const R& routing) const; 

            template <std::size_t N, param Routing, param ... Params>
            std::string full_query(const std::array<std::string, N>& method_urls, const Routing& routing, const Params& ... params) const;
        
            template <std::size_t N, param Routing, param ... Params, param ... Opts>
            std::string full_query(const std::array<std::string, N>& method_urls, const Routing& routing, const Params& ... params, OPTS, const Opts& ... optional_args) const;
            
        public:
            Endpoint(const std::string& url, query_fp client_query) : _query(client_query), _url(url) {};
            
            template <param R>
            std::shared_ptr<query> request(const_str_r key, const_str_r method_url, const R& routing);

            template <std::size_t N, param Routing, param ... Params>
            std::shared_ptr<query> request(const_str_r key, const std::array<std::string, N>& method_urls, const Routing& routing, const Params& ... params);

            template <std::size_t N, param Routing, param ... Params, param ... Opts>
            std::shared_ptr<query> request(const_str_r key, const std::array<std::string, N>& method_urls, const Routing& routing, const Params& ... params, OPTS, const Opts& ... optional_args);

    } Endpoint;

    typedef struct ACCOUNT_V1 : public Endpoint {
        ACCOUNT_V1(query_fp client_query) : Endpoint("riot/account/v1/", client_query) {};
        Json::Value by_puuid(const_str_r routing, const_str_r puuid);
        Json::Value by_riot_id(const_str_r routing, const_str_r gameName, const_str_r tagline);
        Json::Value by_game(const_str_r routing, const_str_r game, const_str_r puuid);

    } ACCOUNT_V1;

    typedef struct CHAMPION_MASTERY_V4 : public Endpoint {
        CHAMPION_MASTERY_V4(query_fp client_query) : Endpoint("lol/champion-mastery/v4/", client_query) {};
        Json::Value by_summoner_id(const_str_r routing, const_str_r summoner_id);
        Json::Value by_summoner_by_champion(const_str_r routing, const_str_r summoner_id, const_str_r champion_id);
        template<param ... Opts>
        Json::Value by_summoner_top(const_str_r routing, const_str_r summoner_id, const Opts& ... optional_args) {
                const std::string method_key("CHAMPION-MASTERY-V4-by-summoner-top");
                const std::array<std::string, 2> method_urls{"champion-masteries/by-summoner/", "/top"};
                std::shared_ptr<query> new_request = this->request(method_key, method_urls, routing, summoner_id, OPTS(), optional_args...);
                return (*this->_query)(new_request);

        }; 
        Json::Value scores_by_summoner(const_str_r routing, const_str_r summoner_id);
        
    } CHAMPION_MASTERY_V4;

    typedef struct CHAMPION_V3 : public Endpoint {
        CHAMPION_V3(query_fp client_query) : Endpoint("lol/platform/v3/", client_query) {};
        Json::Value rotations(const_str_r routing);
    } CHAMPION_V3;

    typedef struct CLASH_V1: public Endpoint {
        CLASH_V1(query_fp client_query) : Endpoint("lol/clash/v1/", client_query) {};
        Json::Value by_summoner_id(const_str_r routing, const_str_r summoner_id);
        Json::Value by_team(const_str_r routing, const_str_r team);
        Json::Value tournament_by_team(const_str_r routing, const_str_r team);
        Json::Value by_tournament(const_str_r routing, const_str_r tournament);
    } CLASH_V1;
    
    typedef struct LEAGUE_EXP_V4 : public Endpoint {
        LEAGUE_EXP_V4(query_fp client_query) : Endpoint("lol/league-exp/v4/", client_query) {};

        template <param ... Opts>
        Json::Value entries(const_str_r routing, const_str_r queue, const_str_r tier, const_str_r division, const Opts& ... optional_args) {
            const std::string method_key("LEAGUE-EXP-V4-entries");
            const std::array<std::string, 3> method_urls{"entries/", "/", "/"};
            std::shared_ptr<query> new_request = this->request(method_key, method_urls, routing, queue, tier, division, OPTS(), optional_args...);
            return (*this->_query)(new_request);
        };
    } LEAGUE_EXP_V4;

    typedef struct LEAGUE_V4 : public Endpoint {
        LEAGUE_V4(query_fp client_query) : Endpoint("lol/league/v4", client_query) {};
        Json::Value challenger(const_str_r routing, const_str_r queue);
        Json::Value grandmaster(const_str_r routing, const_str_r queue);
        Json::Value master(const_str_r routing, const_str_r queue);
        Json::Value by_summoner_id(const_str_r routing, const_str_r summoner_id);
        Json::Value by_league_id(const_str_r routing, const_str_r league_id);

        template <param ... Opts>
        Json::Value specific_league(const_str_r routing, const_str_r queue, const_str_r tier, const_str_r division, const Opts& ... optional_args) {
            const std::string method_key("LEAGUE-V4-specific-league");
            const std::array<std::string, 3> method_urls{"entries/", "/", "/"};
            std::shared_ptr<query> new_request = this->request(method_key, method_urls, routing, queue, tier, division, OPTS(), optional_args ...);
            return (*this->_query)(new_request);
        }
    } LEAGUE_V4;

    typedef struct LOL_CHALLENGES_V1 : public Endpoint {
        LOL_CHALLENGES_V1(query_fp client_query) : Endpoint("lol/challnges/v1/", client_query) {};
        Json::Value config(const_str_r routing);
        Json::Value percentiles(const_str_r routing);
        Json::Value challenge_config(const_str_r routing, const_str_r challenge_id);

        template <param ... Opts>
        Json::Value challenge_leaderboard(const_str_r routing, const_str_r challenge_id, const_str_r level, const Opts& ... optional_args) {
            const std::string method_key("LOL-CHALLENGES-V1-challenge-leaderboard");
            const std::array<std::string, 2> method_urls{"challenges/", "/leaderboards/by-level/"};
            std::shared_ptr<query> new_request = this->request(method_key, method_urls, routing, challenge_id, level, OPTS(), optional_args ...);
            return (*this->_query)(new_request);
        };
        Json::Value challenge_percentiles(const_str_r routing, const_str_r challenge_id);
        Json::Value by_puuid(const_str_r routing, const_str_r puuid);
    } LOL_CHALLENGES_V1;

    typedef struct LOL_STATUS : public Endpoint {
        LOL_STATUS(query_fp client_query) : Endpoint("lol/status/", client_query) {};
        Json::Value v3(const_str_r routing);
        Json::Value v4(const_str_r routing);
    } LOL_STATUS;

    typedef struct LOR_MATCH_V1 : public Endpoint {
        LOR_MATCH_V1(query_fp client_query) : Endpoint("lor/match/v1/", client_query) {};
        Json::Value by_puuid(const_str_r routing, const_str_r puuid);
        Json::Value by_match(const_str_r routing, const_str_r match);
    } LOR_MATCH_V1;

    typedef struct LOR_RANKED_V1 : public Endpoint {
        LOR_RANKED_V1(query_fp client_query) : Endpoint("lor/ranked/v1/", client_query) {};
        Json::Value leaderboards(const_str_r routing);
    } LOR_RANKED_V1;

    typedef struct LOR_STATUS_V1 : public Endpoint {
        LOR_STATUS_V1(query_fp client_query) : Endpoint("lor/status/v1/", client_query) {};
        Json::Value v1(const_str_r routing);
    } LOR_STATUS_V1;

    typedef struct MATCH_V5 : public Endpoint {
        MATCH_V5(query_fp client_query) : Endpoint("lol/match/v5/", client_query) {};

        Json::Value by_match_id(const_str_r routing, const_str_r match_id);
        Json::Value timeline(const_str_r routing, const_str_r match_id);

        template <param ... Opts>
        Json::Value by_puuid(const_str_r routing, const_str_r puuid, const Opts& ... optional_args) {
            const std::string method_key = "MATCH-V5-by-puuid";
            const std::array<std::string, 2> method_urls= {"matches/by-puuid/", "/ids"};
            std::shared_ptr<query> new_request = this->request(method_key, method_urls, routing, puuid, OPTS(), optional_args ...);
            return (*this->_query)(new_request);
        };
    } MATCH_V5;

    typedef struct SUMMONER_V4 : public Endpoint {
        SUMMONER_V4(query_fp client_query) : Endpoint("lol/summoner/v4/", client_query) {};
        Json::Value by_rso_puuid(const_str_r routing, const_str_r puuid);
        Json::Value by_account_id(const_str_r routing, const_str_r account_id);
        Json::Value by_name(const_str_r routing, const_str_r name);
        Json::Value by_puuid(const_str_r routing, const_str_r puuid);
        Json::Value by_summoner_id(const_str_r routing, const_str_r summoner_id);
    } SUMMONER_V4;

    typedef struct SPECTATOR_V4 : public Endpoint {
        SPECTATOR_V4(query_fp client_query) : Endpoint("lol/spectator/v4/", client_query) {};
        Json::Value by_summoner_id(const_str_r routing, const_str_r summonerId);
        Json::Value featured_games(const_str_r routing);
    } SPECTATOR_V4;

    typedef struct TFT_LEAGUE_V1 : public Endpoint {
        TFT_LEAGUE_V1(query_fp client_query) : Endpoint("tft/league/v1/", client_query) {};
        Json::Value challenger(const_str_r routing);
        Json::Value grandmaster(const_str_r routing);
        Json::Value master(const_str_r routing);
        Json::Value by_summoner_id(const_str_r routing, const_str_r summoner_id);
        Json::Value by_league_id(const_str_r routing, const_str_r league_id);
        Json::Value queue_top(const_str_r routing, const_str_r queue);
        Json::Value by_tier_division(const_str_r routing, const_str_r tier, const_str_r division);
    } TFT_LEAGUE_V1;

    typedef struct TFT_MATCH_V1 : public Endpoint {
        TFT_MATCH_V1(query_fp client_query) : Endpoint("tft/match/v1/", client_query) {};
        template <param ... Opts>
        Json::Value by_puuid(const_str_r routing, const_str_r puuid, const Opts& ... optional_args) { //template implementation must be visisble
            const std::string method_key = "TFT-MATCH-V1-by-puuid";
            const std::array<std::string, 2> method_urls = {"matches/by-puuid/", "/ids"};
            std::shared_ptr<query> new_request = this->request(method_key, method_urls, routing, puuid, OPTS(), optional_args...);
            return (*this->_query)(new_request);
        };
        Json::Value by_match(const_str_r routing, const_str_r match_id);
    } TFT_MATCH_V1;

    typedef struct TFT_STATUS_V1 : public Endpoint {
        TFT_STATUS_V1(query_fp client_query) : Endpoint("tft/status/v1/", client_query) {};
        Json::Value v1(const_str_r routing);
    } TFT_STATUS_V1;

    typedef struct TFT_SUMMONER_V1 : public Endpoint {
        TFT_SUMMONER_V1(query_fp client_query) : Endpoint("tft/summoner/v1/", client_query) {};
        Json::Value by_account(const_str_r routing, const_str_r account_id);
        Json::Value by_name(const_str_r routing, const_str_r summoner_name);
        Json::Value by_puuid(const_str_r routing, const_str_r puuid);
        Json::Value by_summoner_id(const_str_r routing, const_str_r summoner_id);
    } TFT_SUMMONER_V1;

    typedef struct VAL_CONTENT_V1 : public Endpoint {
        VAL_CONTENT_V1(query_fp client_query) : Endpoint("val/content/v1/", client_query) {};
        Json::Value content(const_str_r routing);

        template <param ... Opts>
        Json::Value content(const_str_r routing, const Opts& ... optional_arg) {
            const std::string method_key = "VAL-CONTENT-V1-content";
            const std::array<std::string, 1> method_urls = { "contents" };
            std::shared_ptr<query> new_request = this->request(method_key, method_urls, routing, OPTS(), optional_arg ...);
            return (*this->_query)(new_request);
        };
    } VAL_CONTENT_V1;

    typedef struct VAL_MATCH_V1 : public Endpoint {
        VAL_MATCH_V1(query_fp client_query) : Endpoint("val/match/v1/", client_query) {};
        Json::Value by_match(const_str_r routing, const_str_r match_id);
        Json::Value by_puuid(const_str_r routing, const_str_r puuid);
        Json::Value by_queue(const_str_r routing, const_str_r queue);

    } VAL_MATCH_V1;

    typedef struct VAL_RANKED_V1 : public Endpoint {
        VAL_RANKED_V1(query_fp client_query) : Endpoint("val/ranked/v1/", client_query) {};
        template <param ... Opts>
        Json::Value by_act(const_str_r routing, const_str_r actId, const Opts& ... optional_args) {
            const std::string method_key = "VAL_RANKED_V1-by-act";
            const std::array<std::string, 1> method_urls= {"leaderboards/by-act/"};
            std::shared_ptr<query> new_request = this->request(method_key, method_urls, routing, actId, OPTS(), optional_args ...);
            return (*this->_query)(new_request);
        };
    } VAL_RANKED_V1;

    typedef struct VAL_STATUS_V1 : public Endpoint {
        VAL_STATUS_V1(query_fp client_query) : Endpoint("val/status/v1/", client_query) {};
        Json::Value platform_data(const_str_r routing);

    } VAL_STATUS_V1;

}
