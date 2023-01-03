#include <string>
#include <vector>
#include <tuple>
#include <jsoncpp/json/json.h>

using namespace client;



enum class URL_FORM{
    U, // url;
    UP, // url + param;
    UPU, // ...
    UPP,
    UPUP,
    UPPP
}
static const std::unordered_map<const std::string, query_format> ACCOUNT_V1 = {
    "by-puuid":   constexpr make_tuple(URL_FORM::UP, std::vector<std::string>("/riot/account/v1/accounts/by-puuid/")),
    "by-riot-id": constexpr make_tuple(URL_FORM::UPP, std::vector<std::string>("/riot/account/v1/accounts/by-riot-id/")),
    "by-game":    constexpr make_tuple(URL_FORM::UPUP, std::vector<std::string>("/riot/account/v1/accounts/by-game/", "/by-puuid/")),
}

static const std::unordered_map<const std::string, query_format> CHAMPION_MASTERY_V1 = {
    "by-summoner":     constexpr make_tuple(URL_FORM::UP, std::vector<std::string>("/lol/champion-mastery/v4/champion-masteries/by-summoner/")),
    "by-champion":     constexpr make_tuple(URL_FORM::UPUP, std::vector<std::string>("/lol/champion-mastery/v4/champion-masteries/by-summoner/", "/by-champion/")),
    "summoner-top":    constexpr make_tuple(URL_FORM::UPU, std::vector<std::string>("/lol/champion-mastery/v4/champion-masteries/by-summoner/", "/top")),
    "summoner-scores": constexpr make_tuple(URL_FORM::UP, std::vector<std::string>("/lol/champion-mastery/v4/scores/by-summoner/")),
}

static const std::unordered_map<const std::string, query_format> CHAMPION_V3 = {
    "rotations": constexpr make_tuple(URL_FORM::U, std::vector<std::string>("/lol/platform/v3/champion-rotations")),
}

static const std::unordered_map<const std::string, query_format> CLASH_V1 = {
    "by-summoner":         constexpr make_tuple(URL_FORM::UP, std::vector<std::string>("/lol/clash/v1/by-summoner/")),
    "by-team":            constexpr make_tuple(URL_FORM::UP, std::vector<std::string>("/lol/clash/v1/teams/")),
    "tournament-by-team": constexpr make_tuple(URL_FORM::UP, std::vector<std::string>("/lol/clash/v1/tournaments/by-team/")),
    "by-tournament":      constexpr make_tuple(URL_FORM::UP, std::vector<std::string>("/lol/clash/v1/tournaments/")),
}

static const std::unordered_map<const std::string, query_format> LEAGUE_EXP_V4 = {
    "entries": constexpr make_tuple(URL_FORM::UPPP, std::vector<std::string>("/lol/league-exp/v4/entries/"))
}

static const std::unordered_map<const std::string, query_format> LEAGUE_V4 = {
    "challenger" :     constexpr make_tuple(URL_FORM::UP, std::vector<std::string>("/lol/league/v4/challengerleagues/by-queue/")),
    "grandmaster" :    constexpr make_tuple(URL_FORM::UP, std::vector<std::string>("/lol/league/v4/grandmaster/by-queue/")),
    "master" :         constexpr make_tuple(URL_FORM::UP, std::vector<std::string>("/lol/league/v4/master/by-queue/")),
    "by-summoner":     constexpr make_tuple(URL_FORM::UP, std::vector<std::string>("/lol/league/v4/entries/by-summoner/")),
    "specific-league": constexpr make_tuple(URL_FORM::UPPP, std::vector<std::string>("/lol/league/v4/entries/by-summoner/")),
    "by-league":       constexpr make_tuple(URL_FORM::UP, std::vector<std::string>("/lol/league/v4/leagues/"))
}

static const std::unordered_map<const std::string, query_format> LOL_CHALLENGES_V1 = {
    "config":               constexpr make_tuple(URL_FORM::U, std::vector<std::string>("/lol/challenges/v1/challenges/config")),
    "percentiles":          constexpr make_tuple(url_form::U, std::vector<std::string>("/lol/challenges/v1/challenges/percentiles")),
    "chalenge-config":      constexpr make_tuple(url_form::UPU, std::vector<std::string>("/lol/challenges/v1/challenges/", "/config")), 
    "chalenge-leaderboard": constexpr make_tuple(url_form::UPUP, std::vector<std::string>("/lol/challenges/v1/challenges/", "/leaderboards/by-level/")), 
    "chalenge-percentiles": constexpr make_tuple(url_form::UPU, std::vector<std::string>("/lol/challenges/v1/challenges/", "/percentiles")), 
    "by-puuid":             constexpr make_tuple(url_form::UP, std::vector<std::string>("/lol/challenges/v1/player-data/")) 
}

