#include "../../client.h"
#include <string>
#define encode(x) (std::string(curl_easy_escape(this->easy_handle, x.c_str(), x.length())))


Json::Value RiotApiClient::STATUS_V3(std::string region) {
    std::string end_url = "/lol/status/v3/shard-data";
    return this->get(end_url, region, NULL);
}

Json::Value RiotApiClient::STATUS_V4(std::string region) {
    std::string end_url = "/lol/status/v4/platform-data";
    return this->get(end_url, region, NULL);
}
