#include <string>
#include "../../client.h"

using namespace client;

Json::Value RiotApiClient::MATCH_V5_puuid(std::string puuid, std::string region) {
    puuid = encode_url(puuid);
    std::string end_url = "/lol/match/v5/matches/by-puuid/" + puuid + "/ids";
    end_url = this->get_BASE_URL(region) + end_url;
    return this->get(end_url, nullptr);
}

Json::Value RiotApiClient::MATCH_V5_matchid(std::string match_id, std::string region) {
    match_id = encode_url(match_id);
    std::string end_url = "/lol/match/v5/matches/" + match_id;
    end_url = this->get_BASE_URL(region) + end_url;
    return this->get(end_url, nullptr);
}

Json::Value RiotApiClient::MATCH_V5_timeline(std::string match_id, std::string region) {
    match_id = encode_url(match_id);
    std::string end_url = "/lol/match/v5/matches/" + match_id + "/timeline";
    end_url = this->get_BASE_URL(region) + end_url;
    return this->get(end_url, nullptr);
}