static const std::unordered_map<const std::string, query_format> LOL_STATUS = {
    "v3": constexpr make_tuple(URL_FORM::U, std::vector<std::string>("/lol/status/v3/shard-data")),
    "v4": constexpr make_tuple(URL_FORM::U, std::vector<std::string>("/lol/status/v4/platform-data"))
}

static const std::unordered_map<const std::string, query_format> LOR_MATCH_V1 = {
    "by-puuid": constexpr make_tuple(URL_FORM::UPU, std::vector<std::string>("/lor/match/v1/matches/by-puuid/", "/ids")), 
    "by-match": constexpr make_tuple(URL_FORM::UP, std::vector<std::string>("/lor/match/v1/matches/"))
}

static const std::unordered_map<const std::string, query_format> LOR_RANKED_V1 = {
    "leaderboards": constexpr make_tuple(URL_FORM::U, std::vector<std::string>("/lor/ranked/v1/leaderboards"))
}

static const std::unordered_map<const std::string, query_format> LOR_STATUS_V1 = {
    "v1": constexpr make_tuple(URL_FORM::U, std::vector<std::string>("/lor/status/v1/platform-data"))
}

static const std::unordered_map<const std::string, query_format> MATCH_V5 = {
    "by-puuid": constexpr make_tuple(URL_FORM::UPU, std::vector<std::string>("/lol/match/v5/matches/by-puuid/", "/ids")),
    "by-match": constexpr make_tuple(URL_FORM::UP, std::vector<std::string>("/lol/match/v5/matches/")),
    "match-timeline": constexpr make_tuple(URL_FORM::UPU, std::vector<std::string>("/lol/match/v5/matches/", "/timeline"))
}

static const std::unordered_map<const std::string, query_format> SUMMONER_V4 = {
    "by-rso-puuid": constexpr make_tuple(URL_FORM::UP, std::vector<std::string>("/fulfillment/v1/summoners/by-puuid/")),
    "by-account": constexpr make_tuple(URL_FORM::UP, std::vector<std::string>("/lol/summoner/v4/summoners/by-account/")),
    "by-name": constexpr make_tuple(URL_FORM::UP, std::vector<std::string>("/lol/summoner/v4/summoners/by-name/")),
    "by-puuid": constexpr make_tuple(URL_FORM::UP, std::vector<std::string>("/lol/summoner/v4/summoners/by-puuid/")),
    "by-summoner-id": constexpr make_tuple(URL_FORM::UP, std::vector<std::string>("/lol/summoner/v4/summoners/"))
}

static const std::unordered_map<const std::string, query_format> SPECTATOR_V4 = {
    "by-summoner-id": constexpr make_tuple(URL_FORM::UP, std::vector<std::string>("/lol/spectator/v4/active-games/by-summoner/")),
    "featured-games": constexpr make_tuple(URL_FORM::U, std::vector<std::string>("/lol/spectator/v4/featured-games"))
}

static const std::unordered_map<const std::string, query_format> TFT_LEAGUE_V1 = {
    "challenger": constexpr make_tuple(URL_FORM::U, std::vector<std::string>("/tft/league/v1/challenger")),
    "grandmaster": constexpr make_tuple(URL_FORM::U, std::vector<std::string>("/tft/league/v1/grandmaster")),
    "master": constexpr make_tuple(URL_FORM::U, std::vector<std::string>("/tft/league/v1/master")),
    "by-summoner-id": constexpr make_tuple(URL_FORM::UP, std::vector<std::string>("/tft/league/v1/entries/by-summoner/")),
    "by-league-id": constexpr make_tuple(URL_FORM::UP, std::vector<std::string>("/tft/league/v1/leagues/")),
    "queue-top"; constexpr make_tuple(URL_FORM::UPU, std::vector<std::string>("/tft/league/v1/rated-ladders/", "/top")),
    "by-tier-division": constexpr make_tuple(URL_FORM::UPP, std::vector<std::string>("/tft/league/v1/entries/"))
}

static const std::unordered_map<const std::string, query_format> TFT_MATCH_V1 = {
    "by-puuid": constexpr make_tuple(URL_FORM::UPU, std::vector<std::string>("/tft/match/v1/matches/by-puuid/", "/ids")),
    "by-match": constexpr make_tuple(URL_FORM::UP, std::vector<std::string>("/tft/match/v1/matches/"))
}

static const std::unordered_map<const std::string, query_format> TFT_STATUS_V1 = {
    "v1": constexpr make_tuple(URL_FORM::U, std::vector<std::string>("/tft/status/v1/platform-data"))
}

