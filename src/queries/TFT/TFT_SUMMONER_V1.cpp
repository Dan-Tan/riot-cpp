#include "../../client.h"
#include <string>
#define ENCODE(x) (std::string(curl_easy_escape(this->easy_handle, x.c_str(), x.length())))

using namespace client;

Json::Value RiotApiClient::TFT_SUMMONER_V1_account(std::string account_id, std::string region) {
    account_id = ENCODE(account_id);
    std::string end_url = "/tft/summoner/v1/summoners/by-account/" + account_id;
    return this->get(end_url, region, NULL);
}

Json::Value RiotApiClient::TFT_SUMMONER_V1_name(std::string name, std::string region) {
    name = ENCODE(name);
    std::string end_url = "/tft/summoner/v1/summoners/by-name/" + name;
    return this->get(end_url, region, NULL);
}

Json::Value RiotApiClient::TFT_SUMMONER_V1_puuid(std::string puuid, std::string region) { puuid = ENCODE(puuid); 
    std::string end_url = "/tft/summoner/v1/summoners/by-puuid/" + puuid;
    return this->get(end_url, region, NULL);
}

Json::Value RiotApiClient::TFT_SUMMONER_V1_summonerid(std::string summoner_id, std::string region) {
    summoner_id = ENCODE(summoner_id);
    std::string end_url = "/tft/summoner/v1/summoners/" + summoner_id;
    return this->get(end_url, region, NULL);
}
