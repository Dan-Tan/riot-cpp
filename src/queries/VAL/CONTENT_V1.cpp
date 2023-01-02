#include "../../client.h"
#include <string>

using namespace client;

Json::Value RiotApiClient::VAL_CONTENT_V1(std::string summoner_id, std::string region) {
    std::string end_url = "/val/content/v1/contents";
    end_url = this->get_BASE_URL(region) + end_url;
    return this->get(end_url, nullptr);
}

Json::Value RiotApiClient::VAL_RANKED_V1(std::string act_id, std::string region) {
    act_id = encode_url(act_id);
    std::string end_url = "/val/ranked/v1/leaderboards/by-act/" + act_id;
    end_url = this->get_BASE_URL(region) + end_url;
    return this->get(end_url, nullptr);
}

Json::Value RiotApiClient::VAL_STATUS_V1(std::string region) {
    std::string end_url = "/val/status/v1/platform-data";
    end_url = this->get_BASE_URL(region) + end_url;
    return this->get(end_url, nullptr);
}
