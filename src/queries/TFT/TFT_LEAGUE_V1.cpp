#include "../../client.h"
#include <string>

using namespace client;

Json::Value RiotApiClient::TFT_LEAGUE_V1_challenger(std::string region) {
    std::string end_url = "/tft/league/v1/challenger";
    end_url = this->get_BASE_URL(region) + end_url;
    return this->get(end_url, nullptr);
}

Json::Value RiotApiClient::TFT_LEAGUE_V1_summonerid(std::string summoner_id, std::string region) {
    summoner_id = encode_url(summoner_id);
    std::string end_url = "/tft/league/v1/entries/by-summoner" + summoner_id;
    end_url = this->get_BASE_URL(region) + end_url;
    return this->get(end_url, nullptr);
}

Json::Value RiotApiClient::TFT_LEAGUE_V1_tier_div(std::string tier, std::string division, std::string region) {
    tier = encode_url(tier); division = encode_url(division);
    std::string end_url = "/tft/league/v1/entries/" + tier + "/" + division;
    end_url = this->get_BASE_URL(region) + end_url;
    return this->get(end_url, nullptr);
}

Json::Value RiotApiClient::TFT_LEAGUE_V1_grandmaster(std::string region) {
    std::string end_url = "/tft/league/v1/grandmaster";
    end_url = this->get_BASE_URL(region) + end_url;
    return this->get(end_url, nullptr);
}

Json::Value RiotApiClient::TFT_LEAGUE_V1_master(std::string region) {
    std::string end_url = "/tft/league/v1/master";
    end_url = this->get_BASE_URL(region) + end_url;
    return this->get(end_url, nullptr);
}

Json::Value RiotApiClient::TFT_LEAGUE_V1_leagueid(std::string league_id, std::string region) {
    league_id = encode_url(league_id);
    std::string end_url = "/tft/league/v1/leagues/leagueId/" + league_id;
    end_url = this->get_BASE_URL(region) + end_url;
    return this->get(end_url, nullptr);
}

Json::Value RiotApiClient::TFT_LEAGUE_V1_rated_ladder(std::string queue, std::string region) {
    queue = encode_url(queue);
    std::string end_url = "/tft/league/v1/rated-ladders/" + queue + "/top";
    end_url = this->get_BASE_URL(region) + end_url;
    return this->get(end_url, nullptr);
}
