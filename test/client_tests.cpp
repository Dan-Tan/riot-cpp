#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <ctime>
#include <queue>
#include <vector>
#include <string>
#include "../src/riot-cpp/client/client.h"
#include "simdjson.h"

#define CONFIG "../../.api_keys/riot_config.json", "../test/log_file.txt", logging::LEVEL::DEBUG, true

using namespace client;

static std::string ROUTING = "kr";
static std::string SUMMONER_ID;
static std::string PUUID;
static std::string ACCOUNT_ID;

using json_ptr = std::unique_ptr<std::vector<char>>;


TEST_CASE( "ACCOUNT_V1 QUERIES") {
    std::cout << "TESTING ACCOUNT_V1 QUERIES" << '\n';
    RiotApiClient test_client(CONFIG);

    std::vector<std::string> region = {"AMERICAS", "ASIA", "EUROPE"};
    std::string puuid; // puuid is key specific
    std::string gamename = "DanTan1"; std::string tagline = "fresn";
    std::string game = "val";
    std::vector<std::string> end_types = {"by-riot-id", "by-puuid", "by-game"};

    std::string endpoint = "ACCOUNT-V1";
    simdjson::ondemand::parser parser;
    simdjson::ondemand::document doc;

    json_ptr result;

    for (int i = 0; i < 1; i++) {
        result = test_client.Account.by_riot_id(region[i], gamename, tagline);
        result->insert(result->end(),simdjson::SIMDJSON_PADDING, '\0');
        doc = parser.iterate(result->data(), strlen(result->data()), sizeof(result->data()));
        REQUIRE(doc["gameName"].get_string().value() == gamename);
        REQUIRE(doc["tagLine"].get_string().value() == tagline);
        puuid = doc["puuid"].get_string().value();
        result = test_client.Account.by_puuid(region[i], puuid);
        result->insert(result->end(),simdjson::SIMDJSON_PADDING, '\0');
        doc = parser.iterate(result->data(), strlen(result->data()), sizeof(result->data()));
        REQUIRE(doc["puuid"].get_string().value() == puuid);
        result = test_client.Account.by_game(region[i], game, puuid);
        result->insert(result->end(),simdjson::SIMDJSON_PADDING, '\0');
        doc = parser.iterate(result->data(), strlen(result->data()), sizeof(result->data()));
        REQUIRE(doc["puuid"].get_string().value() == puuid);
    }
}

// League of Legend Queries

