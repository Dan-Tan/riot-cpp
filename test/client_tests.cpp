#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <ctime>
#include <queue>
#include <vector>
#include <string>
#include "../src/riot-cpp/client/client.h"
#include "json.hpp"

#define CONFIG "../../.api_keys/riot_config.json", "../test/log_file.txt", logging::LEVEL::DEBUG, true
namespace riotcpp {
using namespace client;

static std::string ROUTING = "KR";
static std::string SUMMONER_ID;
static std::string PUUID;
static std::string ACCOUNT_ID;

using json_ptr = std::unique_ptr<std::vector<char>>;
using json = nlohmann::json;

TEST_CASE( "ACCOUNT_V1 QUERIES") {
    std::cout << "TESTING ACCOUNT_V1 QUERIES" << '\n';
    RiotApiClient test_client(CONFIG);

    std::vector<std::string> region = {"AMERICAS", "ASIA", "EUROPE"};
    std::string puuid; // puuid is key specific
    std::string gamename = "Hide on bush"; std::string tagline = "KR1";
    std::string game = "val";
    std::vector<std::string> end_types = {"by-riot-id", "by-puuid", "by-game"};

    std::string endpoint = "ACCOUNT-V1";
    json doc;


    json_ptr result;

    for (int i = 0; i < 1; i++) {
        result = test_client.Account.by_riot_id(region[i], gamename, tagline);
        doc = json::parse(result->data());
        REQUIRE(doc["gameName"] == gamename);
        REQUIRE(doc["tagLine"] == tagline);

        PUUID = doc["puuid"];

        puuid = doc["puuid"];

        result = test_client.Account.by_puuid(region[i], puuid);
        doc = json::parse(result->data());
        REQUIRE(doc["puuid"] == puuid);

        result = test_client.Account.by_game_by_puuid(region[i], game, puuid);
        doc = json::parse(result->data());
        REQUIRE(doc["puuid"] == puuid);
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
    json doc;

    SECTION("CHALLENGER, GRANDMASTER AND MASTER QUEUE") {
        std::cout << "    CHALLENGER, GRANDMASTER AND MASTER" << '\n';
        for (int i = 0; i < queue.size(); i++) {

            result = test_client.League.challenger(region, queue.at(i));
            doc = json::parse(result->data());
            REQUIRE(doc["tier"] == "CHALLENGER");
            REQUIRE(doc["queue"] == queue[i]);

            result = test_client.League.grandmaster(region, queue.at(i));
            doc = json::parse(result->data());
            REQUIRE(doc["tier"] == "GRANDMASTER");
            REQUIRE(doc["queue"] == queue[i]);

            result = test_client.League.master(region, queue.at(i));
            doc = json::parse(result->data());
            REQUIRE(doc["tier"] == "MASTER");
            REQUIRE(doc["queue"] == queue[i]);
        }
    }

    result = test_client.League.master(region, queue.at(0));
    doc = json::parse(result->data());
    summoner_id = doc["entries"].at(0)["summonerId"];
    league_id = doc["leagueId"];

    SECTION("SPECIFIC QUEUE ") {
        std::cout << "    SPECIFIC QUEUE " << '\n';
        for (int qu = 0; qu < queue.size(); qu++) {
            for (int ti = 0; ti < tier.size(); ti++) {
                for (int div = 0; div < division.size(); div++) {

                    result = test_client.League.entries(region, queue.at(qu), tier.at(ti), division.at(div));
                    doc = json::parse(result->data());
                    auto ref = doc.at(0);
                    REQUIRE(ref["queueType"] == queue[qu]);
                    REQUIRE(ref["tier"] == tier[ti]);
                    REQUIRE(ref["rank"] == division[div]);
                }
            }
        }
    }

    SECTION("Testing Summoner ID and League ID") {
        std::cout << "    SUMMONER ID AND LEAGUE ID " << '\n';

        result = test_client.League.by_summoner_id(region, summoner_id);
        doc = json::parse(result->data());
        REQUIRE(doc.at(0)["summonerId"] == summoner_id);

        result = test_client.League.by_league_id(region, league_id);
        doc = json::parse(result->data());
        REQUIRE(doc["leagueId"] == league_id);
    }
}

TEST_CASE(" SUMMONER QUERIES ") {
    std::cout << "TESTING  SUMMONER QUERIES " << '\n';
    RiotApiClient test_client(CONFIG);

    std::string region = "kr";
    std::string summoner_name = "Hide on bush";

    json_ptr result;
    json doc;

    std::string endpoint = "SUMMONER-V4";

    result = test_client.Summoner.by_puuid(region, PUUID);
    doc = json::parse(result->data());
    REQUIRE(doc["puuid"] == PUUID);

    ACCOUNT_ID = doc["accountId"];
    SUMMONER_ID = doc["id"];

    result = test_client.Summoner.by_account_id(region, ACCOUNT_ID);
    doc = json::parse(result->data());
    REQUIRE(doc["accountId"] == ACCOUNT_ID);

    result = test_client.Summoner.by_summoner_id(region, SUMMONER_ID);
    doc = json::parse(result->data());
    REQUIRE(doc["id"] == SUMMONER_ID);

}

TEST_CASE( "MATCH QUERIES" ) {
    std::cout << "TESTING MATCH QUERIES" << '\n';
    RiotApiClient test_client(CONFIG);

    std::string region = "ASIA";
    std::string match_id = "KR_6279823690";
    json_ptr result;
    json doc;

    std::string endpoint = "MATCH-V5";
    result = test_client.Match.by_puuid(region, PUUID, {.type = "ranked"});
    doc = json::parse(result->data());
    match_id = doc.at(0);
    result = test_client.Match.by_match_id(region, match_id);
    doc = json::parse(result->data());
    REQUIRE(doc["metadata"]["matchId"] == match_id);
    result = test_client.Match.timeline_by_match_id(region, match_id);
    doc = json::parse(result->data());
    REQUIRE(doc["metadata"]["matchId"] == match_id);
}

TEST_CASE("CHAMPION-MASTERY-V4 QUERIES") {
    std::cout << "TESTING CHAMPION-MASTERY-V4 QUERIES" << '\n';
    RiotApiClient test_client(CONFIG);

    const int champion_id = 1;
    std::string endpoint = "CHAMPION-MASTERY-V4";
    std::vector<std::pair<std::string, std::string>> optional_args = {{"count", "1"}};

    json_ptr result;
    json doc;

    result = test_client.League.entries("KR", "RANKED_SOLO_5x5", "DIAMOND", "I");
    doc = json::parse(result->data());
    std::string summonerId; summonerId = doc.at(0)["summonerId"];

    result = test_client.Summoner.by_summoner_id("KR", summonerId);
    doc = json::parse(result->data());
    std::string puuid; 
    puuid = doc["puuid"];

    result = test_client.Champion_Mastery.by_puuid(ROUTING, puuid);
    doc = json::parse(result->data());
    INFO("puuid Used: " + puuid);
    std::string res = doc.at(0)["puuid"];
    REQUIRE(res == puuid);

    result = test_client.Champion_Mastery.by_puuid_by_champion(ROUTING, puuid, champion_id);
    doc = json::parse(result->data());
    INFO(result->data());
    REQUIRE(doc["championId"] == champion_id);
    INFO(result->data());
    res = doc["puuid"];
    REQUIRE(res == puuid);
    result = test_client.Champion_Mastery.scores_by_puuid(ROUTING, puuid);
}

TEST_CASE("CHAMPION-V3") {
    std::cout << "TESTING CHAMPION-V3" << '\n';
    RiotApiClient test_client(CONFIG);

    json_ptr result;
    json doc;

    result = test_client.Champion.champion_rotations(ROUTING);
    doc = json::parse(result->data());
    REQUIRE_NOTHROW(doc.at("freeChampionIds"));
    REQUIRE_NOTHROW(doc.at("freeChampionIdsForNewPlayers"));
}

TEST_CASE("LOL-CHALLENGES-V1") {
    std::cout << "TESTING LOL-CHALLENGES-V1" << '\n';
    RiotApiClient test_client(CONFIG);
    
    const int challenge_id = 1;
    std::string level = "HIGHEST";
    json_ptr result;
    json doc;

    result = test_client.Lol_Challenges.config(ROUTING);
    doc = json::parse(result->data());
    REQUIRE_NOTHROW(doc.at(0).at("id"));
    REQUIRE_NOTHROW(doc.at(0).at("localizedNames"));

    result = test_client.Lol_Challenges.percentiles(ROUTING);
    doc = json::parse(result->data());
    REQUIRE_NOTHROW(doc.at("0"));
    REQUIRE_NOTHROW(doc.at("1"));

    result = test_client.Lol_Challenges.config_by_id(ROUTING, challenge_id);
    doc = json::parse(result->data());
    REQUIRE_NOTHROW(doc.at("id"));
    REQUIRE_NOTHROW(doc.at("localizedNames"));
//    result = test_client.Lol_Challenges.challenge_leaderboard(ROUTING, challenge_id, level); failing on riotswebstire as well?!
//    REQUIRE(result.isMember("BRONZE"));
//    REQUIRE(result.isMember("CHALLENGER"));
    result = test_client.Lol_Challenges.player_data_by_puuid(ROUTING, PUUID);
    doc = json::parse(result->data());
    REQUIRE_NOTHROW(doc.at("challenges"));
    REQUIRE_NOTHROW(doc.at("preferences"));
    REQUIRE_NOTHROW(doc.at("totalPoints"));
    REQUIRE_NOTHROW(doc.at("categoryPoints"));
}
TEST_CASE("LOL-STATUS") {
    std::cout << "TESTING LOL-STATUS" << '\n';
    RiotApiClient test_client(CONFIG);
    
    json_ptr result;
    json doc;
    
    // not available for my development key
    // result = test_client.query(endpoint, std::string("v3"), std::vector<std::string>{ROUTING});
    result = test_client.Lol_Status.status(ROUTING);
    doc = json::parse(result->data());
    REQUIRE(doc["id"] == "KR");
    REQUIRE_NOTHROW(doc.at("maintenances"));
    REQUIRE_NOTHROW(doc.at("incidents"));
    REQUIRE_NOTHROW(doc.at("locales"));
}
TEST_CASE("LOR-MATCH-V1") {
std::cout << "TESTING LOR-MATCH-V1" << '\n';
    RiotApiClient test_client(CONFIG);

    json_ptr result;
    std::string endpoint = "LOR-MATCH-V1";
    std::string match_id;
    json doc;

    result = test_client.Account.by_riot_id("AMERICAS", "Monkeys R Us", "fresn");
    doc = json::parse(result->data());
    std::string puuid; puuid = doc["puuid"];

    result = test_client.Lor_Match.by_puuid("AMERICAS", puuid);
    doc = json::parse(result->data());
    REQUIRE_NOTHROW(doc.at(0));
    match_id = doc.at(1);
    result = test_client.Lor_Match.by_match_id("AMERICAS", match_id);
    // Haven't played enough recently
    // REQUIRE(result["metadata"]["match_id"] == match_id);
}
TEST_CASE("LOR-RANKED-V1") {
    std::cout << "TESTING LOR-RANKED-V1" << '\n';
    RiotApiClient test_client(CONFIG);

    json_ptr result;
    json doc;

    result = test_client.Lor_Ranked.leaderboards("AMERICAS");
    doc = json::parse(result->data());
    REQUIRE_NOTHROW(doc.at("players"));
    REQUIRE(doc["players"].is_array());
}
TEST_CASE("LOR-STATUS-V1") {
    std::cout << "TESTING LOR-STATUS-V1" << '\n';
    RiotApiClient test_client(CONFIG);

    json_ptr result;
    json doc;

    result = test_client.Lor_Status.status("AMERICAS");
    doc = json::parse(result->data());
    REQUIRE(doc["id"] == "Americas");
    REQUIRE(doc["name"] == "Americas");
    REQUIRE(doc["locales"].is_array());
    REQUIRE_NOTHROW(doc.at("maintenances"));
    REQUIRE_NOTHROW(doc.at("incidents"));
}
TEST_CASE("SPECTATOR-V4") {
    std::cout << "TESTING SPECTATOR-V4" << '\n';
    RiotApiClient test_client(CONFIG);

    json_ptr result;
    std::string endpoint = "SPECTATOR-V4";
    json doc;

    result = test_client.Spectator.featured(ROUTING);
    doc = json::parse(result->data());
    INFO("JSON EXISTENCE CHECK: \"gameList\"");
    REQUIRE_NOTHROW(doc.at("gameList"));
    INFO("JSON ARRAY CHECK");
    REQUIRE(doc["gameList"].is_array());
    auto ref = doc["gameList"].at(0);
    INFO("JSON EXISTENCE CHECK: \"participants\"");
    REQUIRE_NOTHROW(ref.at("participants"));
    std::string puuid = ref["participants"].at(0)["puuid"];
    result = test_client.Summoner.by_puuid(ROUTING, puuid);
    doc = json::parse(result->data());
    puuid = doc["puuid"];
    result = test_client.Spectator.by_summoner(ROUTING, puuid);
    doc = json::parse(result->data());
    INFO("JSON EXISTENCE CHECK: \"participants\"");
    REQUIRE_NOTHROW(doc.at("participants"));
    INFO("JSON ARRAY CHECK");
    REQUIRE(doc["participants"].is_array());
    auto foriter = doc["participants"];
    { // find participant in the game to see if the correct game was found
        bool participant_found = false;
        bool temp;
        for (auto& objjs : foriter) {
            temp = objjs["puuid"] == puuid;
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
    json doc;

    std::string summonerid;

    result = test_client.Tft_League.challenger(ROUTING);
    doc = json::parse(result->data());
    REQUIRE(doc["tier"] == "CHALLENGER");
    summonerid = doc["entries"].at(0)["summonerId"];

    result = test_client.Tft_League.grandmaster(ROUTING);
    doc = json::parse(result->data());
    REQUIRE(doc["tier"] == "GRANDMASTER");

    result = test_client.Tft_League.master(ROUTING);
    doc = json::parse(result->data());
    REQUIRE(doc["tier"] == "MASTER");

    result = test_client.Tft_League.by_summoner(ROUTING, summonerid);
    doc = json::parse(result->data());
    
    auto arrobj = doc.at(0);
    INFO("JSON KEY EXISTENCE: \"summonerId\"");
    REQUIRE_NOTHROW(arrobj.at("summonerId"));
    INFO("JSON KEY EXISTENCE: \"leagueId\"");
    REQUIRE_NOTHROW(arrobj.at("leagueId"));
    INFO("CHECKING CORRECT QUERY");
    REQUIRE(arrobj["summonerId"] == summonerid);
    std::string league_id;
    league_id = arrobj["leagueId"];

    result = test_client.Tft_League.by_league_id(ROUTING, league_id);
    doc = json::parse(result->data());
    INFO("JSON KEY EXISTENCE: \"leagueId\"");
    REQUIRE_NOTHROW(doc.at("leagueId"));
    REQUIRE(doc["leagueId"] == league_id);

    result = test_client.Tft_League.top_by_queue(ROUTING, "RANKED_TFT_TURBO");
    doc = json::parse(result->data());
    INFO("ASSERTING JSON RESPONSE IS ARRAY");
    REQUIRE(doc.is_array());

    result = test_client.Tft_League.entries(ROUTING, "DIAMOND", "II", {.queue = "RANKED_TFT", .page = 2});
    doc = json::parse(result->data());
    auto jsonref = doc.at(0);
    INFO("JSON KEY EXISTENCE: \"tier\"");
    REQUIRE_NOTHROW(jsonref.at("tier"));
    INFO("JSON KEY EXISTENCE: \"rank\"");
    REQUIRE_NOTHROW(jsonref.at("rank"));
    INFO("CHECKING CORRECT QUERY");
    REQUIRE(jsonref["tier"] == "DIAMOND");
    REQUIRE(jsonref["rank"] == "II");
}
TEST_CASE("TFT-MATCH-V1") {
    std::cout << "TESTING TFT-MATCH-V1" << '\n';
    RiotApiClient test_client(CONFIG);

    json_ptr result;
    json doc;

    std::string endpoint = "TFT-MATCH-V1";

    result = test_client.Account.by_riot_id("AMERICAS", "Monkeys R Us", "fresn");
    doc = json::parse(result->data());
    std::string puuid;
    puuid = doc["puuid"];
    
    result = test_client.Tft_Match.by_puuid("AMERICAS", puuid, {.startTime = 0, .start = 5, .count = 20});
    doc = json::parse(result->data());
    REQUIRE(doc.is_array());
    REQUIRE(doc.size() == 20);
    std::string match_id = doc.at(0);

    result = test_client.Tft_Match.by_match_id("AMERICAS", match_id);
    doc = json::parse(result->data());
    if (doc.contains("status") && doc["status"]["status_code"] == 404) {
        INFO("404: Data not found error. Possible known problem with RIOT API.");
        REQUIRE(false);
    } else {
        INFO("JSON KEY EXISTENCE: \"metadata\"");
        REQUIRE_NOTHROW(doc.at("metadata"));
        INFO("JSON KEY EXISTENCE: \"info\"");
        REQUIRE_NOTHROW(doc.at("info"));
        INFO("JSON KEY EXISTENCE: \"metadata|match_id\"");
        REQUIRE_NOTHROW(doc.at("metadata").at("match_id"));
        INFO("CHECKING CORRECR QUERY");
        REQUIRE(doc["metadata"]["match_id"] == match_id);
    }
}
TEST_CASE("TFT-STATUS-V1") {
    std::cout << "TESTING TFT-STATUS-V1" << '\n';
    RiotApiClient test_client(CONFIG);

    json_ptr result;
    json doc;

    result = test_client.Tft_Status.status(ROUTING);
    doc = json::parse(result->data());
    REQUIRE_NOTHROW(doc.at("id"));
    REQUIRE(doc["id"] == "KR");

}
TEST_CASE("TFT-SUMMONER-V1") { 
    std::cout << "TESTING TFT-SUMMONER-V1" << '\n';
    RiotApiClient test_client(CONFIG);

    json_ptr result;
    json doc;

    std::string summoner_name = "Monkeys R US";
    std::string reg = "oc1";

    result = test_client.Account.by_riot_id("AMERICAS", "Monkeys R Us", "fresn");
    doc = json::parse(result->data());
    std::string puuid; puuid = doc["puuid"];
    result = test_client.Tft_Summoner.by_puuid("oc1", puuid);
    doc = json::parse(result->data());
    REQUIRE_NOTHROW(doc.at("id"));
    REQUIRE_NOTHROW(doc.at("puuid"));
    REQUIRE_NOTHROW(doc.at("accountId"));
    REQUIRE(doc["puuid"] == puuid);
    puuid = doc["puuid"];
    std::string summoner_id = doc["id"];
    std::string account_id = doc["accountId"];

    result = test_client.Tft_Summoner.by_account_id(reg, account_id);
    doc = json::parse(result->data());
    REQUIRE(doc["accountId"] == account_id);

    result = test_client.Tft_Summoner.by_summoner_id(reg, summoner_id);
    doc = json::parse(result->data());
    REQUIRE(doc["id"] == summoner_id);
}
TEST_CASE("VAL-CONTENT-V1") {
    std::cout << "TESTING VAL-CONTENT-V1" << '\n';
    RiotApiClient test_client(CONFIG);

    json_ptr result;
    json doc;

    result = test_client.Val_Content.contents("AP");
    doc = json::parse(result->data());
    REQUIRE_NOTHROW(doc.at("version"));
    REQUIRE_NOTHROW(doc.at("characters"));
}
}
// not available with my development key
//TEST_CASE("VAL-MATCH-V1") {
//}
//TEST_CASE("VAL-RANKED-V1") {
//
//    std::string act_id = "0df5adb9-4dcb-6899-1306-3e9860661dd3";
//}
//TEST_CASE("VAL-STATUS-V1") {
//}
