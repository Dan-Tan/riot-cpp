#include <string>
#include "../client.h"

using namespace client;

Json::Value RiotApiClient::LEAGUE_V4_challenger(std::string queue, std::string region) {
    std::string end_url = "/lol/league/v4/challenger/by-queue/" + queue;
    return this->get(end_url, region, 0);
}

Json::Value RiotApiClient::LEAGUE_V4_grandmaster(std::string queue, std::string region) {
    std::string end_url = "/lol/league/v4/grandmaster/by-queue/" + queue;
    return this->get(end_url, region, 0);
}

Json::Value RiotApiClient::LEAGUE_V4_master(std::string queue, std::string region) {
    std::string end_url = "/lol/league/v4/master/by-queue/" + queue;
    return this->get(end_url, region, 0);
}

Json::Value RiotApiClient::LEAGUE_V4_queue(std::string queue, std::string tier, std::string division, std::string region) {
    std::string end_url = "/lol/league/v4/entries/" + queue + "/" + tier + "/" + division;
    return this->get(end_url, region, 0);
}

Json::Value RiotApiClient::LEAGUE_V4_summonerid(std::string summoner_id, std::string region) {
    std::string end_url = "/lol/league/v4/entries/by-summoner/" + summoner_id;
    return this->get(end_url, region, 0);
}

Json::Value RiotApiClient::LEAGUE_V4_leagueid(std::string league_id, std::string region) {
    std::string end_url = "lol/league/v4/leagues/" + league_id;
    return this->get(end_url, region, 0);
}
