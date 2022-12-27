#include <string>
#include "../client.h"

using namespace client;

Json::Value RiotApiClient::ACCOUNT_V1_puuid(std::string puuid, std::string region) {
    std::string end_url = "/riot/account/v1/accounts/by-puuid/" + puuid;
    return this->get(end_url, region, 0);
}

Json::Value RiotApiClient::ACCOUNT_V1_riotid(std::string gamename, std::string tagline, std::string region) {
    std::string end_url = "/riot/account/v1/accounts/by-puuid/" + gamename + "/" + tagline;
    return this->get(end_url, region, 0);
}

Json::Value RiotApiClient::ACCOUNT_V1_game(std::string game, std::string puuid, std::string region) {
    std::string end_url = "/riot/account/v1/active-shards/by-game/" + game + "/by-puuid/" + puuid;
    return this->get(end_url, region, 0);
}
