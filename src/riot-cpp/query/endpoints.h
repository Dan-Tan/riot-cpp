#pragma once

#include <bits/utility.h>
#include <string>
#include <cstring>
#include <string_view>
#include <type_traits>
#include <vector>
#include <functional>
#include <memory>
#include <numeric>
#include <ctime>
#include <variant>
#include <sstream>
#include <curl/curl.h>

#include "../types/args.h"
#include "url.h"

namespace riotcpp {
namespace query {
    
    typedef struct RiotHeader { // default to extremely slow rate limit successful requests will overwrite these
        char date[32];          // users with invalid api keys will only be able to send a request every 2 minutes
        char app_limit[64]          = "1:120";
        char app_limit_count[64]    = "1:120";
        char method_limit[64]       = "1:120";
        char method_limit_count[64] = "1:120";
        char retry_after[4];
    } RiotHeader;

    typedef struct query {
        std::string method_key;
        args::routing route;
        std::unique_ptr<char[]> url;
        std::time_t send_time = 0;
        std::unique_ptr<std::vector<char>> response_content;
        RiotHeader response_header;
        int last_response = -2;
        int server_error_count;
    } query;

    using json_text = std::vector<char>;
    using query_fp = std::function<std::unique_ptr<json_text>(std::shared_ptr<query>)>*;
    
    template<std::size_t N>
    inline int len_url_frags(const std::array<std::string, N>& url_frags) {
        return std::accumulate(
                url_frags.begin(),
                url_frags.end(),
                0,
                [](const int acc, const std::string& str){return acc + str.size();});
    }


    template<url::OptArg T, typename...Args>
    class EndpointMethod {
        private:
            // String will be held by the endpoint.
            const std::string& url_base_;
            const std::array<std::string, sizeof...(Args)> url_fragments_;
            const int frag_len_;
            const std::string additional_frag_;

            const std::string method_key_;
            const query_fp get_;

            bool validate_keywords(const std::pair<std::string, std::string>& opt_args...);

            std::unique_ptr<json_text> send_(const std::string& routing, std::unique_ptr<char[]> url) const;

        public:
            EndpointMethod(
                const query_fp get,
                const std::string& url_base,
                const std::string& method_key,
                const std::array<std::string, sizeof...(Args)> url_fragments,
                const std::string& additional_frag=""
            ) : get_(get),
                url_base_(url_base),
                method_key_(method_key),
                url_fragments_(url_fragments),
                additional_frag_(additional_frag),
                frag_len_(len_url_frags(url_fragments)) {};

            std::unique_ptr<json_text> operator()(const std::string& routing, const Args&...args, const T& opt_arg={}) const {
                std::unique_ptr<char[]> url;
                if constexpr (std::is_same<T, url::no_opt>::value) {
                    url = url::construct_url(routing, this->url_base_, this->additional_frag_, this->frag_len_, this->url_fragments_, args...);
                } else {
                    url = url::construct_url(routing, this->url_base_, opt_arg, this->additional_frag_, this->frag_len_, this->url_fragments_, args...);
                }
                return this->send_(routing, std::move(url));
            }
    };

    template<url::OptArg T, typename...Args>
    std::unique_ptr<json_text> EndpointMethod<T, Args...>::send_(const std::string& routing, std::unique_ptr<char[]> url) const {
        std::shared_ptr<query> new_request = std::make_shared<query>(this->method_key_,args::str_to_routing(routing), std::move(url)); 
        new_request->response_content = std::make_unique<json_text>();
        return (*this->get_)(new_request);
    }

    typedef struct Endpoint {
        const std::string url_base_;
    } Endpoint;

    typedef struct Account_v1 : public Endpoint {
        Account_v1(query_fp get) 
            : Endpoint("/riot/account/v1"), 
              by_puuid        (get, url_base_, "Account-v1-by-puuid", {"/accounts/by-puuid/"}),
              by_riot_id      (get, url_base_, "Acccount-v1-by-riot-id", {"/accounts/by-riot-id/", "/"}),
              by_game_by_puuid(get, url_base_, "Account-v1-by-game-by-puuid", {"/active-shards/by-game/", "/by-puuid/"}) {};

        const EndpointMethod<url::no_opt, std::string> by_puuid;
        const EndpointMethod<url::no_opt, std::string, std::string> by_riot_id;
        const EndpointMethod<url::no_opt, std::string, std::string> by_game_by_puuid;
    } Account_v1;

