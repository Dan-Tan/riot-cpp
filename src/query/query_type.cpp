#include <string>
#include <tuple>
#include <numeric>
#include <ctime>
#include <vector>
#include <utility>
#include <stdexcept>
#include "query.h"

namespace query {

template <param P, param R>
static inline std::string opt_helper(const std::stringstream& accumm, const P& arg_key, const R& arg) {
    return accumm << arg_key << "=" << arg << "&";
}

template <param ... Opts, std::size_t ... Ixs>
static inline void construct_args(std::stringstream& accum, const std::tuple<Opts...>& opts) {
    (opt_helper(accum, std::get<2*Ixs>(opts), std::get<2*Ixs+1>(opts)), ...);
}

template <param ... opts>
std::string query_construct(const opts&... optional_args){
    std::stringstream ss;
    constexpr std::size_t n_args = sizeof...(optional_args);
    static_assert(n_args%2 == 0, "Optional arguments must be passed in pairs");
    const std::index_sequence<n_args/2-1> n_passed = std::make_index_sequence<sizeof...(opts)/2-1>{};
    const std::tuple<opts...> store(optional_args...);
    construct_args(ss, store);
    ss << std::get<n_args-2>(store) << "=" << std::get<n_args-1>(store);
    return ss.str();
}

template <param R>
std::stringstream Endpoint::construct_base(const R& routing) const {
    std::stringstream ss;
    ss << "https://" << routing << ".api.riotgames.com/" << this->_url;
    return ss;
}

template <param P>
static inline void full_helper(std::stringstream& accum, const std::string& url_seg, const P& param) {
    accum << url_seg << param;
}

template <std::size_t N, param ... Params, std::size_t ... Ixs>
static inline void full_iter(std::stringstream& accum, const std::array<std::string, N>& method_urls, const std::tuple<Params...>& params, std::index_sequence<Ixs ...>) {
    (full_helper(accum, method_urls[Ixs], std::get<Ixs>(params)), ...);
}

template <std::size_t N, param Routing, param ... Params>
std::string Endpoint::full_query(const std::array<std::string, N>& method_urls, const Routing& routing, const Params& ... params) const {
    std::stringstream base_url = this->construct_base(routing);
    const std::tuple<Params...> store(params...);
    const std::size_t n_params = sizeof ...(Params);
    full_iter(base_url, method_urls, store, std::make_index_sequence<N-1>{});
    if constexpr (n_params < N) {
        base_url << method_urls[N-1];
    } else if constexpr (n_params == N) {
        base_url << method_urls[N-1] << std::get<n_params - 1>(store);
    }
    return base_url.str();
}

template <std::size_t N, param Routing, param ... Params, param ... Opts>
std::string Endpoint::full_query(const std::array<std::string, N>& method_urls, const Routing& routing, const Params& ... params, OPTS, const Opts& ... optional_args) const {
    std::string final_url = this->full_query(method_urls, routing, params...);
    std::string optional = query_construct(optional_args...);
    final_url += optional;
    return final_url;
}
template <param R>
std::shared_ptr<query> Endpoint::request(const std::string& key, const std::string& method_url, const R& routing) {
    std::string final_url = this->construct_base(routing).str();
    std::shared_ptr<query> request = std::make_shared<query>(key, routing, final_url);
    return {};
}

template <std::size_t N, param Routing, param ... Params>
std::shared_ptr<query> Endpoint::request(const std::string& key, const std::array<std::string, N>& method_urls, const Routing& routing, const Params& ... params) {
    std::string final_url = this->full_query(method_urls, routing, params...);
    std::shared_ptr<query> request = std::make_shared<query>(key, routing, final_url);
    return request;
}

template <std::size_t N, param Routing, param ... Params, param ... Opts>
std::shared_ptr<query> Endpoint::request(const std::string& key, const std::array<std::string, N>& method_urls, const Routing& routing, const Params& ... params, OPTS, const Opts& ... optional_args) {
    std::string final_url = this->full_query(method_urls, routing, params..., {}, optional_args...);
    std::shared_ptr<query> request = std::make_shared<query>(key, routing, final_url);
    return request;
}

Json::Value ACCOUNT_V1::by_puuid(const std::string& routing, const std::string& puuid) {
    const std::string method_key = "ACCOUNT-V1-by-puuid";
    const std::array<std::string, 1> method_urls= {"accounts/by-puuid/"};
    std::shared_ptr<query> new_request = this->request(method_key, method_urls, routing, puuid);
    return (*this->_query)(new_request);
}

Json::Value ACCOUNT_V1::by_riot_id(const std::string& routing, const std::string& gameName, const std::string& tagline) {
    const std::string method_key = "ACCOUNT-V1-by-riot-id";
    const std::array<std::string, 2> method_urls= {"accounts/by-riot-id/", "/"};
    std::shared_ptr<query> new_request = this->request(routing, method_urls, routing, gameName, tagline);
    return (*this->_query)(new_request);
}

Json::Value ACCOUNT_V1::by_game(const std::string& routing, const std::string& game, const std::string& puuid) {
    const std::string method_key = "ACCOUNT-V1-by-game";
    const std::array<std::string, 2> method_urls= {"active-shards/by-game/", "/by-puuid/"};
    std::shared_ptr<query> new_request = this->request(routing, method_urls, routing, game, puuid);
    return (*this->_query)(new_request);
}

Json::Value CHAMPION_MASTERY_V4::by_summoner_id(const std::string& routing, const std::string& summoner_id) {
    const std::string method_key("CHAMPION-MASTERY-V4-by-summoner-id");
    const std::array<std::string, 1> method_urls{"champion-masteries/by-summoner/"};
    std::shared_ptr<query> new_request = this->request(method_key, method_urls, routing, summoner_id);
    return (*this->_query)(new_request);
}

Json::Value CHAMPION_MASTERY_V4::by_summoner_by_champion(const std::string &routing, const std::string &summoner_id, const std::string &champion_id) {
    const std::string method_key("CHAMPION-MASTERY-V4-by-summoner-by-champion");
    const std::array<std::string, 2> method_urls{"champion-masteries/by-summoner/", "/by-champion/"};
    std::shared_ptr<query> new_request = this->request(method_key, method_urls, routing, summoner_id, champion_id);
    return (*this->_query)(new_request);
}

Json::Value CHAMPION_MASTERY_V4::scores_by_summoner(const std::string &routing, const std::string &summoner_id) {
    const std::string method_key("CHAMPION-MASTERY-V4-scores-by-summoner");
    const std::array<std::string, 1> method_urls{"scores/by-summoner/"};
    std::shared_ptr<query> new_request = this->request(method_key, method_urls, routing, summoner_id);
    return (*this->_query)(new_request);
}

Json::Value CHAMPION_V3::rotations(const std::string &routing) {
    const std::string method_key("CHAMPION-V3-rotations");
    const std::array<std::string, 1> method_urls{"champion-rotations"};
    std::shared_ptr<query> new_request = this->request(method_key, method_urls, routing);
    return (*this->_query)(new_request);
}

Json::Value CLASH_V1::by_summoner_id(const std::string &routing, const std::string &summoner_id) {
    const std::string method_key("CLASH-V1-by-summoner-id");
    const std::array<std::string, 1> method_urls{"by-summoner"};
    std::shared_ptr<query> new_request = this->request(method_key, method_urls, routing, summoner_id);
    return (*this->_query)(new_request);
}

Json::Value CLASH_V1::by_team(const std::string &routing, const std::string &team) {
    const std::string method_key("CLASH-V1-by-team");
    const std::array<std::string, 1> method_urls{"teams/"};
    std::shared_ptr<query> new_request = this->request(method_key, method_urls, routing, team);
    return (*this->_query)(new_request);
}

Json::Value CLASH_V1::tournament_by_team(const std::string &routing, const std::string &team) {
    const std::string method_key("CLASH-V1-tournament-by-team");
    const std::array<std::string, 1> method_urls{"tournaments/by-team"};
    std::shared_ptr<query> new_request = this->request(method_key, method_urls, routing, team);
    return (*this->_query)(new_request);
}

Json::Value CLASH_V1::by_tournament(const std::string &routing, const std::string &tournament) {
    const std::string method_key("CLASH-V1-by-tournament");
    const std::array<std::string, 1> method_urls{"tournaments/"};
    std::shared_ptr<query> new_request = this->request(method_key, method_urls, routing, tournament);
    return (*this->_query)(new_request);
}

Json::Value LEAGUE_V4::challenger(const_str_r routing, const_str_r queue) {
    const std::string method_key("LEAGUE-V4-challenger");
    const std::array<std::string, 1> method_urls{"challengerleagues/by-queue/"};
    std::shared_ptr<query> new_request = this->request(method_key, method_urls, routing, queue);
    return (*this->_query)(new_request);
}

Json::Value LEAGUE_V4::grandmaster(const_str_r routing, const_str_r queue) {
    const std::string method_key("LEAGUE-V4-grandmaster");
    const std::array<std::string, 1> method_urls{"grandmasterleagues/by-queue/"};
    std::shared_ptr<query> new_request = this->request(method_key, method_urls, routing, queue);
    return (*this->_query)(new_request);
}

Json::Value LEAGUE_V4::master(const_str_r routing, const_str_r queue) {
    const std::string method_key("LEAGUE-V4-master");
    const std::array<std::string, 1> method_urls{"masterleagues/by-queue/"};
    std::shared_ptr<query> new_request = this->request(method_key, method_urls, routing, queue);
    return (*this->_query)(new_request);
}

Json::Value LEAGUE_V4::by_summoner_id(const_str_r routing, const_str_r summoner_id) {
    const std::string method_key("LEAGUE-V4-by-summoner-id");
    const std::array<std::string, 1> method_urls{"entries/by-summoner/"};
    std::shared_ptr<query> new_request = this->request(method_key, method_urls, routing, summoner_id);
    return (*this->_query)(new_request);
}

Json::Value LEAGUE_V4::by_league_id(const_str_r routing, const_str_r league_id) {
    const std::string method_key("LEAGUE-V4-by-league-id");
    const std::array<std::string, 1> method_urls{"leagues/"};
    std::shared_ptr<query> new_request = this->request(method_key, method_urls, routing, league_id);
    return (*this->_query)(new_request);
}

Json::Value LOL_CHALLENGES_V1::config(const std::string &routing) {
    const std::string method_key("LOL-CHALLENGES-V1-config");
    const std::array<std::string, 1> method_urls{"challenges/config"};
    std::shared_ptr<query> new_request = this->request(method_key, method_urls, routing);
    return (*this->_query)(new_request);
}
Json::Value LOL_CHALLENGES_V1::percentiles(const std::string &routing) {
    const std::string method_key("LOL-CHALLENGES-V1-percentiles");
    const std::array<std::string, 1> method_urls{"challenges/percentiles"};
    std::shared_ptr<query> new_request = this->request(method_key, method_urls, routing);
    return (*this->_query)(new_request);
}
Json::Value LOL_CHALLENGES_V1::challenge_config(const std::string& routing, const std::string &challenge_id) {
    const std::string method_key("LOL-CHALLENGES-V1-challenge-config");
    const std::array<std::string, 2> method_urls{"challenges/", "/config"};
    std::shared_ptr<query> new_request = this->request(method_key, method_urls, routing, challenge_id);
    return (*this->_query)(new_request);
}

Json::Value LOL_CHALLENGES_V1::challenge_percentiles(const std::string& routing, const std::string& challenge_id) {
    const std::string method_key("LOL-CHALLENGES-V1-challenge-percentiles");
    const std::array<std::string, 2> method_urls{"challenges/", "/percentiles"};
    std::shared_ptr<query> new_request = this->request(method_key, method_urls, routing, challenge_id);
    return (*this->_query)(new_request);
}
Json::Value LOL_CHALLENGES_V1::by_puuid(const std::string& routing, const std::string& puuid) {
    const std::string method_key("LOL-CHALLENGES-V1-by-puuid");
    const std::array<std::string, 1> method_urls{"player-data/"};
    std::shared_ptr<query> new_request = this->request(method_key, method_urls, routing, puuid);
    return (*this->_query)(new_request);
}

Json::Value LOL_STATUS::v3(const std::string& routing) {
    const std::string method_key("LOL-STATUS-v3");
    const std::array<std::string, 1> method_urls{"v3/shard-data"};
    std::shared_ptr<query> new_request = this->request(method_key, method_urls, routing);
    return (*this->_query)(new_request);
}

Json::Value LOL_STATUS::v4(const std::string& routing) {
    const std::string method_key("LOL-STATUS-v4");
    const std::array<std::string, 1> method_urls{"v4/platform-data"};
    std::shared_ptr<query> new_request = this->request(method_key, method_urls, routing);
    return (*this->_query)(new_request);
}

Json::Value LOR_MATCH_V1::by_puuid(const std::string& routing, const std::string& puuid) {
    const std::string method_key("LOR-MATCH-V1-by-puuid");
    const std::array<std::string, 2> method_urls{"matches/by-puuid/", "/ids"};
    std::shared_ptr<query> new_request = this->request(method_key, method_urls, routing, puuid);
    return (*this->_query)(new_request);
}

Json::Value LOR_MATCH_V1::by_match(const std::string& routing, const std::string& match) {
    const std::string method_key("LOR-MATCH-V1-by-match");
    const std::array<std::string, 1> method_urls{"matches/"};
    std::shared_ptr<query> new_request = this->request(method_key, method_urls, routing, match);
    return (*this->_query)(new_request);
}

Json::Value LOR_RANKED_V1::leaderboards(const std::string& routing) {
    const std::string method_key("LOR-RANKED-V1-leaderboards");
    const std::array<std::string, 1> method_urls{"leaderboards"};
    std::shared_ptr<query> new_request = this->request(method_key, method_urls, routing);
    return (*this->_query)(new_request);
}

Json::Value LOR_STATUS_V1::v1(const std::string& routing) {
    const std::string method_key("LOR-STATUS-V1-v1");
    const std::array<std::string, 1> method_urls{"platform-data"};
    std::shared_ptr<query> new_request = this->request(method_key, method_urls, routing);
    return (*this->_query)(new_request);
}

Json::Value MATCH_V5::by_match_id(const std::string& routing, const std::string& match_id) {
    const std::string method_key = "MATCH-V5-by-match-id";
    const std::array<std::string, 1> method_urls = {"matches/"};
    std::shared_ptr<query> new_request = this->request(method_key, method_urls, routing, match_id);
    return (*this->_query)(new_request);
}

Json::Value MATCH_V5::timeline(const std::string& routing, const std::string& match_id) {
    const std::string method_key = "MATCH-V5-timeline";
    const std::array<std::string, 2> method_urls = {"matches/", "/timeline"};
    std::shared_ptr<query> new_request = this->request(method_key, method_urls, routing, match_id);
    return (*this->_query)(new_request);
}

Json::Value SUMMONER_V4::by_rso_puuid(const std::string& routing, const std::string& puuid) {
    const std::string method_key = "SUMMONER-V4-by-rso-puuid";
    const std::array<std::string, 1> method_urls = {"fulfillment/v1/summoners/by-puuid/"};
    std::shared_ptr<query> new_request = this->request(method_key, method_urls, routing, puuid);
    return (*this->_query)(new_request);
}

Json::Value SUMMONER_V4::by_account_id(const std::string& routing, const std::string& account_id) {
    const std::string method_key = "SUMMONER-V4-by-account-id";
    const std::array<std::string, 1> method_urls = {"summoners/by-account/"};
    std::shared_ptr<query> new_request = this->request(method_key, method_urls, routing, account_id);
    return (*this->_query)(new_request);
}

Json::Value SUMMONER_V4::by_name(const std::string& routing, const std::string& name) {
    const std::string method_key = "SUMMONER-V4-by-name";
    const std::array<std::string, 1> method_urls = {"summoners/by-name/"};
    std::shared_ptr<query> new_request = this->request(method_key, method_urls, routing, name);
    return (*this->_query)(new_request);
}

Json::Value SUMMONER_V4::by_puuid(const std::string& routing, const std::string& puuid) {
    const std::string method_key = "SUMMONER-V4-by-puuid";
    const std::array<std::string, 1> method_urls = {"summoners/by-puuid/"};
    std::shared_ptr<query> new_request = this->request(method_key, method_urls, routing, puuid);
    return (*this->_query)(new_request);
}

Json::Value SUMMONER_V4::by_summoner_id(const std::string& routing, const std::string& summoner_id) {
    const std::string method_key = "SUMMONER-V4-by-summoner-id";
    const std::array<std::string, 1> method_urls = {"summoners/"};
    std::shared_ptr<query> new_request = this->request(method_key, method_urls, routing, summoner_id);
    return (*this->_query)(new_request);
}

Json::Value SPECTATOR_V4::by_summoner_id(const std::string& routing, const std::string& summonerId) {
    const std::string method_key = "SPECTATOR-V4-by-summoner-id";
    const std::array<std::string, 1> method_urls= {"active-games/by-summoner/"};
    std::shared_ptr<query> new_request = this->request(method_key, method_urls, routing, summonerId);
    return (*this->_query)(new_request);
}

Json::Value SPECTATOR_V4::featured_games(const std::string& routing) {
    const std::string method_key = "SPECTATOR-V4-featured-games";
    const std::array<std::string, 1> method_urls= {"featured-games"};
    std::shared_ptr<query> new_request = this->request(method_key, method_urls, routing);
    return (*this->_query)(new_request);
}

Json::Value TFT_LEAGUE_V1::challenger(const std::string& routing) {
    const std::string method_key = "TFT-LEAGUE-V1-challenger";
    const std::array<std::string, 1> method_urls = { "challenger" };
    std::shared_ptr<query> new_request = this->request(method_key, method_urls, routing);
    return (*this->_query)(new_request);
}

Json::Value TFT_LEAGUE_V1::grandmaster(const std::string& routing) {
    const std::string method_key = "TFT-LEAGUE-V1-grandmaster";
    const std::array<std::string, 1> method_urls = { "grandmaster" };
    std::shared_ptr<query> new_request = this->request(method_key, method_urls, routing);
    return (*this->_query)(new_request);
}

Json::Value TFT_LEAGUE_V1::master(const std::string& routing) {
    const std::string method_key = "TFT-LEAGUE-V1-master";
    const std::array<std::string, 1> method_urls = { "master" };
    std::shared_ptr<query> new_request = this->request(method_key, method_urls, routing);
    return (*this->_query)(new_request);
}

Json::Value TFT_LEAGUE_V1::by_summoner_id(const std::string& routing, const std::string& summoner_id) {
    const std::string method_key = "TFT-LEAGUE-V1-by-summoner-id";
    const std::array<std::string, 2> method_urls = { "entries/by-summoner/", "" };
    std::shared_ptr<query> new_request = this->request(method_key, method_urls, routing, summoner_id);
    return (*this->_query)(new_request);
}

Json::Value TFT_LEAGUE_V1::by_league_id(const std::string& routing, const std::string& league_id) {
    const std::string method_key = "TFT-LEAGUE-V1-by-league-id";
    const std::array<std::string, 2> method_urls = { "leagues/", "" };
    std::shared_ptr<query> new_request = this->request(method_key, method_urls, routing, league_id);
    return (*this->_query)(new_request);
}

Json::Value TFT_LEAGUE_V1::queue_top(const std::string& routing, const std::string& queue) {
    const std::string method_key = "TFT_LEAGUE_V1-queue-top";
    const std::array<std::string, 2> method_urls = {"rated-ladders/", "/top"};
    std::shared_ptr<query> new_request = this->request(method_key, method_urls, routing, queue);
    return (*this->_query)(new_request);
}

Json::Value TFT_LEAGUE_V1::by_tier_division(const std::string& routing, const std::string& tier, const std::string& division) {
    const std::string method_key = "TFT_LEAGUE_V1-by-tier-division";
    const std::array<std::string, 2> method_urls = {"entries/", "/"};
    std::shared_ptr<query> new_request = this->request(method_key, method_urls, routing,tier, division);
    return (*this->_query)(new_request);
}

Json::Value TFT_MATCH_V1::by_match(const std::string& routing, const std::string& match_id) {
    const std::string method_key = "TFT-MATCH-V1-by-match";
    const std::array<std::string, 1> method_urls = {"matches/"};
    std::shared_ptr<query> new_request = this->request(method_key, method_urls, routing, match_id);
    return (*this->_query)(new_request);
}

Json::Value TFT_STATUS_V1::v1(const std::string& routing) {
    const std::string method_key = "TFT-STATUS-V1-v1";
    const std::array<std::string, 1> method_urls= {"platform-data"};
    std::shared_ptr<query> new_request = this->request(method_key, method_urls, routing);
    return (*this->_query)(new_request);
}

Json::Value TFT_SUMMONER_V1::by_account(const std::string& routing, const std::string& account_id) {
    const std::string method_key = "TFT-SUMMONER-V1-by-account";
    const std::array<std::string, 1> method_urls= {"summoners/by-account/"};
    std::shared_ptr<query> new_request = this->request(method_key, method_urls, routing, account_id);
    return (*this->_query)(new_request);
}

Json::Value TFT_SUMMONER_V1::by_name(const std::string& routing, const std::string& summoner_name) {
    const std::string method_key = "TFT-SUMMONER-V1-by-name";
    const std::array<std::string, 1> method_urls= {"summoners/by-name/"};
    std::shared_ptr<query> new_request = this->request(method_key, method_urls, routing, summoner_name);
    return (*this->_query)(new_request);
}

Json::Value TFT_SUMMONER_V1::by_puuid(const std::string& routing, const std::string& puuid) {
    const std::string method_key = "TFT-SUMMONER-V1-by-puuid";
    const std::array<std::string, 1> method_urls= {"summoners/by-puuid/"};
    std::shared_ptr<query> new_request = this->request(method_key, method_urls, routing, puuid);
    return (*this->_query)(new_request);
}

Json::Value TFT_SUMMONER_V1::by_summoner_id(const std::string& routing, const std::string& summoner_id) {
    const std::string method_key = "TFT-SUMMONER-V1-by-summoner-id";
    const std::array<std::string, 1> method_urls= {"summoners/"};
    std::shared_ptr<query> new_request = this->request(method_key, method_urls, routing, summoner_id);
    return (*this->_query)(new_request);
}

Json::Value VAL_CONTENT_V1::content(const std::string& routing) {
    const std::string method_key = "VAL-CONTENT-V1-content";
    const std::array<std::string, 1> method_urls = { "contents" };
    std::shared_ptr<query> new_request = this->request(method_key, method_urls, routing);
    return (*this->_query)(new_request);
}

Json::Value VAL_MATCH_V1::by_match(const std::string& routing, const std::string& match_id) {
    const std::string method_key = "VAL-MATCH-V1-by-match";
    const std::array<std::string, 1> method_urls= {"matches/"};
    std::shared_ptr<query> new_request = this->request(method_key, method_urls, routing, match_id);
    return (*this->_query)(new_request);
}

Json::Value VAL_MATCH_V1::by_puuid(const std::string& routing, const std::string& puuid) {
    const std::string method_key = "VAL-MATCH-V1-by-puuid";
    const std::array<std::string, 2> method_urls= {"matchlists/by-puuid/", "/"};
    std::shared_ptr<query> new_request = this->request(routing, method_urls, routing, puuid);
    return (*this->_query)(new_request);
}

Json::Value VAL_MATCH_V1::by_queue(const std::string& routing, const std::string& queue) {
    const std::string method_key = "VAL-MATCH-V1-by-queue";
    const std::array<std::string, 2> method_urls= {"recent-matches/by-queue/", "/"};
    std::shared_ptr<query> new_request = this->request(routing, method_urls, routing, queue);
    return (*this->_query)(new_request);
}

Json::Value VAL_STATUS_V1::platform_data(const std::string& routing) {
    const std::string method_key = "VAL-STATUS-V1";
    const std::array<std::string, 1> method_urls= {"platform-data"};
    std::shared_ptr<query> new_request = this->request(method_key, method_urls, routing);
    return (*this->_query)(new_request);
}
}
