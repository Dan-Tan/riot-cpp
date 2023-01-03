#include <string>
#include "../../client.h"

using namespace client;

Json::Value RiotApiClient::SUMMONER_V4_rso_puuid(std::string rso_puuid, std::string region) {
    rso_puuid = encode_url(rso_puuid);
    std::string end_url = "/fulfillment/v1/summoners/by-puuid/" + rso_puuid;
    end_url = this->get_BASE_URL(region) + end_url;
    return this->get(end_url, nullptr);
}

Json::Value RiotApiClient::SUMMONER_V4_accountid(std::string account_id, std::string region) {
    account_id = encode_url(account_id);
    std::string end_url = "/lol/summoner/v4/summoners/by-account/" + account_id;
    end_url = this->get_BASE_URL(region) + end_url;
    return this->get(end_url, nullptr);
}

Json::Value RiotApiClient::SUMMONER_V4_summoner_name(std::string summoner_name, std::string region) {
    summoner_name = encode_url(summoner_name);
    std::string end_url = "/lol/summoner/v4/summoners/by-name/" + summoner_name;
    end_url = this->get_BASE_URL(region) + end_url;
    return this->get(end_url, nullptr);
}

Json::Value RiotApiClient::SUMMONER_V4_puuid(std::string puuid, std::string region) {
    puuid = encode_url(puuid);
    std::string end_url = "/lol/summoner/v4/summoners/by-puuid/" + puuid;
    end_url = this->get_BASE_URL(region) + end_url;
    return this->get(end_url, nullptr);
}

Json::Value RiotApiClient::SUMMONER_V4_summonerid(std::string summoner_id, std::string region) {
    summoner_id = encode_url(summoner_id);
    std::string end_url = "/lol/summoner/v4/summoners/" + summoner_id;
    end_url = this->get_BASE_URL(region) + end_url;
    return this->get(end_url, nullptr);
}
