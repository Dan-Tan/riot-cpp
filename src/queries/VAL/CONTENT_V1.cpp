#include "../../client.h"
#include <string>
#define ENCODE(x) (std::string(curl_easy_escape(this->easy_handle, x.c_str(), x.length())))

using namespace client;

Json::Value RiotApiClient::VAL_CONTENT_V1(std::string summoner_id, std::string region) {
    std::string end_url = "/val/content/v1/contents";
    return this->get(end_url, region, NULL);
}

Json::Value RiotApiClient::VAL_RANKED_V1(std::string act_id, std::string region) {
    act_id = ENCODE(act_id);
    std::string end_url = "/val/ranked/v1/leaderboards/by-act/" + act_id;
    return this->get(end_url, region, NULL);
}

Json::Value RiotApiClient::VAL_STATUS_V1(std::string region) {
    std::string end_url = "/val/status/v1/platform-data";
    return this->get(end_url, region, NULL);
}