TEST_CASE( "LEAGUE_V4 QUERIES") {
    std::cout << "TESTING LEAGUE_V4 QUERIES" << '\n';
    RiotApiClient test_client(CONFIG);

    std::string region = "na1";
    std::vector<std::string> queue = {"RANKED_SOLO_5x5", "RANKED_FLEX_SR"};
    std::vector<std::string> division = {"I", "II", "III", "IV"};
    std::vector<std::string> tier = {"DIAMOND", "PLATINUM", "GOLD", "SILVER", "BRONZE", "IRON"};
    std::string summoner_id;
    std::string league_id;

    std::string endpoint = "LEAGUE-V4";

    json_ptr result;
    simdjson::ondemand::parser parser;
    simdjson::ondemand::document doc;

    SECTION("CHALLENGER, GRANDMASTER AND MASTER QUEUE") {
        std::cout << "    CHALLENGER, GRANDMASTER AND MASTER" << '\n';
        for (int i = 0; i < queue.size(); i++) {
            result = test_client.League.challenger(region, queue.at(i));
            result->insert(result->end(),simdjson::SIMDJSON_PADDING, '\0');
            doc = parser.iterate(result->data(), strlen(result->data()), sizeof(result->data()));
            REQUIRE(doc["tier"].get_string().value() == "CHALLENGER");
            REQUIRE(doc["queue"].get_string().value() == queue[i]);
            result = test_client.League.grandmaster(region, queue.at(i));
            result->insert(result->end(),simdjson::SIMDJSON_PADDING, '\0');
            doc = parser.iterate(result->data(), strlen(result->data()), sizeof(result->data()));
            REQUIRE(doc["tier"].get_string().value() == "GRANDMASTER");
            REQUIRE(doc["queue"].get_string().value() == queue[i]);
            result = test_client.League.master(region, queue.at(i));
            result->insert(result->end(),simdjson::SIMDJSON_PADDING, '\0');
            doc = parser.iterate(result->data(), strlen(result->data()), sizeof(result->data()));
            REQUIRE(doc["tier"].get_string().value() == "MASTER");
            REQUIRE(doc["queue"].get_string().value() == queue[i]);
        }
    }
    result = test_client.League.master(region, queue.at(0));
    result->insert(result->end(),simdjson::SIMDJSON_PADDING, '\0');
    doc = parser.iterate(result->data(), strlen(result->data()), sizeof(result->data()));
    summoner_id = doc["entries"].at(0)["summonerId"].get_string().value();
    league_id = doc["leagueId"].get_string().value();

    SECTION("SPECIFIC QUEUE ") {
        std::cout << "    SPECIFIC QUEUE " << '\n';
        for (int qu = 0; qu < queue.size(); qu++) {
            for (int ti = 0; ti < tier.size(); ti++) {
                for (int div = 0; div < division.size(); div++) {
                    result = test_client.League.specific_league(region, queue.at(qu), tier.at(ti), division.at(div));
                    result->insert(result->end(),simdjson::SIMDJSON_PADDING, '\0');
                    doc = parser.iterate(result->data(), strlen(result->data()), sizeof(result->data()));
                    auto ref = doc.at(0);
                    REQUIRE(ref["queueType"].get_string().value() == queue[qu]);
                    REQUIRE(ref["tier"].get_string().value() == tier[ti]);
                    REQUIRE(ref["rank"].get_string().value() == division[div]);
                }
            }
        }
    }

    SECTION("Testing Summoner ID and League ID") {
        std::cout << "    SUMMONER ID AND LEAGUE ID " << '\n';
        result = test_client.League.by_summoner_id(region, summoner_id);
        result->insert(result->end(),simdjson::SIMDJSON_PADDING, '\0');
        doc = parser.iterate(result->data(), strlen(result->data()), sizeof(result->data()));
        REQUIRE(doc.at(0)["summonerId"].get_string().value() == summoner_id);
        result = test_client.League.by_league_id(region, league_id);
        result->insert(result->end(),simdjson::SIMDJSON_PADDING, '\0');
        doc = parser.iterate(result->data(), strlen(result->data()), sizeof(result->data()));
        REQUIRE(doc["leagueId"].get_string().value() == league_id);
    }
}

TEST_CASE(" SUMMONER QUERIES ") {
    std::cout << "TESTING  SUMMONER QUERIES " << '\n';
    RiotApiClient test_client(CONFIG);

    std::string region = "kr";
    std::string summoner_name = "Hide on bush";

    json_ptr result;
    simdjson::ondemand::parser parser;
    simdjson::ondemand::document doc;

    std::string endpoint = "SUMMONER-V4";

    result = test_client.Summoner.by_name(region, summoner_name);
    result->insert(result->end(),simdjson::SIMDJSON_PADDING, '\0');
    doc = parser.iterate(result->data(), strlen(result->data()), sizeof(result->data()));
    REQUIRE(doc["name"].get_string().value() == summoner_name);
    ACCOUNT_ID = doc["accountId"].get_string().value();
    PUUID = doc["puuid"].get_string().value();
    SUMMONER_ID = doc["id"].get_string().value();
    result = test_client.Summoner.by_account_id(region, ACCOUNT_ID);
    result->insert(result->end(),simdjson::SIMDJSON_PADDING, '\0');
    doc = parser.iterate(result->data(), strlen(result->data()), sizeof(result->data()));
    REQUIRE(doc["accountId"].get_string().value() == ACCOUNT_ID);
    result = test_client.Summoner.by_puuid(region, PUUID);
    result->insert(result->end(),simdjson::SIMDJSON_PADDING, '\0');
    doc = parser.iterate(result->data(), strlen(result->data()), sizeof(result->data()));
    REQUIRE(doc["puuid"].get_string().value() == PUUID);
    result = test_client.Summoner.by_summoner_id(region, SUMMONER_ID);
    result->insert(result->end(),simdjson::SIMDJSON_PADDING, '\0');
    doc = parser.iterate(result->data(), strlen(result->data()), sizeof(result->data()));
    REQUIRE(doc["id"].get_string().value() == SUMMONER_ID);

}

