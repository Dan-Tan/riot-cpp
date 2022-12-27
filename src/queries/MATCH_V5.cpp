#include <string>
#include "../client.h"

using namespace client;

Json::Value RiotApiClient::MATCH_V5_puuid(std::string puuid, std::string region) {
    std::string end_url = "/lol/match/v5/matches/by-puuid/" + puuid + "/ids";
    return this->get(end_url, region, 0);
}

Json::Value RiotApiClient::MATCH_V5_matchid(std::string match_id, std::string region) {
    std::string end_url = "/lol/match/v5/matches/" + match_id;
    return this->get(end_url, region, 0);
}

Json::Value RiotApiClient::MATCH_V5_timeline(std::string match_id, std::string region) {
    std::string end_url = "/lol/match/v5/matches" + match_id + "/timeline";
    return this->get(end_url, region, 0);
}
