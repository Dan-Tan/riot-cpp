#include "../../client.h"
#include <string>

using namespace client;

Json::Value RiotApiClient::SPECTATOR_V4_summoner_id(std::string summoner_id, std::string region) {
    summoner_id = encode_url(summoner_id);
    std::string end_url = "/lol/spectator/v4/active-games/by-summoner/" + summoner_id;
    end_url = this->get_BASE_URL(region) + end_url;
    return this->get(end_url, nullptr);
}

Json::Value RiotApiClient::SPECTATOR_V4_featured(std::string region) {
    std::string end_url = "/lol/spectator/v4/featured-games";
    end_url = this->get_BASE_URL(region) + end_url;
    return this->get(end_url, nullptr);
}