TEST_CASE( "MATCH QUERIES" ) {
    std::cout << "TESTING MATCH QUERIES" << '\n';
    RiotApiClient test_client(CONFIG);

    std::string region = "ASIA";
    std::string match_id = "KR_6279823690";

    json_ptr result;
    simdjson::ondemand::parser parser;
    simdjson::ondemand::document doc;
    std::string endpoint = "MATCH-V5";

    result = test_client.Match.by_match_id(region, match_id);
    result->insert(result->end(),simdjson::SIMDJSON_PADDING, '\0');
    doc = parser.iterate(result->data(), strlen(result->data()), sizeof(result->data()));
    REQUIRE(doc["metadata"]["matchId"].get_string().value() == match_id);
    result = test_client.Match.timeline(region, match_id);
    result->insert(result->end(),simdjson::SIMDJSON_PADDING, '\0');
    doc = parser.iterate(result->data(), strlen(result->data()), sizeof(result->data()));
    REQUIRE(doc["metadata"]["matchId"].get_string().value() == match_id);
    result = test_client.Match.by_puuid(region, PUUID, {"type","ranked"});
}

TEST_CASE("CHAMPION-MASTERY-V4 QUERIES") {
    std::cout << "TESTING CHAMPION-MASTERY-V4 QUERIES" << '\n';
    RiotApiClient test_client(CONFIG);

    const int champion_id = 1;
    std::string endpoint = "CHAMPION-MASTERY-V4";
    std::vector<std::pair<std::string, std::string>> optional_args = {{"count", "1"}};

    json_ptr result;
    simdjson::ondemand::parser parser;
    simdjson::ondemand::document doc;

    result = test_client.Champion_Mastery.by_summoner_id(ROUTING, SUMMONER_ID);
    result->insert(result->end(),simdjson::SIMDJSON_PADDING, '\0');
    doc = parser.iterate(result->data(), strlen(result->data()), sizeof(result->data()));
    REQUIRE(doc.at(0)["summonerId"].get_string().value() == SUMMONER_ID);
    result = test_client.Champion_Mastery.by_summoner_by_champion(ROUTING, SUMMONER_ID, champion_id);
    result->insert(result->end(),simdjson::SIMDJSON_PADDING, '\0');
    doc = parser.iterate(result->data(), strlen(result->data()), sizeof(result->data()));
    REQUIRE(doc["championId"].get_int64().value() == champion_id);
    REQUIRE(doc["summonerId"].get_string().value() == SUMMONER_ID);
    result = test_client.Champion_Mastery.by_summoner_top(ROUTING, SUMMONER_ID, {"count", 1});
    result->insert(result->end(),simdjson::SIMDJSON_PADDING, '\0');
    doc = parser.iterate(result->data(), strlen(result->data()), sizeof(result->data()));
    REQUIRE(doc.at(0)["summonerId"].get_string().value() == SUMMONER_ID);
    result = test_client.Champion_Mastery.scores_by_summoner(ROUTING, SUMMONER_ID);
}

TEST_CASE("CHAMPION-V3") {
    std::cout << "TESTING CHAMPION-V3" << '\n';
    RiotApiClient test_client(CONFIG);

    json_ptr result;
    simdjson::ondemand::parser parser;
    simdjson::ondemand::document doc;

    result = test_client.Champion.rotations(ROUTING);
    result->insert(result->end(),simdjson::SIMDJSON_PADDING, '\0');
    doc = parser.iterate(result->data(), strlen(result->data()), sizeof(result->data()));
    REQUIRE_NOTHROW(doc["freeChampionIds"]);
    REQUIRE_NOTHROW(doc["freeChampionIdsForNewPlayers"].get_int64());
}

