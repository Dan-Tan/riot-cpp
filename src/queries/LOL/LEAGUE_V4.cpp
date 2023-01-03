#include <string>
#include "../../client.h"

using namespace client;

Json::Value RiotApiClient::LEAGUE_V4_challenger(std::string queue, std::string region) {
    queue = encode_url(queue);
    std::string end_url = "/lol/league/v4/challengerleagues/by-queue/" + queue;
    end_url = this->get_BASE_URL(region) + end_url;
    return this->get(end_url, nullptr);
}

Json::Value RiotApiClient::LEAGUE_V4_grandmaster(std::string queue, std::string region) {
    queue = encode_url(queue);
    std::string end_url = "/lol/league/v4/grandmasterleagues/by-queue/" + queue;
    end_url = this->get_BASE_URL(region) + end_url;
    return this->get(end_url, nullptr);
}

Json::Value RiotApiClient::LEAGUE_V4_master(std::string queue, std::string region) {
    queue = encode_url(queue);
    std::string end_url = "/lol/league/v4/masterleagues/by-queue/" + queue;
    end_url = this->get_BASE_URL(region) + end_url;
    return this->get(end_url, nullptr);
}

Json::Value RiotApiClient::LEAGUE_V4_queue(std::string queue, std::string tier, std::string division, std::string region) {
    queue = encode_url(queue); tier = encode_url(tier); division = encode_url(division);
    std::string end_url = "/lol/league/v4/entries/" + queue + "/" + tier + "/" + division;
    end_url = this->get_BASE_URL(region) + end_url;
    return this->get(end_url, nullptr);
}

Json::Value RiotApiClient::LEAGUE_V4_summonerid(std::string summoner_id, std::string region) {
    summoner_id = encode_url(summoner_id); 
    std::string end_url = "/lol/league/v4/entries/by-summoner/" + summoner_id;
    end_url = this->get_BASE_URL(region) + end_url;
    return this->get(end_url, nullptr);
}

Json::Value RiotApiClient::LEAGUE_V4_leagueid(std::string league_id, std::string region) {
    league_id = encode_url(league_id);
    std::string end_url = "/lol/league/v4/leagues/" + league_id;
    end_url = this->get_BASE_URL(region) + end_url;
    return this->get(end_url, nullptr);
}
