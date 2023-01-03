#include "../../client.h"
#include <string>

using namespace client;

Json::Value RiotApiClient::CHAMPION_MASTERY_V3_summonerid(std::string summoner_id, std::string region) {
    summoner_id = encode_url(summoner_id);
    std::string end_url = "/lol/champion-mastery/v4/champion-masteries/by-summoner/" + summoner_id;
    end_url = this->get_BASE_URL(region) + end_url;
    return this->get(end_url, nullptr);
}

Json::Value RiotApiClient::CHAMPION_MASTERY_V3_summoner_champion(std::string summoner_id, std::string champion_id, std::string region) {
    summoner_id = encode_url(summoner_id); champion_id = encode_url(champion_id);
    std::string end_url = "/lol/champion-mastery/v4/champion-masteries/by-summoner/" + summoner_id + "/by-champion/" + champion_id;
    end_url = this->get_BASE_URL(region) + end_url;
    return this->get(end_url, nullptr);
}

Json::Value RiotApiClient::CHAMPION_MASTERY_V3_top_summoner(std::string summoner_id, std::string region) {
    summoner_id = encode_url(summoner_id);
    std::string end_url = "/lol/champion-mastery/v4/champion-masteries/by-summoner/{encryptedSummonerId}" + summoner_id + "/top";
    end_url = this->get_BASE_URL(region) + end_url;
    return this->get(end_url, nullptr);
}

Json::Value RiotApiClient::CHAMPION_MASTERY_V3_score(std::string summoner_id, std::string region) {
    summoner_id = encode_url(summoner_id);
    std::string end_url = "/lol/champion-mastery/v4/scores/by-summoner/" + summoner_id;
    end_url = this->get_BASE_URL(region) + end_url;
    return this->get(end_url, nullptr);
}

