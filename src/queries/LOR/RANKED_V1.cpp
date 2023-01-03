#include "../../client.h"
#include <string>

using namespace client;

Json::Value RiotApiClient::LOR_RANKED_V1(std::string region) {
    std::string end_url = "/lor/ranked/v1/leaderboards";
    end_url = this->get_BASE_URL(region) + end_url;
    return this->get(end_url, nullptr);
}

Json::Value RiotApiClient::LOR_STATUS_V1(std::string region) {
    std::string end_url = "/lor/status/v1/platform-data";
    end_url = this->get_BASE_URL(region) + end_url;
    return this->get(end_url, nullptr);
}
