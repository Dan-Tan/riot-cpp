#include "../../client.h"
#include <string>

using namespace client;

Json::Value RiotApiClient::VAL_MATCH_V1_matchid(std::string match_id, std::string region) {
    match_id = encode_url(match_id);
    std::string end_url = "/val/match/v1/matches/" + match_id;
    end_url = this->get_BASE_URL(region) + end_url;
    return this->get(end_url, nullptr);
}

Json::Value RiotApiClient::VAL_MATCH_V1_puuid(std::string puuid, std::string region) {
    puuid = encode_url(puuid);
    std::string end_url = "/val/match/v1/matchlist/by-puuid/" + puuid;
    end_url = this->get_BASE_URL(region) + end_url;
    return this->get(end_url, nullptr);
}

Json::Value RiotApiClient::VAL_MATCH_V1_queue(std::string queue, std::string region) {
    queue = encode_url(queue);
    std::string end_url = "/val/match/v1/recent-matches/by-queue/" + queue;
    end_url = this->get_BASE_URL(region) + end_url;
    return this->get(end_url, nullptr);
}
