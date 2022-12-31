#include <string>
#include "../../client.h"
#define ENCODE(x) (std::string(curl_easy_escape(this->easy_handle, x.c_str(), x.length())))

using namespace client;

Json::Value RiotApiClient::SUMMONER_V4_rso_puuid(std::string rso_puuid, std::string region) {
    rso_puuid = ENCODE(rso_puuid);
    std::string end_url = "/fulfillment/v1/summoners/by-puuid/" + rso_puuid;
    return this->get(end_url, region, NULL);
}

Json::Value RiotApiClient::SUMMONER_V4_accountid(std::string account_id, std::string region) {
    account_id = ENCODE(account_id);
    std::string end_url = "/lol/summoner/v4/summoners/by-account/" + account_id;
    return this->get(end_url, region, NULL);
}

Json::Value RiotApiClient::SUMMONER_V4_summoner_name(std::string summoner_name, std::string region) {
    summoner_name = ENCODE(summoner_name);
    std::string end_url = "/lol/summoner/v4/summoners/by-name/" + summoner_name;
    return this->get(end_url, region, NULL);
}

Json::Value RiotApiClient::SUMMONER_V4_puuid(std::string puuid, std::string region) {
    puuid = ENCODE(puuid);
    std::string end_url = "/lol/summoner/v4/summoners/by-puuid/" + puuid;
    return this->get(end_url, region, NULL);
}

Json::Value RiotApiClient::SUMMONER_V4_summonerid(std::string summoner_id, std::string region) {
    summoner_id = ENCODE(summoner_id);
    std::string end_url = "/lol/summoner/v4/summoners/" + summoner_id;
    return this->get(end_url, region, NULL);
}