    typedef struct Champion_Mastery_v4 : public Endpoint {
        Champion_Mastery_v4(query_fp get)
            : Endpoint("/lol/champion-mastery/v4"), 
              by_puuid            (get, url_base_, "Champion-Mastery-v4-by-puuid",             {"/champion-masteries/by-puuid/"}), 
              by_puuid_by_champion(get, url_base_, "Champion-Mastery-v4-by-puuid-by-champion", {"/champion-masteries/by-puuid/", "/by-champion/"}),
              by_puuid_top        (get, url_base_, "Champion-Mastery-v4-by-puuid-top",         {"/champion-masteries/by-puuid/"}, "/top"), 
              scores_by_puuid     (get, url_base_, "Champion-Mastery-v4-scores-by-puuid",      {"/scores/by-puuid/"}) {}

        const EndpointMethod<url::no_opt, std::string> by_puuid;
        const EndpointMethod<url::no_opt, std::string, int> by_puuid_by_champion;
        const EndpointMethod<url::opt_count, std::string> by_puuid_top;
        const EndpointMethod<url::no_opt, std::string> scores_by_puuid;
    } Champion_Mastery_v4;

    typedef struct Champion_v3 : public Endpoint {
        Champion_v3(query_fp get)
            : Endpoint("/lol/platform/v3"),
              champion_rotations(get, url_base_, "Champion-v3=champion-rotations", {}, "/champion-rotations") {};

        const EndpointMethod<url::no_opt> champion_rotations;
    } Champion_v3;

    typedef struct Clash_v1 : public Endpoint{
        Clash_v1(query_fp get)
            : Endpoint("/lol/clash/v1"),
              by_summoner_id     (get, url_base_, "Clash-v1-by-summoner-id",   {"/players/by-summoner/"}),
              teams              (get, url_base_, "Clash-v1-teams",            {"/teams/"}),
              tournaments        (get, url_base_, "Clash-v1-tournaments"     , {}, "/tournaments"),
              tournaments_by_team(get, url_base_, "Clash-v1-by-team"         , {"/tournaments/by-team/"}),
              tournaments_by_id  (get, url_base_, "Clash-v1-tournament-by-id", {"/tournaments/"}) {}

        const EndpointMethod<url::no_opt, std::string> by_summoner_id;
        const EndpointMethod<url::no_opt, std::string> teams;
        const EndpointMethod<url::no_opt> tournaments;
        const EndpointMethod<url::no_opt, std::string> tournaments_by_team;
        const EndpointMethod<url::no_opt, int> tournaments_by_id;
    } Clash_v1;

    typedef struct League_exp_v4 : public Endpoint {
        League_exp_v4(query_fp get)
            : Endpoint("/lol/league-exp/v4"),
              entries(get, url_base_, "League-exp-v4-entries", {"/entries/", "/", "/"}) {};

        const EndpointMethod<url::opt_page, std::string, std::string, std::string> entries;
    } League_exp_v4;

    typedef struct League_v4 : public Endpoint {
        League_v4(query_fp get)
            : Endpoint("/lol/league/v4"),
              challenger    (get, url_base_, "League-v4-challenger", {"/challengerleagues/by-queue/"}),
              by_summoner_id(get, url_base_, "League-v4-by-summoner-id", {"/entries/by-summoner/"}),
              entries       (get, url_base_, "League-v4-entries", {"/entries/", "/", "/"}),
              grandmaster   (get, url_base_, "League-v4-grandmaster", {"/grandmasterleagues/by-queue/"}),
              by_league_id  (get, url_base_, "League-v4-by-league-id", {"/leagues/"}),
              master        (get, url_base_, "League-v4-master",      {"/masterleagues/by-queue/"}) {}

        const EndpointMethod<url::no_opt, std::string> challenger;
        const EndpointMethod<url::no_opt, std::string> by_summoner_id;
        const EndpointMethod<url::opt_page, std::string, std::string, std::string> entries;
        const EndpointMethod<url::no_opt, std::string> grandmaster;
        const EndpointMethod<url::no_opt, std::string> by_league_id;
        const EndpointMethod<url::no_opt, std::string> master;       
    } League_v4;

    typedef struct Lol_Challenges_v1 : public Endpoint {
        Lol_Challenges_v1(query_fp get)
            : Endpoint("/lol/challenges/v1"),
              config                     (get, url_base_, "Lol-Challenges-config",                      {}, "/challenges/config"),
              percentiles                (get, url_base_, "Lol-Challenges-percentilejs",                {}, "/challenges/percentiles"),
              config_by_id               (get, url_base_, "Lol-Challenges-config-by-id",                {"/challenges/"}, "/config"),
              leaderboards_by_id_by_level(get, url_base_, "Lol-Challenges-leaderboards-by-id-by-level", {"/challenges/", "/leaderboards/by-level/"}),
              percentiles_by_id          (get, url_base_, "Lol-Challenges-percentiles-by-id",           {"/challenges/"}, "/percentiles"),
              player_data_by_puuid       (get, url_base_, "Lol-Challenges-player-data-by-puuid",        {"/player-data/"}) {}

        const EndpointMethod<url::no_opt> config;
        const EndpointMethod<url::no_opt> percentiles;
        const EndpointMethod<url::no_opt, std::size_t> config_by_id;
        const EndpointMethod<url::opt_limit, std::string, std::size_t> leaderboards_by_id_by_level;
        const EndpointMethod<url::no_opt, std::size_t> percentiles_by_id;
        const EndpointMethod<url::no_opt, std::string> player_data_by_puuid;      
    } Lol_Challenges_v1;

