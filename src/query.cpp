#include "client.h"
#include "query_type.h"

#define STR (std::string)
using namespace client;

const std::unordered_map<std::string, std::shared_ptr<query::QueryType>> ACCOUNT_V1 = {
    {"by-puuid",   std::make_shared<query::UP>(std::string("/riot/account/v1/accounts/by-puuid/"))},
    {"by-riot-id", std::make_shared<query::UPP>(std::string("/riot/account/v1/accounts/by-riot-id/"))},
    {"by-game",    std::make_shared<query::UPUP>(std::string("/riot/account/v1/accounts/by-game/"), std::string("/by-puuid/"))},
};

const std::unordered_map<std::string, std::shared_ptr<query::QueryType>> CHAMPION_MASTERY_V1 = {
    {"by-summoner",     std::make_shared<query::UP>(std::string("/lol/champion-mastery/v4/champion-masteries/by-summoner/"))},
    {"by-champion",     std::make_shared<query::UPUP>(std::string("/lol/champion-mastery/v4/champion-masteries/by-summoner/"), std::string("/by-champion/"))},
    {"summoner-top",    std::make_shared<query::UPU>(std::string("/lol/champion-mastery/v4/champion-masteries/by-summoner/"), std::string("/top"))},
    {"summoner-scores", std::make_shared<query::UP>(std::string("/lol/champion-mastery/v4/scores/by-summoner/"))},
};

const std::unordered_map<std::string, std::shared_ptr<query::QueryType>> CHAMPION_V3 = {
    {"rotations", std::make_shared<query::U>(std::string("/lol/platform/v3/champion-rotations"))}
};

const std::unordered_map<std::string, std::shared_ptr<query::QueryType>> CLASH_V1 = {
    {"by-summoner",         std::make_shared<query::UP>(std::string("/lol/clash/v1/by-summoner/"))},
    {"by-team",            std::make_shared<query::UP>(std::string("/lol/clash/v1/teams/"))},
    {"tournament-by-team", std::make_shared<query::UP>(std::string("/lol/clash/v1/tournaments/by-team/"))},
    {"by-tournament",      std::make_shared<query::UP>(std::string("/lol/clash/v1/tournaments/"))},
};

const std::unordered_map<std::string, std::shared_ptr<query::QueryType>> LEAGUE_EXP_V4 = {
    {"entries", std::make_shared<query::UPPP>(std::string("/lol/league-exp/v4/entries/"))}
};

const std::unordered_map<std::string, std::shared_ptr<query::QueryType>> LEAGUE_V4 = {
    {"challenger" ,     std::make_shared<query::UP>(std::string("/lol/league/v4/challengerleagues/by-queue/"))},
    {"grandmaster" ,    std::make_shared<query::UP>(std::string("/lol/league/v4/grandmaster/by-queue/"))},
    {"master" ,         std::make_shared<query::UP>(std::string("/lol/league/v4/master/by-queue/"))},
    {"by-summoner",     std::make_shared<query::UP>(std::string("/lol/league/v4/entries/by-summoner/"))},
    {"specific-league", std::make_shared<query::UPPP>(std::string("/lol/league/v4/entries/by-summoner/"))},
    {"by-league",       std::make_shared<query::UP>(std::string("/lol/league/v4/leagues/"))}
};

const std::unordered_map<std::string, std::shared_ptr<query::QueryType>> LOL_CHALLENGES_V1 = {
    {"config",               std::make_shared<query::U>(std::string("/lol/challenges/v1/challenges/config"))},
    {"percentiles",          std::make_shared<query::U>(std::string("/lol/challenges/v1/challenges/percentiles"))},
    {"chalenge-config",      std::make_shared<query::UPU>(std::string("/lol/challenges/v1/challenges/"), std::string("/config"))}, 
    {"challenge-leaderboard", std::make_shared<query::UPUP>(std::string("/lol/challenges/v1/challenges/"), std::string("/leaderboards/by-level/"))},
    {"chalenge-percentiles", std::make_shared<query::UPU>(std::string("/lol/challenges/v1/challenges/"), std::string("/percentiles"))}, 
    {"by-puuid",             std::make_shared<query::UP>(std::string("/lol/challenges/v1/player-data/"))} 
};

