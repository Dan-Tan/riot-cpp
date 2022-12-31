#include "../../client.h"
#include <string>
#define encode(x) (std::string(curl_easy_escape(this->easy_handle, x.c_str(), x.length())))

using namespace client;

Json::Value RiotApiClient::CHAMPION_MASTERY_V3_summonerid(std::string summoner_id, std::string region) {
    summoner_id = ENCODE(summoner_id);
    std::string end_url = "/lol/champion-mastery/v4/champion-masteries/by-summoner/" + summoner_id;
    return this->get(end_url, region, NULL);
}

Json::Value RiotApiClient::CHAMPION_MASTERY_V3_summoner_champion(std::string summoner_id, std::string champion_id, std::string region) {
    summoner_id = ENCODE(summoner_id); champion_id = ENCODE(champion_id);
    std::string end_url = "/lol/champion-mastery/v4/champion-masteries/by-summoner/" + summoner_id + "/by-champion/" + champion_id;
    return this->get(end_url, region, NULL);
}

Json::Value RiotApiClient::CHAMPION_MASTERY_V3_top_summoner(std::string summoner_id, std::string region) {
    summoner_id = ENCODE(summoner_id);
    std::string end_url = "/lol/champion-mastery/v4/champion-masteries/by-summoner/{encryptedSummonerId}" + summoner_id + "/top";
    return this->get(end_url, region, NULL);
}

Json::Value RiotApiClient::CHAMPION_MASTERY_V3_score(std::string summoner_id, std::string region) {
    summoner_id = ENCODE(summoner_id);
    std::string end_url = "/lol/champion-mastery/v4/scores/by-summoner/" + summoner_id;
    return this->get(end_url, region, NULL);
}

