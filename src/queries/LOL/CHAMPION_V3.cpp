#include "../../client.h"
#include <string>

using namespace client;

Json::Value RiotApiClient::CHAMPION_V3_rotation(std::string region) {
    std::string end_url = "/lol/platform/v3/champion-rotations";
    return this->get(end_url, region, NULL);
}