const std::unordered_map<std::string, std::shared_ptr<query::QueryType>> LOL_STATUS = {
    {"v3", std::make_shared<query::U>(std::string("/lol/status/v3/shard-data"))},
    {"v4", std::make_shared<query::U>(std::string("/lol/status/v4/platform-data"))}
};

const std::unordered_map<std::string, std::shared_ptr<query::QueryType>> LOR_MATCH_V1 = {
    {"by-puuid", std::make_shared<query::UPU>(std::string("/lor/match/v1/matches/by-puuid/"), std::string("/ids"))}, 
    {"by-match", std::make_shared<query::UP>(std::string("/lor/match/v1/matches/"))}
};

const std::unordered_map<std::string, std::shared_ptr<query::QueryType>> LOR_RANKED_V1 = {
    {"leaderboards", std::make_shared<query::U>(std::string("/lor/ranked/v1/leaderboards"))}
};

const std::unordered_map<std::string, std::shared_ptr<query::QueryType>> LOR_STATUS_V1 = {
    {"v1", std::make_shared<query::U>(std::string("/lor/status/v1/platform-data"))}
};

const std::unordered_map<std::string, std::shared_ptr<query::QueryType>> MATCH_V5 = {
    {"by-puuid", std::make_shared<query::UPU>(std::string("/lol/match/v5/matches/by-puuid/"), std::string("/ids"))},
    {"by-match", std::make_shared<query::UP>(std::string("/lol/match/v5/matches/"))},
    {"match-timeline", std::make_shared<query::UPU>(std::string("/lol/match/v5/matches/"), std::string("/timeline"))}
};

const std::unordered_map<std::string, std::shared_ptr<query::QueryType>> SUMMONER_V4 = {
    {"by-rso-puuid", std::make_shared<query::UP>(std::string("/fulfillment/v1/summoners/by-puuid/"))},
    {"by-account", std::make_shared<query::UP>(std::string("/lol/summoner/v4/summoners/by-account/"))},
    {"by-name", std::make_shared<query::UP>(std::string("/lol/summoner/v4/summoners/by-name/"))},
    {"by-puuid", std::make_shared<query::UP>(std::string("/lol/summoner/v4/summoners/by-puuid/"))},
    {"by-summoner-id", std::make_shared<query::UP>(std::string("/lol/summoner/v4/summoners/"))}
};

const std::unordered_map<std::string, std::shared_ptr<query::QueryType>> SPECTATOR_V4 = {
    {"by-summoner-id", std::make_shared<query::UP>(std::string("/lol/spectator/v4/active-games/by-summoner/"))},
    {"featured-games", std::make_shared<query::U>(std::string("/lol/spectator/v4/featured-games"))}
};

const std::unordered_map<std::string, std::shared_ptr<query::QueryType>> TFT_LEAGUE_V1 = {
    {"challenger", std::make_shared<query::U>(std::string("/tft/league/v1/challenger"))},
    {"grandmaster", std::make_shared<query::U>(std::string("/tft/league/v1/grandmaster"))},
    {"master", std::make_shared<query::U>(std::string("/tft/league/v1/master"))},
    {"by-summoner-id", std::make_shared<query::UP>(std::string("/tft/league/v1/entries/by-summoner/"))},
    {"by-league-id", std::make_shared<query::UP>(std::string("/tft/league/v1/leagues/"))},
    {"queue-top", std::make_shared<query::UPU>(std::string("/tft/league/v1/rated-ladders/"), std::string("/top"))},
    {"by-tier-division", std::make_shared<query::UPP>(std::string("/tft/league/v1/entries/"))}
};

const std::unordered_map<std::string, std::shared_ptr<query::QueryType>> TFT_MATCH_V1 = {
    {"by-puuid", std::make_shared<query::UPU>(std::string("/tft/match/v1/matches/by-puuid/"), std::string("/ids"))},
    {"by-match", std::make_shared<query::UP>(std::string("/tft/match/v1/matches/"))}
};

