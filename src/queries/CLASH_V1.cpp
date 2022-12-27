#include <string>
#include "../client.h"

using namespace client;

Json::Value RiotApiClient::CLASH_V1_summonerid(std::string summoner_id, std::string region) {
    std::string end_url = "/lol/clash/v1/players/by-summoner/" + summoner_id;
    return this->get(end_url, region, 0);
}

Json::Value RiotApiClient::CLASH_V1_teamid(std::string team_id, std::string region) {
    std::string end_url = "/lol/clash/v1/teams/" + team_id;
    return this->get(end_url, region, 0);
}

Json::Value RiotApiClient::CLASH_V1_tournament_byteam(std::string team_id, std::string region) {
    std::string end_url = "/lol/clash/v1/tournaments/by-team/" + team_id;
    return this->get(end_url, region, 0);
}

Json::Value RiotApiClient::CLASH_V1_tournamentid(std::string tournament_id, std::string region) {
    std::string end_url = "/lol/clash/v1/tournaments/" + tournament_id;
    return this->get(end_url, region, 0);
}
