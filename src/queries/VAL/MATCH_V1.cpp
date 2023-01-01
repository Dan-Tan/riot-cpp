#include "../../client.h"
#include <string>
#define ENCODE(x) (std::string(curl_easy_escape(this->easy_handle, x.c_str(), x.length())))

using namespace client;

Json::Value RiotApiClient::VAL_MATCH_V1_matchid(std::string match_id, std::string region) {
    match_id = ENCODE(match_id);
    std::string end_url = "/val/match/v1/matches/" + match_id;
    return this->get(end_url, region, NULL);
}

Json::Value RiotApiClient::VAL_MATCH_V1_puuid(std::string puuid, std::string region) {
    puuid = ENCODE(puuid);
    std::string end_url = "/val/match/v1/matchlist/by-puuid/" + puuid;
    return this->get(end_url, region, NULL);
}

Json::Value RiotApiClient::VAL_MATCH_V1_queue(std::string queue, std::string region) {
    queue = ENCODE(queue);
    std::string end_url = "/val/match/v1/recent-matches/by-queue/" + queue;
    return this->get(end_url, region, NULL);
}
