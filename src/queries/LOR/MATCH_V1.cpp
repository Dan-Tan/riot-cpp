#include "../../client.h"
#include <string>

using namespace client;

Json::Value RiotApiClient::LOR_MATCH_V1_puuid(std::string puuid, std::string region) {
    puuid = encode_url(puuid);
    std::string end_url = "/lor/match/v1/matches/by-puuid/" + puuid + "/ids";
    end_url = this->get_BASE_URL(region) + end_url;
    return this->get(end_url, nullptr);
}

Json::Value RiotApiClient::LOR_MATCH_V1_matchid(std::string match_id, std::string region) {
    match_id = encode_url(match_id);
    std::string end_url = "/lor/match/v1/matches/" + match_id;
    end_url = this->get_BASE_URL(region) + end_url;
    return this->get(end_url, nullptr);
}

