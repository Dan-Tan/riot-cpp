#include <string>
#include "../client.h"
#include <curl/curl.h>

#define ENCODE(x) (std::string(curl_easy_escape(this->easy_handle, x.c_str(), x.length())))

using namespace client;

Json::Value RiotApiClient::ACCOUNT_V1_puuid(std::string puuid, std::string region) {
    puuid = ENCODE(puuid);
    std::string end_url = "/riot/account/v1/accounts/by-puuid/" + puuid;
    return this->get(end_url, region, NULL);
}

Json::Value RiotApiClient::ACCOUNT_V1_riotid(std::string gamename, std::string tagline, std::string region) {
    gamename = ENCODE(gamename); tagline = ENCODE(tagline);
    std::string end_url = "/riot/account/v1/accounts/by-riot-id/" + gamename + "/" + tagline;
    return this->get(end_url, region, NULL);
}

Json::Value RiotApiClient::ACCOUNT_V1_game(std::string game, std::string puuid, std::string region) {
    game = ENCODE(game); puuid = ENCODE(puuid);
    std::string end_url = "/riot/account/v1/active-shards/by-game/" + game + "/by-puuid/" + puuid;
    return this->get(end_url, region, NULL);
}
