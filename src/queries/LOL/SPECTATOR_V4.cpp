#include "../../client.h"
#include <string>
#define encode(x) (std::string(curl_easy_escape(this->easy_handle, x.c_str(), x.length())))


Json::Value RiotApiClient::SPECTATOR_V4_summoner_id(std::string summoner_id, std::string region) {
    summoner_id = ENCODE(summoner_id);
    std::string end_url = "/lol/spectator/v4/active-games/by-summoner/" + summoner_id;
    return this->get(end_url, region, NULL);
}

Json::Value RiotApiClient::SPECTATOR_V4_featured(std::string region) {
    std::string end_url = "/lol/spectator/v4/featured-games";
    return this->get(end_url, region, NULL);
}