const std::unordered_map<std::string, std::shared_ptr<query::QueryType>> TFT_STATUS_V1 = {
    {"v1", std::make_shared<query::U>(std::string("/tft/status/v1/platform-data"))}
};

const std::unordered_map<std::string, std::shared_ptr<query::QueryType>> TFT_SUMMONER_V1 = {
    {"by-account", std::make_shared<query::UP>(std::string("/tft/summoner/v1/summoners/by-account/"))},
    {"by-name", std::make_shared<query::UP>(std::string("/tft/summoner/v1/summoners/by-name/"))},
    {"by-puuid", std::make_shared<query::UP>(std::string("/tft/summoner/v1/summoners/by-puuid/"))},
    {"by-summoner-id", std::make_shared<query::UP>(std::string("/tft/summoner/v1/summoners/"))},
};;

const std::unordered_map<std::string, std::shared_ptr<query::QueryType>> VAL_CONTENT_V1 = {
    {"content", std::make_shared<query::U>(std::string("/val/content/v1/contents"))}
};

const std::unordered_map<std::string, std::shared_ptr<query::QueryType>> VAL_MATCH_V1 = {
    {"by-match", std::make_shared<query::UP>(std::string("/val/match/v1/matches/"))},
    {"by-puuid", std::make_shared<query::UP>(std::string("/val/match/v1/matchlists/by-puuid/"))},
    {"by-queue", std::make_shared<query::UP>(std::string("/val/match/v1/recent-matches/by-queue/"))}
};

const std::unordered_map<std::string, std::shared_ptr<query::QueryType>> VAL_RANKED_V1 = {
    {"by-act", std::make_shared<query::UP>(std::string("/val/ranked/v1/leaderboards/by-act/"))}
};

const std::unordered_map<std::string, std::shared_ptr<query::QueryType>> VAL_STATUS_V1 = {
    {"v1", std::make_shared<query::U>(std::string("/val/status/v1/platform-data"))}
};

const std::unordered_map<std::string, std::unordered_map<std::string, std::shared_ptr<query::QueryType>>> RiotApiClient::query_types = {
    {"ACCOUNT-V1", ACCOUNT_V1},
    {"CHAMPION-MASTERY-V1", CHAMPION_MASTERY_V1},
    {"CHAMPION-V3", CHAMPION_V3},
    {"CLASH-V1", CLASH_V1},
    {"LEAGUE-EXP-V4", LEAGUE_EXP_V4},
    {"LEAGUE-V4", LEAGUE_V4},
    {"LOL-CHALLENGES-V1", LOL_CHALLENGES_V1},
    {"LOL-STATUS", LOL_STATUS},
    {"LOR-MATCH-V1", LOR_MATCH_V1},
    {"LOR-RANKED-V1", LOR_RANKED_V1},
    {"LOR-STATUS-V1", LOR_STATUS_V1},
    {"MATCH-V5", MATCH_V5},
    {"SPECTATOR-V4", SPECTATOR_V4},
    {"SUMMONER-V4", SUMMONER_V4},
    {"TFT-LEAGUE-V1", TFT_LEAGUE_V1},
    {"TFT-MATCH-V1", TFT_MATCH_V1},
    {"TFT-STATUS-V1", TFT_STATUS_V1},
    {"TFT-SUMMONER-V1", TFT_SUMMONER_V1},
    {"VAL-CONTENT-V1", VAL_CONTENT_V1},
    {"VAL-MATCH-V1", VAL_MATCH_V1},
    {"VAL-RANKED-V1", VAL_RANKED_V1},
    {"VAL-STATUS-V1", VAL_STATUS_V1}
};

#include <iostream>

Json::Value RiotApiClient::query(std::string endpoint, std::string end_type, std::vector<std::string> params) {

    for (int i = 0; i < params.size(); i++) {
        params[i] = this->encode_url(params[i]);
    }

    std::string address = (*(this->query_types.at(endpoint).at(end_type))).construct_url(params);

    std::shared_ptr<query_attempts> attempts = std::make_shared<query_attempts>(0, 0, 0);

    return this->get(address, attempts);
}
