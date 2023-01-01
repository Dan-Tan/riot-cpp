#include "../../client.h"
#include <string>

using namespace client;

Json::Value RiotApiClient::LOR_RANKED_V1(std::string region) {
    std::string end_url = "/lor/ranked/v1/leaderboards";
    return this->get(end_url, region, NULL);
}

Json::Value RiotApiClient::LOR_STATUS_V1(std::string region) {
    std::string end_url = "/lor/status/v1/platform-data";
    return this->get(end_url, region, NULL);
}
