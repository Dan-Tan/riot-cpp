#include <string>
#include "../client.h"
#include <curl/curl.h>

using namespace client;

Json::Value RiotApiClient::ACCOUNT_V1_puuid(std::string puuid, std::string region) {
    puuid = this->encode_url(puuid);
    std::string end_url = "/riot/account/v1/accounts/by-puuid/" + puuid;
    end_url = this->get_BASE_URL(region) + end_url;
    return this->get(end_url, nullptr);
}

Json::Value RiotApiClient::ACCOUNT_V1_riotid(std::string gamename, std::string tagline, std::string region) {
    gamename = encode_url(gamename); tagline = encode_url(tagline);
    std::string end_url = "/riot/account/v1/accounts/by-riot-id/" + gamename + "/" + tagline;
    end_url = this->get_BASE_URL(region) + end_url;
    return this->get(end_url, nullptr);
}

Json::Value RiotApiClient::ACCOUNT_V1_game(std::string game, std::string puuid, std::string region) {
    game = encode_url(game); puuid = encode_url(puuid);
    std::string end_url = "/riot/account/v1/active-shards/by-game/" + game + "/by-puuid/" + puuid;
    end_url = this->get_BASE_URL(region) + end_url;
    return this->get(end_url, nullptr);
}
