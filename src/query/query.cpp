#include <stdexcept>
#include <iostream>
#include <chrono>
#include <thread>
#include <queue>
#include <ctime>
#include "../client/client.h"
#include "query.h"

namespace client {

using opt_args = std::pair<std::string, std::string>;

static const std::unordered_map<std::string_view, std::shared_ptr<query::QueryType>> ACCOUNT_V1 = {
    {"by-puuid",  std::make_shared<query::UP>("/riot/account/v1/accounts/by-puuid/", false)},
    {"by-riot-id", std::make_shared<query::UPP>("/riot/account/v1/accounts/by-riot-id/", false)},
    {"by-game",    std::make_shared<query::UPUP>("/riot/account/v1/active-shards/by-game/", "/by-puuid/", false)},
};

static const std::unordered_map<std::string_view, std::shared_ptr<query::QueryType>> CHAMPION_MASTERY_V4 = {
    {"by-summoner-id",     std::make_shared<query::UP>("/lol/champion-mastery/v4/champion-masteries/by-summoner/", false)},
    {"by-champion",     std::make_shared<query::UPUP>("/lol/champion-mastery/v4/champion-masteries/by-summoner/", "/by-champion/", false)},
    {"summoner-top",    std::make_shared<query::UPU>("/lol/champion-mastery/v4/champion-masteries/by-summoner/", "/top", true)},
    {"summoner-scores", std::make_shared<query::UP>("/lol/champion-mastery/v4/scores/by-summoner/", false)},
};

static const std::unordered_map<std::string_view, std::shared_ptr<query::QueryType>> CHAMPION_V3 = {
    {"rotations", std::make_shared<query::U>("/lol/platform/v3/champion-rotations", false)}
};

static const std::unordered_map<std::string_view, std::shared_ptr<query::QueryType>> CLASH_V1 = {
    {"by-summoner-id",         std::make_shared<query::UP>("/lol/clash/v1/by-summoner/", false)},
    {"by-team",            std::make_shared<query::UP>("/lol/clash/v1/teams/", false)},
    {"tournament-by-team", std::make_shared<query::UP>("/lol/clash/v1/tournaments/by-team/", false)},
    {"by-tournament",      std::make_shared<query::UP>("/lol/clash/v1/tournaments/", false)},
};

static const std::unordered_map<std::string_view, std::shared_ptr<query::QueryType>> LEAGUE_EXP_V4 = {
    {"entries", std::make_shared<query::UPPP>("/lol/league-exp/v4/entries/", true)}
};

static const std::unordered_map<std::string_view, std::shared_ptr<query::QueryType>> LEAGUE_V4 = {
    {"challenger" ,     std::make_shared<query::UP>("/lol/league/v4/challengerleagues/by-queue/", false)},
    {"grandmaster" ,    std::make_shared<query::UP>("/lol/league/v4/grandmasterleagues/by-queue/", false)},
    {"master" ,         std::make_shared<query::UP>("/lol/league/v4/masterleagues/by-queue/", false)},
    {"by-summoner-id",     std::make_shared<query::UP>("/lol/league/v4/entries/by-summoner/", false)},
    {"specific-league", std::make_shared<query::UPPP>("/lol/league/v4/entries/", true)},
    {"by-league-id",       std::make_shared<query::UP>("/lol/league/v4/leagues/", false)}
};

static const std::unordered_map<std::string_view, std::shared_ptr<query::QueryType>> LOL_CHALLENGES_V1 = {
    {"config",               std::make_shared<query::U>("/lol/challenges/v1/challenges/config", false)},
    {"percentiles",          std::make_shared<query::U>("/lol/challenges/v1/challenges/percentiles", false)},
    {"challenge-config",      std::make_shared<query::UPU>("/lol/challenges/v1/challenges/", "/config", false)}, 
    {"challenge-leaderboard", std::make_shared<query::UPUP>("/lol/challenges/v1/challenges/", "/leaderboards/by-level/", true)},
    {"challenge-percentiles", std::make_shared<query::UPU>("/lol/challenges/v1/challenges/", "/percentiles", false)}, 
    {"by-puuid",             std::make_shared<query::UP>("/lol/challenges/v1/player-data/", false)} 
};

static const std::unordered_map<std::string_view, std::shared_ptr<query::QueryType>> LOL_STATUS = {
    {"v3", std::make_shared<query::U>("/lol/status/v3/shard-data", false)},
    {"v4", std::make_shared<query::U>("/lol/status/v4/platform-data", false)}
};

static const std::unordered_map<std::string_view, std::shared_ptr<query::QueryType>> LOR_MATCH_V1 = {
    {"by-puuid", std::make_shared<query::UPU>("/lor/match/v1/matches/by-puuid/", "/ids", false)}, 
    {"by-match", std::make_shared<query::UP>("/lor/match/v1/matches/", false)}
};

static const std::unordered_map<std::string_view, std::shared_ptr<query::QueryType>> LOR_RANKED_V1 = {
    {"leaderboards", std::make_shared<query::U>("/lor/ranked/v1/leaderboards", false)}
};

static const std::unordered_map<std::string_view, std::shared_ptr<query::QueryType>> LOR_STATUS_V1 = {
    {"v1", std::make_shared<query::U>("/lor/status/v1/platform-data", false)}
};

static const std::unordered_map<std::string_view, std::shared_ptr<query::QueryType>> MATCH_V5 = {
    {"by-puuid", std::make_shared<query::UPU>("/lol/match/v5/matches/by-puuid/", "/ids", true)},
    {"by-match-id", std::make_shared<query::UP>("/lol/match/v5/matches/", false)},
    {"match-timeline", std::make_shared<query::UPU>("/lol/match/v5/matches/", "/timeline", false)}
};

static const std::unordered_map<std::string_view, std::shared_ptr<query::QueryType>> SUMMONER_V4 = {
    {"by-rso-puuid", std::make_shared<query::UP>("/fulfillment/v1/summoners/by-puuid/", false)},
    {"by-account-id", std::make_shared<query::UP>("/lol/summoner/v4/summoners/by-account/", false)},
    {"by-name", std::make_shared<query::UP>("/lol/summoner/v4/summoners/by-name/", false)},
    {"by-puuid", std::make_shared<query::UP>("/lol/summoner/v4/summoners/by-puuid/", false)},
    {"by-summoner-id", std::make_shared<query::UP>("/lol/summoner/v4/summoners/", false)}
};

static const std::unordered_map<std::string_view, std::shared_ptr<query::QueryType>> SPECTATOR_V4 = {
    {"by-summoner-id", std::make_shared<query::UP>("/lol/spectator/v4/active-games/by-summoner/", false)},
    {"featured-games", std::make_shared<query::U>("/lol/spectator/v4/featured-games", false)}
};

static const std::unordered_map<std::string_view, std::shared_ptr<query::QueryType>> TFT_LEAGUE_V1 = {
    {"challenger", std::make_shared<query::U>("/tft/league/v1/challenger", false)},
    {"grandmaster", std::make_shared<query::U>("/tft/league/v1/grandmaster", false)},
    {"master", std::make_shared<query::U>("/tft/league/v1/master", false)},
    {"by-summoner-id", std::make_shared<query::UP>("/tft/league/v1/entries/by-summoner/", false)},
    {"by-league-id", std::make_shared<query::UP>("/tft/league/v1/leagues/", false)},
    {"queue-top", std::make_shared<query::UPU>("/tft/league/v1/rated-ladders/", "/top", false)},
    {"by-tier-division", std::make_shared<query::UPP>("/tft/league/v1/entries/", true)}
};

static const std::unordered_map<std::string_view, std::shared_ptr<query::QueryType>> TFT_MATCH_V1 = {
    {"by-puuid", std::make_shared<query::UPU>("/tft/match/v1/matches/by-puuid/", "/ids", true)},
    {"by-match", std::make_shared<query::UP>("/tft/match/v1/matches/", false)}
};

static const std::unordered_map<std::string_view, std::shared_ptr<query::QueryType>> TFT_STATUS_V1 = {
    {"v1", std::make_shared<query::U>("/tft/status/v1/platform-data", false)}
};

static const std::unordered_map<std::string_view, std::shared_ptr<query::QueryType>> TFT_SUMMONER_V1 = {
    {"by-account", std::make_shared<query::UP>("/tft/summoner/v1/summoners/by-account/", false)},
    {"by-name", std::make_shared<query::UP>("/tft/summoner/v1/summoners/by-name/", false)},
    {"by-puuid", std::make_shared<query::UP>("/tft/summoner/v1/summoners/by-puuid/", false)},
    {"by-summoner-id", std::make_shared<query::UP>("/tft/summoner/v1/summoners/", false)},
};;

static const std::unordered_map<std::string_view, std::shared_ptr<query::QueryType>> VAL_CONTENT_V1 = {
    {"content", std::make_shared<query::U>("/val/content/v1/contents", true)}
};

static const std::unordered_map<std::string_view, std::shared_ptr<query::QueryType>> VAL_MATCH_V1 = {
    {"by-match", std::make_shared<query::UP>("/val/match/v1/matches/", false)},
    {"by-puuid", std::make_shared<query::UP>("/val/match/v1/matchlists/by-puuid/", false)},
    {"by-queue", std::make_shared<query::UP>("/val/match/v1/recent-matches/by-queue/", false)}
};

static const std::unordered_map<std::string_view, std::shared_ptr<query::QueryType>> VAL_RANKED_V1 = {
    {"by-act", std::make_shared<query::UP>("/val/ranked/v1/leaderboards/by-act/", true)}
};

static const std::unordered_map<std::string_view, std::shared_ptr<query::QueryType>> VAL_STATUS_V1 = {
    {"v1", std::make_shared<query::U>("/val/status/v1/platform-data", true)}
};

const std::unordered_map<std::string_view, std::unordered_map<std::string_view, std::shared_ptr<query::QueryType>>> RiotApiClient::query_types = {
    {"ACCOUNT-V1", ACCOUNT_V1},
    {"CHAMPION-MASTERY-V4", CHAMPION_MASTERY_V4},
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

static inline void wait_until(std::time_t send_time) {
    const std::time_t c_time = std::time(NULL);
    std::time_t current_time = std::mktime(std::gmtime(&c_time));
    if (current_time >= send_time) {
        return;
    } else {
        std::this_thread::sleep_for(std::chrono::seconds(send_time - current_time));
        return;
    }
}

Json::Value RiotApiClient::query(std::string endpoint, std::string end_type, std::vector<std::string> params, std::vector<opt_args> optional_args) {

    for (int i = 0; i < params.size(); i++) {
        params[i] = this->encode_url(params[i]);
    }

    try {
        bool parse_success; std::string address = (*(this->query_types.at(endpoint).at(end_type))).construct_url(params, optional_args);
        std::shared_ptr<query::query> request = std::make_shared<query::query>(endpoint + "-" + end_type, params.at(0), address);
        while (this->request_handler->review_request(request)) {
            if (request->last_response == 200) {
                return request->response_content;
            } 
            if (!this->request_handler->validate_request(request)) {
                (*this->logger) << logging::LEVEL::WARNING << "Request sent was invalid or the server is unavailable" << 0;
                throw std::runtime_error("Request sent was invalid or the server is unavailable");
            }
            wait_until(request->send_time);
            this->get(request);
        }
        //log failure
        return request->response_content;
    }
    catch (const std::out_of_range& endpoint_exception) { //invalid request
        throw std::invalid_argument(std::string(endpoint_exception.what()) + "\nGiven request type does not exist: (" + endpoint + ", " + end_type + ")");
    }
}
}
