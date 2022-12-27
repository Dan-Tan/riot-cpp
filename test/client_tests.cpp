#include <catch2/catch_test_macros.hpp>
#include <string>
#include "../src/client.h"
#include <jsoncpp/json/json.h>

#define KEY_PATH "../../.api_keys/riot_config.json"

using namespace client;

TEST_CASE( "TESTING ACCOUNT QUERIES") {
    RiotApiClient test_client(KEY_PATH, "Not used yet");

    std::vector<std::string> region = {"americas", "asia", "europe"};
    std::string puuid = "nvf_tmMcUOXhaWI4neMvma8WG3iymFlqS32-FpUFsLk3FqH9pGB_oQh5DoecV-Uiue2NIYddgww50A";
    std::string gamename = "DanTan1"; std::string tagline = "fresn";
    std::string game = "val";

    Json::Value result;
    
    for (int i = 0; i < 3; i++) {
        result = test_client.ACCOUNT_V1_puuid(puuid, region[i]);
        REQUIRE(result["puuid"] == puuid);
        result = test_client.ACCOUNT_V1_riotid(gamename, tagline, region[i]);
        REQUIRE(result["gameName"] == gamename);
        REQUIRE(result["tagline"] == tagline);
        result = test_client.ACCOUNT_V1_game(game, puuid, region[i]);
        REQUIRE(result["puuid"] == puuid);
    }
}

// League of Legend Queries

//TEST_CASE( "TESTING LEAGUE QUERIES") {
//    RiotApiClient test_client(KEY_PATH, "Not used yet");
//    
//    std::string region;
//    std::string queue;
//    std::string tier, division;
//    std::string summoner_id;
//    std::string league_id;
//    
//}
//
//TEST_CASE(" TESTING SUMMONER QUERIES", "[RiotApiClient::SUMMONER_V4]") {
//
//}
//
//TEST_CASE( "TESTING MATCH QUERIES", "[RiotApiClient::MATCH_V5]" ) {
//
//}
//
//TEST_CASE( "TESTING CLASH QUERIES") {
//
//}
//
//// TODO: 
//
//// Legends of Runeterra Queries
//
//// Valorant Queries
