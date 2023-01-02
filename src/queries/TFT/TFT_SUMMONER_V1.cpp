#include "../../client.h"
#include <string>

using namespace client;

Json::Value RiotApiClient::TFT_SUMMONER_V1_account(std::string account_id, std::string region) {
    account_id = encode_url(account_id);
    std::string end_url = "/tft/summoner/v1/summoners/by-account/" + account_id;
    end_url = this->get_BASE_URL(region) + end_url;
    return this->get(end_url, nullptr);
}

Json::Value RiotApiClient::TFT_SUMMONER_V1_name(std::string name, std::string region) {
    name = encode_url(name);
    std::string end_url = "/tft/summoner/v1/summoners/by-name/" + name;
    end_url = this->get_BASE_URL(region) + end_url;
    return this->get(end_url, nullptr);
}

Json::Value RiotApiClient::TFT_SUMMONER_V1_puuid(std::string puuid, std::string region) { puuid = encode_url(puuid); 
    std::string end_url = "/tft/summoner/v1/summoners/by-puuid/" + puuid;
    end_url = this->get_BASE_URL(region) + end_url;
    return this->get(end_url, nullptr);
}

Json::Value RiotApiClient::TFT_SUMMONER_V1_summonerid(std::string summoner_id, std::string region) {
    summoner_id = encode_url(summoner_id);
    std::string end_url = "/tft/summoner/v1/summoners/" + summoner_id;
    end_url = this->get_BASE_URL(region) + end_url;
    return this->get(end_url, nullptr);
}
