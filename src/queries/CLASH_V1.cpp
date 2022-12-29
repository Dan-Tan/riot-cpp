#include <string>
#include "../client.h"
#define ENCODE(x) (std::string(curl_easy_escape(this->easy_handle, x.c_str(), x.length())))

using namespace client;

Json::Value RiotApiClient::CLASH_V1_summonerid(std::string summoner_id, std::string region) {
    summoner_id = ENCODE(summoner_id);
    std::string end_url = "/lol/clash/v1/players/by-summoner/" + summoner_id;
    return this->get(end_url, region, NULL);
}

Json::Value RiotApiClient::CLASH_V1_teamid(std::string team_id, std::string region) {
    team_id = ENCODE(team_id);
    std::string end_url = "/lol/clash/v1/teams/" + team_id;
    return this->get(end_url, region, NULL);
}

Json::Value RiotApiClient::CLASH_V1_tournament_byteam(std::string team_id, std::string region) {
    team_id = ENCODE(team_id);
    std::string end_url = "/lol/clash/v1/tournaments/by-team/" + team_id;
    return this->get(end_url, region, NULL);
}

Json::Value RiotApiClient::CLASH_V1_tournamentid(std::string tournament_id, std::string region) {
    tournament_id = ENCODE(tournament_id);
    std::string end_url = "/lol/clash/v1/tournaments/" + tournament_id;
    return this->get(end_url, region, NULL);
}
