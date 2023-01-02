#include "../../client.h"
#include <string>

using namespace client;

Json::Value RiotApiClient::STATUS_V3(std::string region) {
    std::string end_url = "/lol/status/v3/shard-data";
    end_url = this->get_BASE_URL(region) + end_url;
    return this->get(end_url, nullptr);
}

Json::Value RiotApiClient::STATUS_V4(std::string region) {
    std::string end_url = "/lol/status/v4/platform-data";
    end_url = this->get_BASE_URL(region) + end_url;
    return this->get(end_url, nullptr);
}