static const std::unordered_map<const std::string, query_format> TFT_SUMMONER_V1 = {
    "by-account": constexpr make_tuple(URL_FORM::UP, std::vector<std::string>("/tft/summoner/v1/summoners/by-account/")),
    "by-name": constexpr make_tuple(URL_FORM::UP, std::vector<std::string>("/tft/summoner/v1/summoners/by-name/")),
    "by-puuid": constexpr make_tuple(URL_FORM::UP, std::vector<std::string>("/tft/summoner/v1/summoners/by-puuid/")),
    "by-summoner-id": constexpr make_tuple(URL_FORM::UP, std::vector<std::string>("/tft/summoner/v1/summoners/")),
}

static const std::unordered_map<const std::string, query_format> VAL_CONTENT_V1 = {
    "content": constexpr make_tuple(URL_FORM::U, std::vector<std::string>("/val/content/v1/contents"))
}

static const std::unordered_map<const std::string, query_format> VAL_MATCH_V1 = {
    "by-match": constexpr make_tuple(URL_FORM::UP, std::vector<std::string>("/val/match/v1/matches/")),
    "by-puuid": constexpr make_tuple(URL_FORM::UP, std::vector<std::string>("/val/match/v1/matchlists/by-puuid/")),
    "by-queue": constexpr make_tuple(URL_FORM::UP, std::vector<std::string>("/val/match/v1/recent-matches/by-queue/"))
}

static const std::unordered_map<const std::string, query_format> VAL_RANKED_V1 = {
    "by-act": constexpr make_tuple(URL_FORM::UP, std::vector<std::string>("/val/ranked/v1/leaderboards/by-act/"))
}

static const std::unordered_map<const std::string, query_format> VAL_STATUS_V1 = {
    "v1": constexpr make_tuple(URL_FORM::U, std::vector<std::string>("/val/status/v1/platform-data"))
}

const std::unordered_map<const std::string, std::unordered_map<std::string, query_format>> RiotApiClient::query_types = {
    "ACCOUNT-V1": ACCOUNT_V1,
    "CHAMPION-MASTERY-V1": CHAMPION_MASTERY_V1,
    "CHAMPION-V3": CHAMPION_V3,
    "CLASH-V1": CLASH_V1,
    "LEAGUE-EXP-V4": LEAGUE_EXP_V4,
    "LEAGUE-V4": LEAGUE_V4,
    "LOL-CHALLENGES-V1": LOL_CHALLENGES_V1,
    "LOL-STATUS": LOL_STATUS,
    "LOR-MATCH-V1": LOR_MATCH_V1,
    "LOR-RANKED-V1": LOR_RANKED_V1,
    "LOR-STATUS-V1": LOR_STATUS_V1,
    "MATCH-V5": MATCH_V5,
    "SPECTATOR-V4": SPECTATOR_V4,
    "SUMMONER-V4": SUMMONER_V4,
    "TFT-LEAGUE-V1": TFT_LEAGUE_V1,
    "TFT-MATCH-V1": TFT_MATCH_V1,
    "TFT-STATUS-V1": TFT_STATUS_V1,
    "TFT-SUMMONER-V1": TFT_SUMMONER_V1,
    "VAL-CONTENT-V1": VAL_CONTENT_V1,
    "VAL-MATCH-V1": VAL_MATCH_V1,
    "VAL-RANKED-V1": VAL_RANKED_V1,
    "VAL-STATUS-V1": VAL_STATUS_V1
}


Json::Value RiotApiClient::query(std::string_view endpoint, std::string_view end_type, int num, ...) {
    va_list query_params;
    va_start(va_list, num);

    query_format url_structure = this->query_types.at(endpoint).at(end_type); 
    URL_FORM form = get<0>url_structure;
    std::vector<std::string> url_components = get<1>url_structure;

    std::string address = "https:\\" + va_arg(va_list, 1) + ".api.riotgames.com" + url_components.at(0);


    if constexpr (form == 0) {
        
    } else if constexpr (form == 1) {
        address += this->encode_url(va_arg(va_list, 0));
    } else if constexpr (form == 2) {
        address += this->encode_url(va_arg(va_list, 0)) + url_components.at(1); 
    } else if constexpr (form == 3) {
        address += this->encode_url(va_arg(va_list, 0)) + this->encode_url(va_arg(va_list, 1));
    } else if constexpr (form == 4) {
        address += this->encode_url(va_arg(va_list, 0)) + url_components.at(1) + this->encode_url(va_arg(va_list, 1));
    } else if constexpr (form == 5) {
        address += this->encode_url(va_arg(va_list, 0)) = this->encode_url(va_arg(va_list, 1)) + this->encode_url(va_arg(va_list, 2))
    } else {

    }

    return this->get(address, *attempts;);
}
