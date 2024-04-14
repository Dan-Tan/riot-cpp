#pragma once

#include <bits/utility.h>
#include <string>
#include <string_view>
#include <vector>
#include <functional>
#include <memory>
#include <ctime>
#include <variant>
#include <sstream>
#include <curl/curl.h>

#include "../types/args.h"

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
        std::string url;
        std::time_t send_time = 0;
        std::unique_ptr<std::vector<char>> response_content;
        RiotHeader response_header;
        int last_response = -2;
        int server_error_count;
    } query;
    
    // Replace with an implementation to avoid allocations
    inline const std::string encode_params(const std::string& to_encode) {
        char* encoded = curl_easy_escape(NULL, to_encode.c_str(), to_encode.length());
        const std::string encoded_str(encoded);
        curl_free(encoded);
        return encoded_str;
    }

    template<typename T>
    concept Param = requires(std::ostream& b, T a) {
        b << a;
    };

    using json_text = std::vector<char>;
    using query_fp = std::function<std::unique_ptr<json_text>(std::shared_ptr<query>)>*;

    inline std::size_t length(const std::string& arg) {
        return arg.size();
    }
    inline std::size_t length(const std::string& arg, const std::string& args...) {
        return arg.size() + length(args);
    }
    
    template<std::size_t N, Param...Args, typename = typename std::enable_if<sizeof...(Args) == N>>
    void construct_required(std::string& url_base, const std::array<std::string, N>& url_fragments, const Args&...args) {
        std::size_t counter = 0;
        ([&] {
            url_base += url_fragments[counter] + encode_params(args);
            ++counter;
        } (), ...);
    }
    
    template<Param...OptArgs>
    void construct_optional(std::string& url_base, const std::pair<std::string, OptArgs>&... opt_args) {
        bool first = true;
        ([&] {
            url_base += (first ? "?" : "&") + std::get<0>(opt_args) + "=" + std::get<1>(opt_args);
            first = false;
        } (), ...);
    }

    template<std::size_t N_REQ, std::size_t N_OPT>
    class EndpointMethod;
    
    // Endpoint implementation with no optional arguements
    template<std::size_t N_REQ>
    class EndpointMethod<N_REQ, 0> {
        private:
            const std::string& url_base_;
            const std::array<std::string, N_REQ> url_fragments_;
            const std::string additional_frag_;

            const std::string method_key_;
            const query_fp get_;
        
        public:
            EndpointMethod(
                const query_fp get,
                const std::string& url_base,
                const std::string& method_key,
                const std::array<std::string, N_REQ> url_fragments,
                const std::string& additional_frag=""
            ) : get_(get),
                url_base_(url_base),
                method_key_(method_key),
                url_fragments_(url_fragments),
                additional_frag_(additional_frag) {}

            template<Param...Args, 
                typename = typename std::enable_if<sizeof...(Args) == N_REQ>::type>
            std::unique_ptr<json_text> operator()(const std::string& routing, const Args&... args) const {
                std::string url = "https://" + routing + ".api.riotgames.com" + this->url_base_;
                construct_required(url, this->url_fragments_, args...);
                url += this->additional_frag;
                std::shared_ptr<query> new_request = std::make_shared<query>(this->method_key_, args::str_to_routing(routing), url);
                new_request->response_content = std::make_unique<json_text>();
                return (*this->get_)(new_request);
            };
    };

    template<std::size_t N_REQ, std::size_t N_OPT>
    class EndpointMethod {
        private:
            // String will be held by the endpoint.
            const std::string& url_base_;
            const std::array<std::string, N_REQ> url_fragments_;
            const std::string additional_frag_;
            const std::array<std::string, N_OPT> opt_keywords_;

            const std::string method_key_;
            const query_fp get_;

            bool validate_keywords(const std::pair<std::string, std::string>& opt_args...);

        public:
            EndpointMethod(
                const query_fp get,
                const std::string& url_base,
                const std::string& method_key,
                const std::array<std::string, N_REQ> url_fragments,
                const std::array<std::string, N_OPT> opt_keywords,
                const std::string& additional_frag=""
            ) : get_(get),
                url_base_(url_base),
                method_key_(method_key),
                url_fragments_(url_fragments),
                additional_frag_(additional_frag),
                opt_keywords_(opt_keywords) {}

            template<Param...Args, Param...OptArgs,
                typename = typename std::enable_if<sizeof...(Args) == N_REQ>::type,
                typename = typename std::enable_if<sizeof...(OptArgs) <= N_OPT>::type>
            std::unique_ptr<json_text> operator()(const std::string& routing, const Args&... args, const std::pair<std::string, OptArgs>&... opt_args) const {
                std::string url = "https://" + routing + ".api.riotgames.com" + this->url_base_;
                construct_required(url, this->url_fragments_, args...);
                url += this->additional_frag;
                if constexpr (sizeof...(OptArgs) == 0) {
                    construct_optional(url, opt_args...);
                }
                std::shared_ptr<query> new_request = std::make_shared<query>(this->method_key_, args::str_to_routing(routing), url);
                new_request->response_content = std::make_unique<json_text>();
                return (*this->get_)(new_request);
            };
    };

    typedef struct Endpoint {
        const std::string url_base_;
    } Endpoint;

    typedef struct Account_v1 : public Endpoint {
        Account_v1(query_fp get) 
            : Endpoint("/riot/account/v1"), 
              by_puuid        (get, url_base_, "Account-v1-by-puuid", {"/accounts/by-puuid/"}),
              by_riot_id      (get, url_base_, "Acccount-v1-by-riot-id", {"/accounts/by-riot-id/", "/"}),
              by_game_by_puuid(get, url_base_, "Account-v1-by-game-by-puuid", {"/active-shards/by-game/", "/by-puuid/"}) {};

        const EndpointMethod<1, 0> by_puuid;
        const EndpointMethod<2, 0> by_riot_id;
        const EndpointMethod<2, 0> by_game_by_puuid;
    } Account_v1;

    typedef struct Champion_Mastery_v4 : public Endpoint {
        Champion_Mastery_v4(query_fp get)
            : Endpoint("/lol/champion-mastery/v4"), 
              by_puuid            (get, url_base_, "Champion-Mastery-v4-by-puuid",             {"champion-masteries/by-puuid/"}), 
              by_puuid_by_champion(get, url_base_, "Champion-Mastery-v4-by-puuid-by-champion", {"champion-masteries/by-puuid/", "/by-champion/"}, {"count"}),
              by_puuid_top        (get, url_base_, "Champion-Mastery-v4-by-puuid-top",         {"champion-masteries/by-puuid/"}, "/top"), 
              scores_by_puuid     (get, url_base_, "Champion-Mastery-v4-scores-by-puuid",      {"scores/by-puuid/"}) {}

        const EndpointMethod<1, 0> by_puuid;
        const EndpointMethod<2, 0> by_puuid_by_champion;
        const EndpointMethod<1, 1> by_puuid_top;
        const EndpointMethod<1, 0> scores_by_puuid;
    } Champion_Mastery_v4;

    typedef struct Champion_v3 : public Endpoint {
        Champion_v3(query_fp get)
            : Endpoint("/lol/platform/v3"),
              champion_rotations(get, url_base_, "Champion-v3=champion-rotations", {}, "/champion-rotations") {};

        const EndpointMethod<0, 0> champion_rotations;
    } Champion_v3;

    typedef struct Clash_v1 : public Endpoint{
        Clash_v1(query_fp get)
            : Endpoint("/lol/clash/v1"),
              by_summoner_id     (get, url_base_, "Clash-v1-by-summoner-id",   {"/players/by-summoner/"}),
              teams              (get, url_base_, "Clash-v1-teams",            {"/teams/"}),
              tournaments        (get, url_base_, "Clash-v1-tournaments"     , {}, "/tournaments"),
              tournaments_by_team(get, url_base_, "Clash-v1-by-team"         , {"/tournaments/by-team/"}),
              tournaments_by_id  (get, url_base_, "Clash-v1-tournament-by-id", {"/tournaments/"}) {}

        const EndpointMethod<1, 0> by_summoner_id;
        const EndpointMethod<1, 0> teams;
        const EndpointMethod<0, 0> tournaments;
        const EndpointMethod<1, 0> tournaments_by_team;
        const EndpointMethod<1, 0> tournaments_by_id;
    } Clash_v1;

    typedef struct League_exp_v4 : public Endpoint {
        League_exp_v4(query_fp get)
            : Endpoint("/lol/league-exp/v4"),
              entries(get, url_base_, "League-exp-v4-entries", {"/entries/", "/", "/"}, {"page"}) {};

        const EndpointMethod<3, 1> entries;
    } League_exp_v4;

    typedef struct League_v4 : public Endpoint {
        League_v4(query_fp get)
            : Endpoint("/lol/league/v4"),
              challenger    (get, url_base_, "League-v4-challenger", {"/challengerleagues/by-queue/"}),
              by_summoner_id(get, url_base_, "League-v4-by-summoner-id", {"/entries/by-summoner/"}),
              entries       (get, url_base_, "League-v4-entries", {"/entries/", "/", "/"}),
              grandmaster   (get, url_base_, "League-v4-grandmaster", {"/grandmasterleagues/by-queue/"}),
              by_league_id  (get, url_base_, "League-v4-by-league-id", {"/leagues/"}),
              master        (get, url_base_, "League-v4-master",      {"/grandmasterleagues/by-queue/"}) {}

        const EndpointMethod<1, 0> challenger;
        const EndpointMethod<1, 0> by_summoner_id;
        const EndpointMethod<3, 0> entries;
        const EndpointMethod<1, 0> grandmaster;
        const EndpointMethod<1, 0> by_league_id;
        const EndpointMethod<1, 0> master;       
    } League_v4;

    typedef struct Lol_Challenges_v1 : public Endpoint {
        Lol_Challenges_v1(query_fp get)
            : Endpoint("/lol/challenges/v1"),
              config                     (get, url_base_, "Lol-Challenges-config",                      {}, "/challenges/config"),
              percentiles                (get, url_base_, "Lol-Challenges-percentilejs",                {}, "/challenges/percentiles"),
              config_by_id               (get, url_base_, "Lol-Challenges-config-by-id",                {"/challenges/"}, "/config"),
              leaderboards_by_id_by_level(get, url_base_, "Lol-Challenges-leaderboards-by-id-by-level", {"/challenges/", "/leaderboards/by-level/"}, {"limit"}),
              percentiles_by_id          (get, url_base_, "Lol-Challenges-percentiles-by-id",           {"/challenges/"}, "/percentiles"),
              player_data_by_puuid       (get, url_base_, "Lol-Challenges-player-data-by-puuid",        {"/player-data/"}) {}

        const EndpointMethod<0, 0> config;
        const EndpointMethod<0, 0> percentiles;
        const EndpointMethod<1, 0> config_by_id;
        const EndpointMethod<2, 1> leaderboards_by_id_by_level;
        const EndpointMethod<1, 0> percentiles_by_id;
        const EndpointMethod<1, 0> player_data_by_puuid;      
    } Lol_Challenges_v1;
}
}
