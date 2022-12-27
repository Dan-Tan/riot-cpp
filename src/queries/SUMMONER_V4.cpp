#include <string>
#include "../client.h"

using namespace client;

Json::Value RiotApiClient::SUMMONER_V4_rso_puuid(std::string rso_puuid, std::string region) {
    std::string end_url = "/fulfillment/v1/summoners/by-puuid/" + rso_puuid;
    return this->get(end_url, region, 0);
}

Json::Value RiotApiClient::SUMMONER_V4_accountid(std::string account_id, std::string region) {
    std::string end_url = "/lol/summoner/v4/summoners/by-account/" + account_id;
    return this->get(end_url, region, 0);
}

Json::Value RiotApiClient::SUMMONER_V4_summoner_name(std::string summoner_name, std::string region) {
    std::string end_url = "/lol/summoner/v4/summoners/by-name" + summoner_name;
    return this->get(end_url, region, 0);
}

Json::Value RiotApiClient::SUMMONER_V4_puuid(std::string puuid, std::string region) {
    std::string end_url = "/lol/summoner/v4/summoners/by_puuid" + puuid;
    return this->get(end_url, region, 0);
}

Json::Value RiotApiClient::SUMMONER_V4_summonerid(std::string summoner_id, std::string region) {
    std::string end_url = "/lol/summoner/v4/summoners/" + summoner_id;
    return this->get(end_url, region, 0);
}
