#include <string>
#include <memory>
#include <sstream>
#include <tuple>
#include <numeric>
#include <ctime>
#include <vector>
#include <utility>
#include <stdexcept>
#include "query.h"

namespace query {

static const std::string encode_params(const std::string& to_encode) {
    char* encoded = curl_easy_escape(NULL, to_encode.c_str(), to_encode.length());
    const std::string encoded_str(encoded);
    curl_free(encoded);
    return encoded_str;
}

template <param opt>
static inline void opt_helper(std::stringstream& accumm, const std::pair<std::string, opt>& optional) {
    if (!optional.first.compare("")) {return;};
    accumm << optional.first << "=" << optional.second << "&";
}

template <param ... Opts, std::size_t ... Ixs>
static inline void construct_args(std::stringstream& accum, const std::tuple<std::pair<std::string, Opts>...>& optional, const std::index_sequence<Ixs...>) {
    (opt_helper(accum, std::get<Ixs>(optional)), ...);
}

template <param ... opts>
std::string query_construct(const std::pair<std::string, opts>&... optional_args){
    std::stringstream ss;
    ss << "?";
    constexpr std::size_t n_args = sizeof...(optional_args);
    const std::tuple<std::pair<std::string, opts>...> store(optional_args...);
    construct_args(ss, store, std::make_index_sequence<n_args>{});
    std::string final = ss.str();
    final.pop_back();
    return final;
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

static inline void full_helper(std::stringstream& accum, const std::string& url_seg, const std::string& param) {
    accum << url_seg << encode_params(param);
}

template <std::size_t N, param ... Params, std::size_t ... Ixs>
static inline void full_iter(std::stringstream& accum, const std::array<std::string, N>& method_urls, const std::index_sequence<Ixs ...>, const Params&... params) {
    (full_helper(accum, method_urls[Ixs], params), ...);
}

template <std::size_t N, param Routing, param ... Params>
std::string Endpoint::full_query(const std::array<std::string, N>& method_urls, const Routing& routing, const Params& ... params) const {
    std::stringstream base_url = this->construct_base(routing);
    const std::tuple<Params...> store(params...);
    constexpr std::size_t n_params = sizeof ...(Params);
    full_iter(base_url, method_urls, std::make_index_sequence<n_params>{}, params...);
    if constexpr (n_params < N) {
        base_url << method_urls[N-1]; 
    }     return base_url.str();
}

template <std::size_t N, param Routing, param ... Params>
std::shared_ptr<query> Endpoint::request(const std::string& key, const std::array<std::string, N>& method_urls, const Routing& routing, const Params& ... params) {
    std::string final_url = this->full_query(method_urls, routing, params...);
    std::shared_ptr<query> new_request = std::make_shared<query>(key, routing, final_url);
    new_request->response_content = std::make_unique<json_text>();
    return new_request;
}

std::unique_ptr<json_text> ACCOUNT_V1::by_puuid(const std::string& routing, const std::string& puuid) {
    const std::string method_key = "ACCOUNT-V1-by-puuid";
    const std::array<std::string, 1> method_urls= {"accounts/by-puuid/"};
    std::shared_ptr<query> new_request = this->request(method_key, method_urls, routing, puuid);
    return (*this->_query)(new_request);
}

std::unique_ptr<json_text> ACCOUNT_V1::by_riot_id(const std::string& routing, const std::string& gameName, const std::string& tagline) {
    const std::string method_key = "ACCOUNT-V1-by-riot-id";
    const std::array<std::string, 2> method_urls= {"accounts/by-riot-id/", "/"};
    std::shared_ptr<query> new_request = this->request(method_key, method_urls, routing, gameName, tagline);
    return (*this->_query)(new_request);
}

std::unique_ptr<json_text> ACCOUNT_V1::by_game(const std::string& routing, const std::string& game, const std::string& puuid) {
    const std::string method_key = "ACCOUNT-V1-by-game";
    const std::array<std::string, 2> method_urls= {"active-shards/by-game/", "/by-puuid/"};
    std::shared_ptr<query> new_request = this->request(method_key, method_urls, routing, game, puuid);
    return (*this->_query)(new_request);
}

std::unique_ptr<json_text> CHAMPION_MASTERY_V4::by_summoner_id(const std::string& routing, const std::string& summoner_id) {
    const std::string method_key("CHAMPION-MASTERY-V4-by-summoner-id");
    const std::array<std::string, 1> method_urls{"champion-masteries/by-summoner/"};
    std::shared_ptr<query> new_request = this->request(method_key, method_urls, routing, summoner_id);
    return (*this->_query)(new_request);
}

std::unique_ptr<json_text> CHAMPION_MASTERY_V4::by_summoner_by_champion(const std::string &routing, const std::string &summoner_id, const int champion_id) {
    const std::string method_key("CHAMPION-MASTERY-V4-by-summoner-by-champion");
    const std::array<std::string, 2> method_urls{"champion-masteries/by-summoner/", "/by-champion/"};
    std::shared_ptr<query> new_request = this->request(method_key, method_urls, routing, summoner_id, champion_id);
    return (*this->_query)(new_request);
}
std::unique_ptr<json_text> CHAMPION_MASTERY_V4::by_summoner_top(const std::string& routing, const std::string& summoner_id, const std::pair<std::string, int>& count) {
    const std::string method_key("CHAMPION-MASTERY-V4-by-summoner-top");
    const std::array<std::string, 2> method_urls{"champion-masteries/by-summoner/", "/top"};
    std::shared_ptr<query> new_request = this->request(method_key, method_urls, routing, summoner_id);
    new_request->url += query_construct(count);
    return (*this->_query)(new_request);
        }; 

std::unique_ptr<json_text> CHAMPION_MASTERY_V4::scores_by_summoner(const std::string &routing, const std::string &summoner_id) {
    const std::string method_key("CHAMPION-MASTERY-V4-scores-by-summoner");
    const std::array<std::string, 1> method_urls{"scores/by-summoner/"};
    std::shared_ptr<query> new_request = this->request(method_key, method_urls, routing, summoner_id);
    return (*this->_query)(new_request);
}

std::unique_ptr<json_text> CHAMPION_V3::rotations(const std::string &routing) {
    const std::string method_key("CHAMPION-V3-rotations");
    const std::array<std::string, 1> method_urls{"champion-rotations"};
    std::shared_ptr<query> new_request = this->request(method_key, method_urls, routing);
    return (*this->_query)(new_request);
}

std::unique_ptr<json_text> CLASH_V1::by_summoner_id(const std::string &routing, const std::string &summoner_id) {
    const std::string method_key("CLASH-V1-by-summoner-id");
    const std::array<std::string, 1> method_urls{"by-summoner"};
    std::shared_ptr<query> new_request = this->request(method_key, method_urls, routing, summoner_id);
    return (*this->_query)(new_request);
}

std::unique_ptr<json_text> CLASH_V1::by_team(const std::string &routing, const std::string &team) {
    const std::string method_key("CLASH-V1-by-team");
    const std::array<std::string, 1> method_urls{"teams/"};
    std::shared_ptr<query> new_request = this->request(method_key, method_urls, routing, team);
    return (*this->_query)(new_request);
}

std::unique_ptr<json_text> CLASH_V1::tournament_by_team(const std::string &routing, const std::string &team) {
    const std::string method_key("CLASH-V1-tournament-by-team");
    const std::array<std::string, 1> method_urls{"tournaments/by-team"};
    std::shared_ptr<query> new_request = this->request(method_key, method_urls, routing, team);
    return (*this->_query)(new_request);
}

std::unique_ptr<json_text> CLASH_V1::by_tournament(const std::string &routing, const std::string &tournament) {
    const std::string method_key("CLASH-V1-by-tournament");
    const std::array<std::string, 1> method_urls{"tournaments/"};
    std::shared_ptr<query> new_request = this->request(method_key, method_urls, routing, tournament);
    return (*this->_query)(new_request);
}

std::unique_ptr<json_text> LEAGUE_EXP_V4::entries(const std::string& routing, const std::string& queue, const std::string& tier, const std::string& division, const std::pair<std::string, int>& page) {
    const std::string method_key("LEAGUE-EXP-V4-entries");
    const std::array<std::string, 3> method_urls{"entries/", "/", "/"};
    std::shared_ptr<query> new_request = this->request(method_key, method_urls, routing, queue, tier, division);
    new_request->url += query_construct(page);
    return (*this->_query)(new_request);
};
std::unique_ptr<json_text> LEAGUE_V4::challenger(const std::string& routing, const std::string& queue) {
    const std::string method_key("LEAGUE-V4-challenger");
    const std::array<std::string, 1> method_urls{"challengerleagues/by-queue/"};
    std::shared_ptr<query> new_request = this->request(method_key, method_urls, routing, queue);
    return (*this->_query)(new_request);
}

std::unique_ptr<json_text> LEAGUE_V4::grandmaster(const std::string& routing, const std::string& queue) {
    const std::string method_key("LEAGUE-V4-grandmaster");
    const std::array<std::string, 1> method_urls{"grandmasterleagues/by-queue/"};
    std::shared_ptr<query> new_request = this->request(method_key, method_urls, routing, queue);
    return (*this->_query)(new_request);
}

std::unique_ptr<json_text> LEAGUE_V4::master(const std::string& routing, const std::string& queue) {
    const std::string method_key("LEAGUE-V4-master");
    const std::array<std::string, 1> method_urls{"masterleagues/by-queue/"};
    std::shared_ptr<query> new_request = this->request(method_key, method_urls, routing, queue);
    return (*this->_query)(new_request);
}

std::unique_ptr<json_text> LEAGUE_V4::by_summoner_id(const std::string& routing, const std::string& summoner_id) {
    const std::string method_key("LEAGUE-V4-by-summoner-id");
    const std::array<std::string, 1> method_urls{"entries/by-summoner/"};
    std::shared_ptr<query> new_request = this->request(method_key, method_urls, routing, summoner_id);
    return (*this->_query)(new_request);
}

std::unique_ptr<json_text> LEAGUE_V4::by_league_id(const std::string& routing, const std::string& league_id) {
    const std::string method_key("LEAGUE-V4-by-league-id");
    const std::array<std::string, 1> method_urls{"leagues/"};
    std::shared_ptr<query> new_request = this->request(method_key, method_urls, routing, league_id);
    return (*this->_query)(new_request);
}

std::unique_ptr<json_text> LEAGUE_V4::specific_league(const_str_r routing, const_str_r queue, const_str_r tier, const_str_r division, const std::pair<std::string, int>& page) {
    const std::string method_key("LEAGUE-V4-specific-league");
    const std::array<std::string, 3> method_urls{"entries/", "/", "/"};
    std::shared_ptr<query> new_request = this->request(method_key, method_urls, routing, queue, tier, division);
    new_request->url += query_construct(page);
    return (*this->_query)(new_request);
}

std::unique_ptr<json_text> LOL_CHALLENGES_V1::config(const std::string &routing) {
    const std::string method_key("LOL-CHALLENGES-V1-config");
    const std::array<std::string, 1> method_urls{"challenges/config"};
    std::shared_ptr<query> new_request = this->request(method_key, method_urls, routing);
    return (*this->_query)(new_request);
}
std::unique_ptr<json_text> LOL_CHALLENGES_V1::percentiles(const std::string &routing) {
    const std::string method_key("LOL-CHALLENGES-V1-percentiles");
    const std::array<std::string, 1> method_urls{"challenges/percentiles"};
    std::shared_ptr<query> new_request = this->request(method_key, method_urls, routing);
    return (*this->_query)(new_request);
}
std::unique_ptr<json_text> LOL_CHALLENGES_V1::challenge_config(const std::string& routing, const int challenge_id) {
    const std::string method_key("LOL-CHALLENGES-V1-challenge-config");
    const std::array<std::string, 2> method_urls{"challenges/", "/config"};
    std::shared_ptr<query> new_request = this->request(method_key, method_urls, routing, challenge_id);
    return (*this->_query)(new_request);
}

std::unique_ptr<json_text> LOL_CHALLENGES_V1::challenge_percentiles(const std::string& routing, const int challenge_id) {
    const std::string method_key("LOL-CHALLENGES-V1-challenge-percentiles");
    const std::array<std::string, 2> method_urls{"challenges/", "/percentiles"};
    std::shared_ptr<query> new_request = this->request(method_key, method_urls, routing, challenge_id);
    return (*this->_query)(new_request);
}
std::unique_ptr<json_text> LOL_CHALLENGES_V1::by_puuid(const std::string& routing, const std::string& puuid) {
    const std::string method_key("LOL-CHALLENGES-V1-by-puuid");
    const std::array<std::string, 1> method_urls{"player-data/"};
    std::shared_ptr<query> new_request = this->request(method_key, method_urls, routing, puuid);
    return (*this->_query)(new_request);
}

std::unique_ptr<json_text> LOL_CHALLENGES_V1::challenge_leaderboard(const_str_r routing, const int challenge_id, const_str_r level, const std::pair<std::string, int>& limit) {
    const std::string method_key("LOL-CHALLENGES-V1-challenge-leaderboard");
    const std::array<std::string, 2> method_urls{"challenges/", "/leaderboards/by-level/"};
    std::shared_ptr<query> new_request = this->request(method_key, method_urls, routing, challenge_id, level);
    new_request->url += query_construct(limit);
    return (*this->_query)(new_request);
};

std::unique_ptr<json_text> LOL_STATUS::v3(const std::string& routing) {
    const std::string method_key("LOL-STATUS-v3");
    const std::array<std::string, 1> method_urls{"v3/shard-data"};
    std::shared_ptr<query> new_request = this->request(method_key, method_urls, routing);
    return (*this->_query)(new_request);
}

std::unique_ptr<json_text> LOL_STATUS::v4(const std::string& routing) {
    const std::string method_key("LOL-STATUS-v4");
    const std::array<std::string, 1> method_urls{"v4/platform-data"};
    std::shared_ptr<query> new_request = this->request(method_key, method_urls, routing);
    return (*this->_query)(new_request);
}

std::unique_ptr<json_text> LOR_MATCH_V1::by_puuid(const std::string& routing, const std::string& puuid) {
    const std::string method_key("LOR-MATCH-V1-by-puuid");
    const std::array<std::string, 2> method_urls{"matches/by-puuid/", "/ids"};
    std::shared_ptr<query> new_request = this->request(method_key, method_urls, routing, puuid);
    return (*this->_query)(new_request);
}

std::unique_ptr<json_text> LOR_MATCH_V1::by_match(const std::string& routing, const std::string& match) {
    const std::string method_key("LOR-MATCH-V1-by-match");
    const std::array<std::string, 1> method_urls{"matches/"};
    std::shared_ptr<query> new_request = this->request(method_key, method_urls, routing, match);
    return (*this->_query)(new_request);
}

std::unique_ptr<json_text> LOR_RANKED_V1::leaderboards(const std::string& routing) {
    const std::string method_key("LOR-RANKED-V1-leaderboards");
    const std::array<std::string, 1> method_urls{"leaderboards"};
    std::shared_ptr<query> new_request = this->request(method_key, method_urls, routing);
    return (*this->_query)(new_request);
}

std::unique_ptr<json_text> LOR_STATUS_V1::v1(const std::string& routing) {
    const std::string method_key("LOR-STATUS-V1-v1");
    const std::array<std::string, 1> method_urls{"platform-data"};
    std::shared_ptr<query> new_request = this->request(method_key, method_urls, routing);
    return (*this->_query)(new_request);
}

std::unique_ptr<json_text> MATCH_V5::by_match_id(const std::string& routing, const std::string& match_id) {
    const std::string method_key = "MATCH-V5-by-match-id";
    const std::array<std::string, 1> method_urls = {"matches/"};
    std::shared_ptr<query> new_request = this->request(method_key, method_urls, routing, match_id);
    return (*this->_query)(new_request);
}

std::unique_ptr<json_text> MATCH_V5::timeline(const std::string& routing, const std::string& match_id) {
    const std::string method_key = "MATCH-V5-timeline";
    const std::array<std::string, 2> method_urls = {"matches/", "/timeline"};
    std::shared_ptr<query> new_request = this->request(method_key, method_urls, routing, match_id);
    return (*this->_query)(new_request);
}

std::unique_ptr<json_text> MATCH_V5::by_puuid(const std::string& routing, const std::string& puuid, const std::pair<std::string, long>& startTime, const std::pair<std::string, long>& endTime, const std::pair<std::string, int>& queue, const std::pair<std::string, int>& start, const std::pair<std::string, int>& count) {
    const std::string method_key = "MATCH-V5-by-puuid";
    const std::array<std::string, 2> method_urls= {"matches/by-puuid/", "/ids"};
    std::shared_ptr<query> new_request = this->request(method_key, method_urls, routing, puuid);
    new_request->url += query_construct(startTime, endTime, queue, start, count);
    return (*this->_query)(new_request);
}

std::unique_ptr<json_text> MATCH_V5::by_puuid(const std::string& routing, const std::string& puuid, const std::pair<std::string, std::string>& types, const std::pair<std::string, long>& startTime, const std::pair<std::string, long>& endTime, const std::pair<std::string, int>& queue, const std::pair<std::string, int>& start, const std::pair<std::string, int>& count) {
    const std::string method_key = "MATCH-V5-by-puuid";
    const std::array<std::string, 2> method_urls= {"matches/by-puuid/", "/ids"};
    std::shared_ptr<query> new_request = this->request(method_key, method_urls, routing, puuid);
    new_request->url += query_construct(startTime, endTime, queue, types, start, count);
    return (*this->_query)(new_request);
};

std::unique_ptr<json_text> SUMMONER_V4::by_rso_puuid(const std::string& routing, const std::string& puuid) {
    const std::string method_key = "SUMMONER-V4-by-rso-puuid";
    const std::array<std::string, 1> method_urls = {"fulfillment/v1/summoners/by-puuid/"};
    std::shared_ptr<query> new_request = this->request(method_key, method_urls, routing, puuid);
    return (*this->_query)(new_request);
}

std::unique_ptr<json_text> SUMMONER_V4::by_account_id(const std::string& routing, const std::string& account_id) {
    const std::string method_key = "SUMMONER-V4-by-account-id";
    const std::array<std::string, 1> method_urls = {"summoners/by-account/"};
    std::shared_ptr<query> new_request = this->request(method_key, method_urls, routing, account_id);
    return (*this->_query)(new_request);
}

std::unique_ptr<json_text> SUMMONER_V4::by_name(const std::string& routing, const std::string& name) {
    const std::string method_key = "SUMMONER-V4-by-name";
    const std::array<std::string, 1> method_urls = {"summoners/by-name/"};
    std::shared_ptr<query> new_request = this->request(method_key, method_urls, routing, name);
    return (*this->_query)(new_request);
}

std::unique_ptr<json_text> SUMMONER_V4::by_puuid(const std::string& routing, const std::string& puuid) {
    const std::string method_key = "SUMMONER-V4-by-puuid";
    const std::array<std::string, 1> method_urls = {"summoners/by-puuid/"};
    std::shared_ptr<query> new_request = this->request(method_key, method_urls, routing, puuid);
    return (*this->_query)(new_request);
}

std::unique_ptr<json_text> SUMMONER_V4::by_summoner_id(const std::string& routing, const std::string& summoner_id) {
    const std::string method_key = "SUMMONER-V4-by-summoner-id";
    const std::array<std::string, 1> method_urls = {"summoners/"};
    std::shared_ptr<query> new_request = this->request(method_key, method_urls, routing, summoner_id);
    return (*this->_query)(new_request);
}

std::unique_ptr<json_text> SPECTATOR_V4::by_summoner_id(const std::string& routing, const std::string& summonerId) {
    const std::string method_key = "SPECTATOR-V4-by-summoner-id";
    const std::array<std::string, 1> method_urls= {"active-games/by-summoner/"};
    std::shared_ptr<query> new_request = this->request(method_key, method_urls, routing, summonerId);
    return (*this->_query)(new_request);
}

std::unique_ptr<json_text> SPECTATOR_V4::featured_games(const std::string& routing) {
    const std::string method_key = "SPECTATOR-V4-featured-games";
    const std::array<std::string, 1> method_urls= {"featured-games"};
    std::shared_ptr<query> new_request = this->request(method_key, method_urls, routing);
    return (*this->_query)(new_request);
}

std::unique_ptr<json_text> TFT_LEAGUE_V1::challenger(const std::string& routing) {
    const std::string method_key = "TFT-LEAGUE-V1-challenger";
    const std::array<std::string, 1> method_urls = { "challenger" };
    std::shared_ptr<query> new_request = this->request(method_key, method_urls, routing);
    return (*this->_query)(new_request);
}

std::unique_ptr<json_text> TFT_LEAGUE_V1::grandmaster(const std::string& routing) {
    const std::string method_key = "TFT-LEAGUE-V1-grandmaster";
    const std::array<std::string, 1> method_urls = { "grandmaster" };
    std::shared_ptr<query> new_request = this->request(method_key, method_urls, routing);
    return (*this->_query)(new_request);
}

std::unique_ptr<json_text> TFT_LEAGUE_V1::master(const std::string& routing) {
    const std::string method_key = "TFT-LEAGUE-V1-master";
    const std::array<std::string, 1> method_urls = { "master" };
    std::shared_ptr<query> new_request = this->request(method_key, method_urls, routing);
    return (*this->_query)(new_request);
}

std::unique_ptr<json_text> TFT_LEAGUE_V1::by_summoner_id(const std::string& routing, const std::string& summoner_id) {
    const std::string method_key = "TFT-LEAGUE-V1-by-summoner-id";
    const std::array<std::string, 2> method_urls = { "entries/by-summoner/", "" };
    std::shared_ptr<query> new_request = this->request(method_key, method_urls, routing, summoner_id);
    return (*this->_query)(new_request);
}

std::unique_ptr<json_text> TFT_LEAGUE_V1::by_league_id(const std::string& routing, const std::string& league_id) {
    const std::string method_key = "TFT-LEAGUE-V1-by-league-id";
    const std::array<std::string, 2> method_urls = { "leagues/", "" };
    std::shared_ptr<query> new_request = this->request(method_key, method_urls, routing, league_id);
    return (*this->_query)(new_request);
}

std::unique_ptr<json_text> TFT_LEAGUE_V1::queue_top(const std::string& routing, const std::string& queue) {
    const std::string method_key = "TFT-LEAGUE-V1-queue-top";
    const std::array<std::string, 2> method_urls = {"rated-ladders/", "/top"};
    std::shared_ptr<query> new_request = this->request(method_key, method_urls, routing, queue);
    return (*this->_query)(new_request);
}

std::unique_ptr<json_text> TFT_LEAGUE_V1::by_tier_division(const std::string& routing, const std::string& tier, const std::string& division, const std::pair<std::string, int>& count) {
    const std::string method_key = "TFT_LEAGUE_V1-by-tier-division";
    const std::array<std::string, 2> method_urls = {"entries/", "/"};
    std::shared_ptr<query> new_request = this->request(method_key, method_urls, routing,tier, division);
    new_request->url += query_construct(count);
    return (*this->_query)(new_request);
}
std::unique_ptr<json_text> TFT_MATCH_V1::by_puuid(const std::string& routing, const std::string& puuid, const std::pair<std::string, int>& start, const std::pair<std::string, long>& endTime, const std::pair<std::string, long>& startTime, const std::pair<std::string, int>& count) {
    const std::string method_key = "TFT-MATCH-V1-by-puuid";
    const std::array<std::string, 2> method_urls = {"matches/by-puuid/", "/ids"};
    std::shared_ptr<query> new_request = this->request(method_key, method_urls, routing, puuid);
    new_request->url += query_construct(start, endTime, startTime, count);
    return (*this->_query)(new_request);
};

std::unique_ptr<json_text> TFT_MATCH_V1::by_match(const std::string& routing, const std::string& match_id) {
    const std::string method_key = "TFT-MATCH-V1-by-match";
    const std::array<std::string, 1> method_urls = {"matches/"};
    std::shared_ptr<query> new_request = this->request(method_key, method_urls, routing, match_id);
    return (*this->_query)(new_request);
}

std::unique_ptr<json_text> TFT_STATUS_V1::v1(const std::string& routing) {
    const std::string method_key = "TFT-STATUS-V1-v1";
    const std::array<std::string, 1> method_urls= {"platform-data"};
    std::shared_ptr<query> new_request = this->request(method_key, method_urls, routing);
    return (*this->_query)(new_request);
}

std::unique_ptr<json_text> TFT_SUMMONER_V1::by_account(const std::string& routing, const std::string& account_id) {
    const std::string method_key = "TFT-SUMMONER-V1-by-account";
    const std::array<std::string, 1> method_urls= {"summoners/by-account/"};
    std::shared_ptr<query> new_request = this->request(method_key, method_urls, routing, account_id);
    return (*this->_query)(new_request);
}

std::unique_ptr<json_text> TFT_SUMMONER_V1::by_name(const std::string& routing, const std::string& summoner_name) {
    const std::string method_key = "TFT-SUMMONER-V1-by-name";
    const std::array<std::string, 1> method_urls= {"summoners/by-name/"};
    std::shared_ptr<query> new_request = this->request(method_key, method_urls, routing, summoner_name);
    return (*this->_query)(new_request);
}

std::unique_ptr<json_text> TFT_SUMMONER_V1::by_puuid(const std::string& routing, const std::string& puuid) {
    const std::string method_key = "TFT-SUMMONER-V1-by-puuid";
    const std::array<std::string, 1> method_urls= {"summoners/by-puuid/"};
    std::shared_ptr<query> new_request = this->request(method_key, method_urls, routing, puuid);
    return (*this->_query)(new_request);
}

std::unique_ptr<json_text> TFT_SUMMONER_V1::by_summoner_id(const std::string& routing, const std::string& summoner_id) {
    const std::string method_key = "TFT-SUMMONER-V1-by-summoner-id";
    const std::array<std::string, 1> method_urls= {"summoners/"};
    std::shared_ptr<query> new_request = this->request(method_key, method_urls, routing, summoner_id);
    return (*this->_query)(new_request);
}

std::unique_ptr<json_text> VAL_CONTENT_V1::content(const std::string& routing, const std::pair<std::string, std::string>& locale) {
    const std::string method_key = "VAL-CONTENT-V1-content";
    const std::array<std::string, 1> method_urls = { "contents" };
    std::shared_ptr<query> new_request = this->request(method_key, method_urls, routing);
    new_request->url += query_construct(locale);
    return (*this->_query)(new_request);
}

std::unique_ptr<json_text> VAL_MATCH_V1::by_match(const std::string& routing, const std::string& match_id) {
    const std::string method_key = "VAL-MATCH-V1-by-match";
    const std::array<std::string, 1> method_urls= {"matches/"};
    std::shared_ptr<query> new_request = this->request(method_key, method_urls, routing, match_id);
    return (*this->_query)(new_request);
}

std::unique_ptr<json_text> VAL_MATCH_V1::by_puuid(const std::string& routing, const std::string& puuid) {
    const std::string method_key = "VAL-MATCH-V1-by-puuid";
    const std::array<std::string, 2> method_urls= {"matchlists/by-puuid/", "/"};
    std::shared_ptr<query> new_request = this->request(method_key, method_urls, routing, puuid);
    return (*this->_query)(new_request);
}

std::unique_ptr<json_text> VAL_MATCH_V1::by_queue(const std::string& routing, const std::string& queue) {
    const std::string method_key = "VAL-MATCH-V1-by-queue";
    const std::array<std::string, 2> method_urls= {"recent-matches/by-queue/", "/"};
    std::shared_ptr<query> new_request = this->request(method_key, method_urls, routing, queue);
    return (*this->_query)(new_request);
}
std::unique_ptr<json_text> VAL_RANKED_V1::by_act(const_str_r routing, const_str_r actId, const std::pair<std::string, std::string>& locale) {
    const std::string method_key = "VAL_RANKED_V1-by-act";
    const std::array<std::string, 1> method_urls= {"leaderboards/by-act/"};
    std::shared_ptr<query> new_request = this->request(method_key, method_urls, routing, actId);
    new_request->url += query_construct(locale);
    return (*this->_query)(new_request);
};

std::unique_ptr<json_text> VAL_STATUS_V1::platform_data(const std::string& routing) {
    const std::string method_key = "VAL-STATUS-V1";
    const std::array<std::string, 1> method_urls= {"platform-data"};
    std::shared_ptr<query> new_request = this->request(method_key, method_urls, routing);
    return (*this->_query)(new_request);
}
}
