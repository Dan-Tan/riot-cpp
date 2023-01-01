#include "../../client.h"
#include <string>
#define ENCODE(x) (std::string(curl_easy_escape(this->easy_handle, x.c_str(), x.length())))

using namespace client;

Json::Value RiotApiClient::TFT_LEAGUE_V1_challenger(std::string region) {
    std::string end_url = "/tft/league/v1/challenger";
    return this->get(end_url, region, NULL);
}

Json::Value RiotApiClient::TFT_LEAGUE_V1_summonerid(std::string summoner_id, std::string region) {
    summoner_id = ENCODE(summoner_id);
    std::string end_url = "/tft/league/v1/entries/by-summoner" + summoner_id;
    return this->get(end_url, region, NULL);
}

Json::Value RiotApiClient::TFT_LEAGUE_V1_tier_div(std::string tier, std::string division, std::string region) {
    tier = ENCODE(tier); division = ENCODE(division);
    std::string end_url = "/tft/league/v1/entries/" + tier + "/" + division;
    return this->get(end_url, region, NULL);
}

Json::Value RiotApiClient::TFT_LEAGUE_V1_grandmaster(std::string region) {
    std::string end_url = "/tft/league/v1/grandmaster";
    return this->get(end_url, region, NULL);
}

Json::Value RiotApiClient::TFT_LEAGUE_V1_master(std::string region) {
    std::string end_url = "/tft/league/v1/master";
    return this->get(end_url, region, NULL);
}

Json::Value RiotApiClient::TFT_LEAGUE_V1_leagueid(std::string league_id, std::string region) {
    league_id = ENCODE(league_id);
    std::string end_url = "/tft/league/v1/leagues/leagueId/" + league_id;
    return this->get(end_url, region, NULL);
}

Json::Value RiotApiClient::TFT_LEAGUE_V1_rated_ladder(std::string queue, std::string region) {
    queue = ENCODE(queue);
    std::string end_url = "/tft/league/v1/rated-ladders/" + queue + "/top";
    return this->get(end_url, region, NULL);
}