    typedef struct Lol_Status_v4 : public Endpoint {
        Lol_Status_v4(query_fp get)
            : Endpoint("/lol/status/v4"),
              status(get, url_base_, "Lol-Status-v4-status", {}, "/platform-data") {};

        const EndpointMethod<url::no_opt> status;
    } Lol_Status_v4;

    typedef struct Lor_Match_v1 : public Endpoint {
        Lor_Match_v1(query_fp get)
            : Endpoint("/lor/match/v1"),
              by_puuid   (get, url_base_, "Lor-Match-v1-by-puuid",    {"/matches/by-puuid/"}, "/ids"),
              by_match_id(get, url_base_, "Lor-Match-v1-by-match-id", {"/matches/"}) {};

        const EndpointMethod<url::no_opt, std::string> by_puuid;
        const EndpointMethod<url::no_opt, std::string> by_match_id;
    } Lor_Match_v1;

    typedef struct Lor_Ranked_v1 : public Endpoint {
        Lor_Ranked_v1(query_fp get)
            : Endpoint("/lor/ranked/v1"),
              leaderboards(get, url_base_, "Lor-Ranked-v1-leaderboards", {}, "/leaderboards") {};

        const EndpointMethod<url::no_opt> leaderboards;
    } Lor_Ranked_v1;

    typedef struct Lor_Status_v1 : public Endpoint {
        Lor_Status_v1(query_fp get)
            : Endpoint("/lor/status/v1"),
              status(get, url_base_, "Lor-Status-v1", {}, "/platform-data"){};

        const EndpointMethod<url::no_opt> status;
    } Lor_Status_v1;

    typedef struct Match_v5 : public Endpoint {
        Match_v5(query_fp get)
            : Endpoint("/lol/match/v5"),
              by_puuid            (get, url_base_, "Match-v5-by-puuid",             {"/matches/by-puuid/"}, "/ids"),
              by_match_id         (get, url_base_, "Match-v5-by-match-id",          {"/matches/"}),
              timeline_by_match_id(get, url_base_, "Match-v5-timeline-by-match-id", {"/matches/"}, "/timeline") {};

        const EndpointMethod<url::opt_match_history, std::string> by_puuid;
        const EndpointMethod<url::no_opt, std::string> by_match_id;
        const EndpointMethod<url::no_opt, std::string> timeline_by_match_id;
    } Match_v5;

    typedef struct Spectator_Tft_v5 : public Endpoint {
        Spectator_Tft_v5(query_fp get)
            : Endpoint("/lol/spectator/tft/v5"),
              by_puuid(get, url_base_, "Spectator-Tft-v5-by-puuid", {"/active-games/by-puuid/"}),
              featured(get, url_base_, "Spectator-Tft-v5=featured", {}, "/featured-games") {};

        const EndpointMethod<url::no_opt, std::string> by_puuid;
        const EndpointMethod<url::no_opt> featured;
    } Spectator_Tft_v5;

    typedef struct Spectator_v5 : public Endpoint {
        Spectator_v5(query_fp get)
            : Endpoint("/lol/spectator/v5"),
              by_summoner(get, url_base_, "Spectator-v5-by-summoner-id", {"/active-games/by-summoner/"}),
              featured   (get, url_base_, "Spectator-v5-featured",       {}, "/featured-games") {};

        const EndpointMethod<url::no_opt, std::string> by_summoner;
        const EndpointMethod<url::no_opt> featured;
    } Spectator_v5;

    typedef struct Summoner_v4 : public Endpoint {
        Summoner_v4(query_fp get)
            : Endpoint("/lol/summoner/v4"),
              by_account_id (get, url_base_, "Summoner-v4-by-account-id",  {"/summoners/by-account/"}),
              by_puuid      (get, url_base_, "Summoner-v4-by-puuid",       {"/summoners/by-puuid/"}),
              by_summoner_id(get, url_base_, "Summoner-v4-by-summoner-id", {"/summoners/"}) {};

        const EndpointMethod<url::no_opt, std::string> by_account_id;
        const EndpointMethod<url::no_opt, std::string> by_puuid;
        const EndpointMethod<url::no_opt, std::string> by_summoner_id;
    } Summoner_v4;

