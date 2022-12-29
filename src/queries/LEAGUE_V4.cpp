#include <string>
#include "../client.h"
#define ENCODE(x) (std::string(curl_easy_escape(this->easy_handle, x.c_str(), x.length())))

using namespace client;

Json::Value RiotApiClient::LEAGUE_V4_challenger(std::string queue, std::string region) {
    queue = ENCODE(queue);
    std::string end_url = "/lol/league/v4/challengerleagues/by-queue/" + queue;
    return this->get(end_url, region, NULL);
}

Json::Value RiotApiClient::LEAGUE_V4_grandmaster(std::string queue, std::string region) {
    queue = ENCODE(queue);
    std::string end_url = "/lol/league/v4/grandmasterleagues/by-queue/" + queue;
    return this->get(end_url, region, NULL);
}

Json::Value RiotApiClient::LEAGUE_V4_master(std::string queue, std::string region) {
    queue = ENCODE(queue);
    std::string end_url = "/lol/league/v4/masterleagues/by-queue/" + queue;
    return this->get(end_url, region, NULL);
}

Json::Value RiotApiClient::LEAGUE_V4_queue(std::string queue, std::string tier, std::string division, std::string region) {
    queue = ENCODE(queue); tier = ENCODE(tier); division = ENCODE(division);
    std::string end_url = "/lol/league/v4/entries/" + queue + "/" + tier + "/" + division;
    return this->get(end_url, region, NULL);
}

Json::Value RiotApiClient::LEAGUE_V4_summonerid(std::string summoner_id, std::string region) {
    summoner_id = ENCODE(summoner_id); 
    std::string end_url = "/lol/league/v4/entries/by-summoner/" + summoner_id;
    return this->get(end_url, region, NULL);
}

Json::Value RiotApiClient::LEAGUE_V4_leagueid(std::string league_id, std::string region) {
    league_id = ENCODE(league_id);
    std::string end_url = "/lol/league/v4/leagues/" + league_id;
    return this->get(end_url, region, NULL);
}
