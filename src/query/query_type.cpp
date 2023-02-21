#include <string>
#include <tuple>
#include <numeric>
#include <ctime>
#include <vector>
#include <utility>
#include <stdexcept>
#include "query.h"

namespace query {

template <param P>
static inline std::string construct_args(const std::stringstream& accumm, const std::pair<P, P>& optional_arg) {
    return accumm << optional_arg.first << "=" << optional_arg.second << "&";
}

template <param ... opts>
std::string query_construct(const std::tuple<std::pair<std::string, opts> ...>& optional_args){
    std::stringstream ss;
    ss = std::accumulate(optional_args.begin(), --optional_args.end(), std::stringstream("?"), construct_args);
    const std::size_t n_args = std::tuple_size<decltype(optional_args)>::value;
    ss << std::get<n_args-1>(optional_args).first << "=" << std::get<n_args-1>(optional_args).second;
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
static inline void full_iter(std::stringstream& accum, const std::array<std::string, N>& method_urls, const std::tuple<Params ...>& params, std::index_sequence<Ixs ...>) {
    (full_helper(accum, method_urls[Ixs], std::get<Ixs+1>(params)), ...);
}

template <std::size_t N, param ... Params>
std::string Endpoint::full_query(const std::array<std::string, N>& method_urls, const std::tuple<Params ...>& store) const {
    std::stringstream base_url = this->construct_base(std::get<0>(store));
    const std::size_t n_params = std::tuple_size<std::tuple<Params...>>::value - 1;
    full_iter(base_url, method_urls, store, std::make_index_sequence<N-1>{});
    if constexpr (n_params < N) {
        base_url << method_urls[N-1];
    } 
    return base_url.str();
}

template <std::size_t N, param ... Params, param ... opts>
std::string Endpoint::full_query(const std::array<std::string, N>& method_urls, const std::tuple<Params ...>& params, const std::tuple<std::pair<std::string, opts>...>& optional_args) const {
    std::string final_url = this->full_query(method_urls, params);
    std::string optional = query_construct(optional_args);
    final_url += optional;
    return final_url;
}
template <param R>
std::shared_ptr<query> Endpoint::request(const std::string& key, const std::string& method_url, const R& routing) {
    std::string final_url = this->construct_base(routing).str();
    std::shared_ptr<query> request = std::make_shared<query>(key, routing, final_url);
    return {};
}

template <std::size_t N, param ... Params>
std::shared_ptr<query> Endpoint::request(const std::string& key, const std::array<std::string, N>& method_urls, const Params& ... params) {
    const std::tuple<Params ...> store(params ...);
    std::string final_url = this->full_query(method_urls, store);
    std::shared_ptr<query> request = std::make_shared<query>(key, std::get<0>(store), final_url);
    return request;
}

template <std::size_t N, param ... Params, param ... opts>
std::shared_ptr<query> Endpoint::request(const std::string& key, const std::array<std::string, N>& method_urls, const Params& ... params, const std::pair<std::string, opts>& ... optional_args) {
    std::tuple<Params ...> store_params(params ...);
    std::tuple<std::pair<std::string, opts> ...> store_opts(optional_args...);
    std::string final_url = this->full_query(method_urls, store_params, store_opts);
    std::shared_ptr<query> request = std::make_shared<query>(key, std::get<0>(store_params), final_url);
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

Json::Value CHAMPION_MASTERY_V4::by_summoner_top(const std::string &routing, const std::string &summoner_id) {
    const std::string method_key("CHAMPION-MASTERY-V4-by-summoner-top");
    const std::array<std::string, 2> method_urls{"champion-masteries/by-summoner/", "/top"};
    std::shared_ptr<query> new_request = this->request(method_key, method_urls, routing, summoner_id);
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

Json::Value LEAGUE_EXP_V4::entries(const std::string &routing, const std::string &queue, const std::string &tier, const std::string &division) {
    const std::string method_key("LEAGUE-EXP-V4-entries");
    const std::array<std::string, 3> method_urls{"entries/", "/", "/"};
    std::shared_ptr<query> new_request = this->request(method_key, method_urls, routing, queue, tier, division);
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

Json::Value LEAGUE_V4::specific_league(const std::string &routing, const std::string &queue, const std::string &tier, const std::string &division) {
    const std::string method_key("LEAGUE-V4-specific-league");
    const std::array<std::string, 3> method_urls{"entries/", "/", "/"};
    std::shared_ptr<query> new_request = this->request(method_key, method_urls, routing, queue, tier, division);
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

Json::Value LOL_CHALLENGES_V1::challenge_leaderboard(const std::string &routing, const std::string& challenge_id, const std::string& level) {
    const std::string method_key("LOL-CHALLENGES-V1-challenge-leaderboard")
    const std::array<std::string, 2> method_urls{"challenges/", "/leaderboards/by-level/"};
    std::shared_ptr<query> new_request = this->request(method_key, method_urls, routing, challenge_id, level);
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

//    {"by-puuid", std::make_shared<query::UPU>("/lol/match/v5/matches/by-puuid/", "/ids", true)},
//    {"by-match-id", std::make_shared<query::UP>("/lol/match/v5/matches/", false)},
//    {"match-timeline", std::make_shared<query::UPU>("/lol/match/v5/matches/", "/timeline", false)}
}
