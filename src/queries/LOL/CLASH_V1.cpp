#include <string>
#include "../../client.h"

using namespace client;

Json::Value RiotApiClient::CLASH_V1_summonerid(std::string summoner_id, std::string region) {
    summoner_id = encode_url(summoner_id);
    std::string end_url = "/lol/clash/v1/players/by-summoner/" + summoner_id;
    end_url = this->get_BASE_URL(region) + end_url;
    return this->get(end_url, nullptr);
}

Json::Value RiotApiClient::CLASH_V1_teamid(std::string team_id, std::string region) {
    team_id = encode_url(team_id);
    std::string end_url = "/lol/clash/v1/teams/" + team_id;
    end_url = this->get_BASE_URL(region) + end_url;
    return this->get(end_url, nullptr);
}

Json::Value RiotApiClient::CLASH_V1_tournament_byteam(std::string team_id, std::string region) {
    team_id = encode_url(team_id);
    std::string end_url = "/lol/clash/v1/tournaments/by-team/" + team_id;
    end_url = this->get_BASE_URL(region) + end_url;
    return this->get(end_url, nullptr);
}

Json::Value RiotApiClient::CLASH_V1_tournamentid(std::string tournament_id, std::string region) {
    tournament_id = encode_url(tournament_id);
    std::string end_url = "/lol/clash/v1/tournaments/" + tournament_id;
    end_url = this->get_BASE_URL(region) + end_url;
    return this->get(end_url, nullptr);
}
