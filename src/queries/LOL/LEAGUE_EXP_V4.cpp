#include "../../client.h"
#include <string>

using namespace client;

Json::Value RiotApiClient::LEAGUE_EXP_V4(std::string queue, std::string tier, std::string division, std::string region) {
    queue = encode_url(queue); tier = encode_url(tier); division = encode_url(division);
    std::string end_url = "/lol/league-exp/v4/entries/" + queue + "/" + tier + "/" + division;
    end_url = this->get_BASE_URL(region) + end_url;
    return this->get(end_url, nullptr);
}