    typedef struct Tft_League_v1 : public Endpoint {
        Tft_League_v1(query_fp get)
            : Endpoint("/tft/league/v1"),
              challenger  (get, url_base_, "Tft-League-v1-challenger",   {}, "/challenger"),
              by_summoner (get, url_base_, "Tft-League-v1-by-summoner",  {"/entries/by-summoner/"}),
              entries     (get, url_base_, "Tft-League-v1-entries",      {"/entries/", "/"}),
              grandmaster (get, url_base_, "Tft-League-v1-grandmaster",  {}, "/grandmaster"),
              by_league_id(get, url_base_, "Tft-League-v1-by-league-id", {"/leagues/"}),
              master      (get, url_base_, "Tft-League-v1-master",       {}, "/master"),
              top_by_queue(get, url_base_, "Tft-League-v1-top-by-queue", {"/rated-ladders/"}, "/top") {}

        const EndpointMethod<url::opt_queue> challenger;
        const EndpointMethod<url::no_opt, std::string> by_summoner;
        const EndpointMethod<url::opt_queue_page, std::string, std::string> entries;
        const EndpointMethod<url::opt_queue> grandmaster;
        const EndpointMethod<url::no_opt, std::string> by_league_id;
        const EndpointMethod<url::opt_queue> master;
        const EndpointMethod<url::no_opt, std::string> top_by_queue;
    } Tft_League_v1;

    typedef struct Tft_Match_v1 : public Endpoint {
        Tft_Match_v1(query_fp get)
            : Endpoint("/tft/match/v1"),
              by_puuid(   get, url_base_, "Tft-Match-v1-by-puuid",    {"/matches/by-puuid/"}, "/ids"),
              by_match_id(get, url_base_, "Tft-Match-v1-by-match-id", {"/matches/"}) {};

        const EndpointMethod<url::opt_tft_match_history, std::string> by_puuid;
        const EndpointMethod<url::no_opt, std::string> by_match_id;
    } Tft_Match_v1;

    typedef struct Tft_Status_v1 : public Endpoint {
        Tft_Status_v1(query_fp get)
            : Endpoint("/tft/status/v1"),
              status(get, url_base_, "Tft-Status-v1-status", {}, "/platform-data") {};

        const EndpointMethod<url::no_opt> status;
    } Tft_Status_v1;

    typedef struct Tft_Summoner_v1 : public Endpoint {
        Tft_Summoner_v1(query_fp get)
            : Endpoint("/tft/summoner/v1"),
              by_account_id (get, url_base_, "Tft-Summoner-v1-by-account-id",  {"/summoners/by-account/"}),
              by_puuid      (get, url_base_, "Tft-Summoner-v1-by-puuid",       {"/summoners/by-puuid/"}),
              by_summoner_id(get, url_base_, "Tft-Summoner-v1-by-summoner-id", {"/summoners/"}) {};

        const EndpointMethod<url::no_opt, std::string> by_account_id;
        const EndpointMethod<url::no_opt, std::string> by_puuid;
        const EndpointMethod<url::no_opt, std::string> by_summoner_id;
    } Tft_Summoner_v1;

    typedef struct Val_Content_v1 : public Endpoint {
        Val_Content_v1(query_fp get)
            : Endpoint("/val/content/v1"),
              contents(get, url_base_, "Val-Content-v1-contents", {}, "/contents") {};

        const EndpointMethod<url::opt_locale> contents; 
    } Val_Content_v1;

    typedef struct Val_Match_v1 : public Endpoint {
        Val_Match_v1(query_fp get)
            : Endpoint("/val/match/v1"),
              by_match_id    (get, url_base_, "Val-Match-v1-by-match-id",     {"/matches/"}), 
              by_puuid       (get, url_base_, "Val-Match-v1-by-puuid",        {"/matches/by-puuid/"}),
              recent_by_queue(get, url_base_, "Val-Match-v1-recent-by-queue", {"/recent-matches/by-queue/"}) {};

        const EndpointMethod<url::no_opt, std::string> by_match_id;
        const EndpointMethod<url::no_opt, std::string> by_puuid;
        const EndpointMethod<url::no_opt, std::string> recent_by_queue;
    } Val_Match_v1;

    typedef struct Val_Ranked_v1 : public Endpoint {
        Val_Ranked_v1(query_fp get)
            : Endpoint("/val/ranked/v1"),
              leaderboards_by_act(get, url_base_, "Val-Ranked-v1-leaderbords-by-act", {"/leaderboards/by-act/"}) {};

        const EndpointMethod<url::opt_start_size, std::string> leaderboards_by_act; 
    } Val_Ranked_v1;

    typedef struct Val_Status_v1 : public Endpoint {
        Val_Status_v1(query_fp get)
            : Endpoint("/val/status/v1"),
              status(get, url_base_, "Val-Status-v1-status", {}, "/platform-data") {};

        const EndpointMethod<url::no_opt> status;
    } Val_Status_v1;
}
}