TEST_CASE("LOL-CHALLENGES-V1") {
    std::cout << "TESTING LOL-CHALLENGES-V1" << '\n';
    RiotApiClient test_client(CONFIG);
    
    const int challenge_id = 1;
    std::string level = "HIGHEST";
    json_ptr result;
    simdjson::ondemand::parser parser;
    simdjson::ondemand::document doc;

    result = test_client.Lol_Challenges.config(ROUTING);
    result->insert(result->end(),simdjson::SIMDJSON_PADDING, '\0');
    doc = parser.iterate(result->data(), strlen(result->data()), sizeof(result->data()));
    REQUIRE_NOTHROW(doc.at(0)["id"]);
    REQUIRE_NOTHROW(doc.at(0)["localizedNames"]);
    result = test_client.Lol_Challenges.percentiles(ROUTING);
    result->insert(result->end(),simdjson::SIMDJSON_PADDING, '\0');
    doc = parser.iterate(result->data(), strlen(result->data()), sizeof(result->data()));
    REQUIRE_NOTHROW(doc["0"]);
    REQUIRE_NOTHROW(doc["1"]);
    result = test_client.Lol_Challenges.challenge_config(ROUTING, challenge_id);
    result->insert(result->end(),simdjson::SIMDJSON_PADDING, '\0');
    doc = parser.iterate(result->data(), strlen(result->data()), sizeof(result->data()));
    REQUIRE_NOTHROW(doc["id"]);
    REQUIRE_NOTHROW(doc["localizedNames"]);
//    result = test_client.Lol_Challenges.challenge_leaderboard(ROUTING, challenge_id, level); failing on riotswebstire as well?!
//    REQUIRE(result.isMember("BRONZE"));
//    REQUIRE(result.isMember("CHALLENGER"));
    result = test_client.Lol_Challenges.by_puuid(ROUTING, PUUID);
    result->insert(result->end(),simdjson::SIMDJSON_PADDING, '\0');
    doc = parser.iterate(result->data(), strlen(result->data()), sizeof(result->data()));
    REQUIRE_NOTHROW(doc["challenges"]);
    REQUIRE_NOTHROW(doc["preferences"]);
    REQUIRE_NOTHROW(doc["totalPoints"]);
    REQUIRE_NOTHROW(doc["categoryPoints"]);
}
TEST_CASE("LOL-STATUS") {
    std::cout << "TESTING LOL-STATUS" << '\n';
    RiotApiClient test_client(CONFIG);
    
    json_ptr result;
    simdjson::ondemand::parser parser;
    simdjson::ondemand::document doc;
    
    // not available for my development key
    // result = test_client.query(endpoint, std::string("v3"), std::vector<std::string>{ROUTING});
    result = test_client.Lol_Status.v4(ROUTING);
    result->insert(result->end(),simdjson::SIMDJSON_PADDING, '\0');
    doc = parser.iterate(result->data(), strlen(result->data()), sizeof(result->data()));
    REQUIRE(doc["id"].get_string().value() == "KR");
    REQUIRE_NOTHROW(doc["maintenances"]);
    REQUIRE_NOTHROW(doc["incidents"]);
    REQUIRE_NOTHROW(doc["locales"]);
}
//TEST_CASE("LOR-MATCH-V1") {
//std::cout << "TESTING LOR-MATCH-V1" << '\n';
//    RiotApiClient test_client(CONFIG);
//
//    json_ptr result;
//    std::string endpoint = "LOR-MATCH-V1";
//    std::string match_id;
//
//    result = test_client.Summoner.by_name("oc1", "mtucks");
//    std::string puuid = result["puuid"].asString();
//    result = test_client.Lor_Match.by_puuid("AMERICAS", puuid);
//    REQUIRE(result.isArray());
//    match_id = result[result.size() - 1].asString();
//    result = test_client.Lor_Match.by_match("AMERICAS", match_id);
//    REQUIRE(result["metadata"]["match_id"] == match_id);
//}
TEST_CASE("LOR-RANKED-V1") {
    std::cout << "TESTING LOR-RANKED-V1" << '\n';
    RiotApiClient test_client(CONFIG);

    json_ptr result;
    simdjson::ondemand::parser parser;
    simdjson::ondemand::document doc;

    result = test_client.Lor_Ranked.leaderboards("AMERICAS");
    result->insert(result->end(),simdjson::SIMDJSON_PADDING, '\0');
    doc = parser.iterate(result->data(), strlen(result->data()), sizeof(result->data()));
    REQUIRE_NOTHROW(doc["players"]);
    REQUIRE_NOTHROW(doc["players"].get_array());
}
TEST_CASE("LOR-STATUS-V1") {
    std::cout << "TESTING LOR-STATUS-V1" << '\n';
    RiotApiClient test_client(CONFIG);

    json_ptr result;
    simdjson::ondemand::parser parser;
    simdjson::ondemand::document doc;

    result = test_client.Lor_Status.v1("AMERICAS");
    result->insert(result->end(),simdjson::SIMDJSON_PADDING, '\0');
    doc = parser.iterate(result->data(), strlen(result->data()), sizeof(result->data()));
    REQUIRE(doc["id"].get_string().value() == "Americas");
    REQUIRE(doc["name"].get_string().value() == "Americas");
    REQUIRE_NOTHROW(doc["locales"].get_array());
    REQUIRE_NOTHROW(doc["maintenances"]);
    REQUIRE_NOTHROW(doc["incidents"]);
}
TEST_CASE("SPECTATOR-V4") {
    std::cout << "TESTING SPECTATOR-V4" << '\n';
    RiotApiClient test_client(CONFIG);

    json_ptr result;
    std::string endpoint = "SPECTATOR-V4";
    simdjson::ondemand::parser parser;
    simdjson::ondemand::document doc;

    result = test_client.Spectator.featured_games(ROUTING);
    result->insert(result->end(),simdjson::SIMDJSON_PADDING, '\0');
    doc = parser.iterate(result->data(), strlen(result->data()), sizeof(result->data()));
    INFO("JSON EXISTENCE CHECK: \"gameList\"");
    REQUIRE_NOTHROW(doc["gameList"]);
    INFO("JSON ARRAY CHECK");
    REQUIRE_NOTHROW(doc["gameList"].get_array());
    auto ref = doc["gameList"].at(0);
    INFO("JSON EXISTENCE CHECK: \"participants\"");
    REQUIRE_NOTHROW(ref["participants"]);
    std::string summoner_name;
    summoner_name = ref["participants"].at(0)["summonerName"].get_string().value();
    result = test_client.Summoner.by_name(ROUTING, summoner_name);
    result->insert(result->end(),simdjson::SIMDJSON_PADDING, '\0');
    doc = parser.iterate(result->data(), strlen(result->data()), sizeof(result->data()));
    std::string summoner_id;
    summoner_id = doc["id"].get_string().value();
    result = test_client.Spectator.by_summoner_id(ROUTING, summoner_id);
    result->insert(result->end(),simdjson::SIMDJSON_PADDING, '\0');
    doc = parser.iterate(result->data(), strlen(result->data()), sizeof(result->data()));
    INFO("JSON EXISTENCE CHECK: \"participants\"");
    REQUIRE_NOTHROW(doc["participants"]);
    INFO("JSON ARRAY CHECK");
    REQUIRE_NOTHROW(doc["participants"].get_array());
    auto foriter = doc["participants"];
    { // find participant in the game to see if the correct game was found
        bool participant_found = false;
        bool temp;
        for (simdjson::ondemand::object objjs : foriter.get_array()) {
            temp = objjs["summonerId"].get_string().value() == summoner_id;
            participant_found = participant_found || temp;
        }
        REQUIRE(participant_found);
    }
}
TEST_CASE("TFT-LEAGUE-V1") {
    std::cout << "TESTING TFT-LEAGUE-V1" << '\n';
    RiotApiClient test_client(CONFIG);

    json_ptr result;
    std::string endpoint = "TFT-LEAGUE-V1";
    simdjson::ondemand::parser parser;
    simdjson::ondemand::document doc;

    std::string summonerid;

    result = test_client.Tft_League.challenger(ROUTING);
    result->insert(result->end(),simdjson::SIMDJSON_PADDING, '\0');
    INFO("Validating JSON and padding from challenger");
    REQUIRE_NOTHROW(doc = parser.iterate(result->data(), strlen(result->data()), sizeof(result->data())));
    REQUIRE(doc["tier"].get_string().value() == "CHALLENGER");
    summonerid = doc["entries"].at(0)["summonerId"].get_string().value();

    result = test_client.Tft_League.grandmaster(ROUTING);
    result->insert(result->end(),simdjson::SIMDJSON_PADDING, '\0');
    INFO("Validating JSON and padding from grandmaster");
    REQUIRE_NOTHROW(doc = parser.iterate(result->data(), strlen(result->data()), sizeof(result->data())));
    REQUIRE(doc["tier"].get_string().value() == "GRANDMASTER");

    result = test_client.Tft_League.master(ROUTING);
    result->insert(result->end(),simdjson::SIMDJSON_PADDING, '\0');
    INFO("Validating JSON and padding from master");
    REQUIRE_NOTHROW(doc = parser.iterate(result->data(), strlen(result->data()), sizeof(result->data())));
    REQUIRE(doc["tier"].get_string().value() == "MASTER");

    result = test_client.Tft_League.by_summoner_id(ROUTING, summonerid);
    result->insert(result->end(),simdjson::SIMDJSON_PADDING, '\0');
    INFO("Validating JSON and padding from by summoner");
    REQUIRE_NOTHROW(doc = parser.iterate(result->data(), strlen(result->data()), sizeof(result->data())));
    
    auto arrobj = doc.at(0);
    INFO("JSON KEY EXISTENCE: \"summonerId\"");
    REQUIRE_NOTHROW(arrobj["summonerId"]);
    INFO("JSON KEY EXISTENCE: \"leagueId\"");
    REQUIRE_NOTHROW(arrobj["leagueId"]);
    INFO("CHECKING CORRECT QUERY");
    REQUIRE(arrobj["summonerId"].get_string().value() == summonerid);
    std::string league_id;
    league_id = arrobj["leagueId"].get_string().value();

    result = test_client.Tft_League.by_league_id(ROUTING, league_id);
    result->insert(result->end(),simdjson::SIMDJSON_PADDING, '\0');
    doc = parser.iterate(result->data(), strlen(result->data()), sizeof(result->data()));
    INFO("JSON KEY EXISTENCE: \"leagueId\"");
    REQUIRE_NOTHROW(doc["leagueId"]);
    REQUIRE(doc["leagueId"].get_string().value() == league_id);

    result = test_client.Tft_League.queue_top(ROUTING, "RANKED_TFT_TURBO");
    result->insert(result->end(),simdjson::SIMDJSON_PADDING, '\0');
    doc = parser.iterate(result->data(), strlen(result->data()), sizeof(result->data()));
    INFO("ASSERTING JSON RESPONSE IS ARRAY");
    REQUIRE_NOTHROW(doc.get_array());

    result = test_client.Tft_League.by_tier_division(ROUTING, "DIAMOND", "II", {"page", 2});
    result->insert(result->end(),simdjson::SIMDJSON_PADDING, '\0');
    doc = parser.iterate(result->data(), strlen(result->data()), sizeof(result->data()));
    auto jsonref = doc.at(0);
    INFO("JSON KEY EXISTENCE: \"tier\"");
    REQUIRE_NOTHROW(jsonref["tier"]);
    INFO("JSON KEY EXISTENCE: \"rank\"");
    REQUIRE_NOTHROW(jsonref["rank"]);
    INFO("CHECKING CORRECT QUERY");
    REQUIRE(jsonref["tier"].get_string().value() == "DIAMOND");
    REQUIRE(jsonref["rank"].get_string().value() == "II");
}
TEST_CASE("TFT-MATCH-V1") {
    std::cout << "TESTING TFT-MATCH-V1" << '\n';
    RiotApiClient test_client(CONFIG);

    json_ptr result;
    simdjson::ondemand::parser parser;
    simdjson::ondemand::document doc;

    std::string endpoint = "TFT-MATCH-V1";

    result = test_client.Summoner.by_name("oc1", "Monkeys R Us");
    result->insert(result->end(),simdjson::SIMDJSON_PADDING, '\0');
    doc = parser.iterate(result->data(), strlen(result->data()), sizeof(result->data()));
    std::string puuid;
    puuid = doc["puuid"].get_string().value();
    
    result = test_client.Tft_Match.by_puuid("AMERICAS", puuid, {"start", 5}, {"startTime", 0}, {"count", 20});
    result->insert(result->end(),simdjson::SIMDJSON_PADDING, '\0');
    doc = parser.iterate(result->data(), strlen(result->data()), sizeof(result->data()));
    REQUIRE(doc.count_elements().value() == 20);
    std::string match_id;
    match_id = doc.at(0).get_string().value();

    result = test_client.Tft_Match.by_match("AMERICAS", match_id);
    result->insert(result->end(),simdjson::SIMDJSON_PADDING, '\0');
    doc = parser.iterate(result->data(), strlen(result->data()), sizeof(result->data()));
    INFO("JSON KEY EXISTENCE: \"metadata\"");
    REQUIRE_NOTHROW(doc["metadata"]);
    INFO("JSON KEY EXISTENCE: \"info\"");
    REQUIRE_NOTHROW(doc["info"]);
    INFO("JSON KEY EXISTENCE: \"metadata|match_id\"");
    REQUIRE_NOTHROW(doc["metadata"]["match_id"]);
    INFO("CHECKING CORRECR QUERY");
    REQUIRE(doc["metadata"]["match_id"].get_string().value() == match_id);
}
TEST_CASE("TFT-STATUS-V1") {
    std::cout << "TESTING TFT-STATUS-V1" << '\n';
    RiotApiClient test_client(CONFIG);

    json_ptr result;
    simdjson::ondemand::parser parser;
    simdjson::ondemand::document doc;

    result = test_client.Tft_Status.v1(ROUTING);
    result->insert(result->end(),simdjson::SIMDJSON_PADDING, '\0');
    doc = parser.iterate(result->data(), strlen(result->data()), sizeof(result->data()));
    REQUIRE_NOTHROW(doc["id"]);
    REQUIRE(doc["id"].get_string().value() == "KR");

}
TEST_CASE("TFT-SUMMONER-V1") { 
    std::cout << "TESTING TFT-SUMMONER-V1" << '\n';
    RiotApiClient test_client(CONFIG);

    json_ptr result;
    simdjson::ondemand::parser parser;
    simdjson::ondemand::document doc;

    std::string summoner_name = "Monkeys R US";
    std::string reg = "oc1";

    result = test_client.Tft_Summoner.by_name(reg, summoner_name);
    result->insert(result->end(),simdjson::SIMDJSON_PADDING, '\0');
    doc = parser.iterate(result->data(), strlen(result->data()), sizeof(result->data()));
    REQUIRE_NOTHROW(doc["name"]);
    REQUIRE(doc["name"].get_string().value() == summoner_name);
    REQUIRE_NOTHROW(doc["id"]);
    REQUIRE_NOTHROW(doc["puuid"]);
    REQUIRE_NOTHROW(doc["accountId"]);
    std::string puuid; puuid = doc["puuid"].get_string().value();
    std::string summoner_id; summoner_id = doc["id"].get_string().value();
    std::string account_id; account_id = doc["accountId"].get_string().value();

    result = test_client.Tft_Summoner.by_account(reg, account_id);
    result->insert(result->end(),simdjson::SIMDJSON_PADDING, '\0');
    doc = parser.iterate(result->data(), strlen(result->data()), sizeof(result->data()));
    REQUIRE(doc["accountId"].value().get_string().value() == account_id);
    result = test_client.Tft_Summoner.by_puuid(reg, puuid);
    result->insert(result->end(),simdjson::SIMDJSON_PADDING, '\0');
    doc = parser.iterate(result->data(), strlen(result->data()), sizeof(result->data()));
    REQUIRE(doc["puuid"].value().get_string().value() == puuid);
    result = test_client.Tft_Summoner.by_summoner_id(reg, summoner_id);
    result->insert(result->end(),simdjson::SIMDJSON_PADDING, '\0');
    doc = parser.iterate(result->data(), strlen(result->data()), sizeof(result->data()));
    REQUIRE(doc["id"].get_string().value() == summoner_id);
}
TEST_CASE("VAL-CONTENT-V1") {
    std::cout << "TESTING VAL-CONTENT-V1" << '\n';
    RiotApiClient test_client(CONFIG);

    json_ptr result;
    simdjson::ondemand::parser parser;
    simdjson::ondemand::document doc;

    result = test_client.Val_Content.content(ROUTING);
    result->insert(result->end(),simdjson::SIMDJSON_PADDING, '\0');
    doc = parser.iterate(result->data(), strlen(result->data()), sizeof(result->data()));
    REQUIRE_NOTHROW(doc["version"]);
    REQUIRE_NOTHROW(doc["characters"]);
    REQUIRE_NOTHROW(doc["maps"]);
}
//// not available with my development
////TEST_CASE("VAL-MATCH-V1") {
////}
////TEST_CASE("VAL-RANKED-V1") {
////
////    std::string act_id = "0df5adb9-4dcb-6899-1306-3e9860661dd3";
////}
////TEST_CASE("VAL-STATUS-V1") {
////}
