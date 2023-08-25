#include <catch2/catch_test_macros.hpp>
#include <ctime>
#include <queue>
#include <vector>
#include <string>
#include "../src/client/client.h"
#include "../src/jsoncpp/include/json/json.h"

#define CONFIG "../../.api_keys/riot_config.json", "../test/log_file.txt", logging::LEVEL::DEBUG

using namespace client;

static std::string ROUTING = "kr";
static std::string SUMMONER_ID;
static std::string PUUID;
static std::string ACCOUNT_ID;


TEST_CASE( "ACCOUNT_V1 QUERIES") {
    RiotApiClient test_client(CONFIG);

    std::vector<std::string> region = {"AMERICAS", "ASIA", "EUROPE"};
    std::string puuid; // puuid is key specific
    std::string gamename = "DanTan1"; std::string tagline = "fresn";
    std::string game = "val";
    std::vector<std::string> end_types = {"by-riot-id", "by-puuid", "by-game"};

    std::string endpoint = "ACCOUNT-V1";

    Json::Value result;
    
    for (int i = 0; i < 1; i++) {
        result = test_client.Account.by_riot_id(region[i], gamename, tagline);
        REQUIRE(result["gameName"] == gamename);
        REQUIRE(result["tagLine"] == tagline);
        puuid = result["puuid"].asString();
        result = test_client.Account.by_puuid(region[i], puuid);
        REQUIRE(result["puuid"] == puuid);
        result = test_client.Account.by_game(region[i], game, puuid);
        REQUIRE(result["puuid"] == puuid);
    }
}

// League of Legend Queries

TEST_CASE( "LEAGUE_V4 QUERIES") {
    RiotApiClient test_client(CONFIG);
    
    std::string region = "na1";
    std::vector<std::string> queue = {"RANKED_SOLO_5x5", "RANKED_FLEX_SR"};
    std::vector<std::string> division = {"I", "II", "III", "IV"};
    std::vector<std::string> tier = {"DIAMOND", "PLATINUM", "GOLD", "SILVER", "BRONZE", "IRON"};
    std::string summoner_id;
    std::string league_id;

    std::string endpoint = "LEAGUE-V4";

    Json::Value result;

    SECTION("CHALLENGER, GRANDMASTER AND MASTER QUEUE") {
        for (int i = 0; i < queue.size(); i++) {
            result = test_client.League.challenger(region, queue.at(i));
            REQUIRE(result["tier"] == "CHALLENGER");
            REQUIRE(result["queue"] == queue[i]);
            result = test_client.League.grandmaster(region, queue.at(i));
            REQUIRE(result["tier"] == "GRANDMASTER");
            REQUIRE(result["queue"] == queue[i]);
            result = test_client.League.master(region, queue.at(i));
            REQUIRE(result["tier"] == "MASTER");
            REQUIRE(result["queue"] == queue[i]);
        }
    }
    result = test_client.League.master(region, queue.at(0));
    summoner_id = result["entries"][0]["summonerId"].asString();
    league_id = result["leagueId"].asString();

    Json::StreamWriterBuilder builder;

    SECTION("SPECIFIC QUEUE ") {
        for (int qu = 0; qu < queue.size(); qu++) {
            for (int ti = 0; ti < tier.size(); ti++) {
                for (int div = 0; div < division.size(); div++) {
                    result = test_client.League.specific_league(region, queue.at(qu), tier.at(ti), division.at(div));
                    REQUIRE(result[0]["queueType"] == queue[qu]);
                    REQUIRE(result[0]["tier"] == tier[ti]);
                    REQUIRE(result[0]["rank"] == division[div]);
                }
            }
        }
    }

    SECTION("Testing Summoner ID and League ID") {
        result = test_client.League.by_summoner_id(region, summoner_id);
        REQUIRE(result[0]["summonerId"] == summoner_id);
        result = test_client.League.by_league_id(region, league_id);
        REQUIRE(result["leagueId"] == league_id);
    }
}

TEST_CASE(" SUMMONER QUERIES ") {
    RiotApiClient test_client(CONFIG);
    
    std::string region = "kr";
    std::string summoner_name = "Hide on bush";

    Json::Value result;

    std::string endpoint = "SUMMONER-V4";

    result = test_client.Summoner.by_name(region, summoner_name);
    REQUIRE(result["name"] == summoner_name);
    ACCOUNT_ID = result["accountId"].asString();
    PUUID = result["puuid"].asString();
    SUMMONER_ID = result["id"].asString();
    result = test_client.Summoner.by_account_id(region, ACCOUNT_ID);
    REQUIRE(result["accountId"] == ACCOUNT_ID);
    result = test_client.Summoner.by_puuid(region, PUUID);
    REQUIRE(result["puuid"] == PUUID);
    result = test_client.Summoner.by_summoner_id(region, SUMMONER_ID);
    REQUIRE(result["id"] == SUMMONER_ID);

}

TEST_CASE( "MATCH QUERIES" ) {
    RiotApiClient test_client(CONFIG);

    std::string region = "ASIA";
    std::string match_id = "KR_6279823690";

    Json::Value result;
    std::string endpoint = "MATCH-V5";

    result = test_client.Match.by_match_id(region, match_id);
    REQUIRE(result["metadata"]["matchId"] == match_id);
    result = test_client.Match.timeline(region, match_id);
    REQUIRE(result["metadata"]["matchId"] == match_id);
    result = test_client.Match.by_puuid(region, PUUID, {"type","ranked"});
}

TEST_CASE("CHAMPION-MASTERY-V4 QUERIES") {
    RiotApiClient test_client(CONFIG);
    
    const int champion_id = 1;
    std::string endpoint = "CHAMPION-MASTERY-V4";
    std::vector<std::pair<std::string, std::string>> optional_args = {{"count", "1"}};

    Json::Value result;

    result = test_client.Champion_Mastery.by_summoner_id(ROUTING, SUMMONER_ID);
    REQUIRE(result[0]["summonerId"] == SUMMONER_ID);
    result = test_client.Champion_Mastery.by_summoner_by_champion(ROUTING, SUMMONER_ID, champion_id);
    REQUIRE(result["championId"] == champion_id);
    REQUIRE(result["summonerId"] == SUMMONER_ID);
    result = test_client.Champion_Mastery.by_summoner_top(ROUTING, SUMMONER_ID, {"count", 1});
    REQUIRE(result[0]["summonerId"] == SUMMONER_ID);
    result = test_client.Champion_Mastery.scores_by_summoner(ROUTING, SUMMONER_ID);
}

TEST_CASE("CHAMPION-V3") {
    RiotApiClient test_client(CONFIG);
    
    Json::Value result;

    result = test_client.Champion.rotations(ROUTING);
    REQUIRE(result.isMember("freeChampionIds"));
    REQUIRE(result.isMember("freeChampionIdsForNewPlayers"));
}

TEST_CASE("LOL-CHALLENGES-V1") {
    RiotApiClient test_client(CONFIG);
    
    const int challenge_id = 1;
    std::string level = "HIGHEST";
    Json::Value result;

    result = test_client.Lol_Challenges.config(ROUTING);
    REQUIRE(result[0].isMember("id"));
    REQUIRE(result[0].isMember("localizedNames"));
    result = test_client.Lol_Challenges.percentiles(ROUTING);
    REQUIRE(result.isMember("0"));
    REQUIRE(result.isMember("1"));
    result = test_client.Lol_Challenges.challenge_config(ROUTING, challenge_id);
    REQUIRE(result.isMember("id"));
    REQUIRE(result.isMember("localizedNames"));
//    result = test_client.Lol_Challenges.challenge_leaderboard(ROUTING, challenge_id, level); failing on riotswebstire as well?!
//    REQUIRE(result.isMember("BRONZE"));
//    REQUIRE(result.isMember("CHALLENGER"));
    result = test_client.Lol_Challenges.by_puuid(ROUTING, PUUID);
    REQUIRE(result.isMember("challenges"));
    REQUIRE(result.isMember("preferences"));
    REQUIRE(result.isMember("totalPoints"));
    REQUIRE(result.isMember("categoryPoints"));
}
TEST_CASE("LOL-STATUS") {
    RiotApiClient test_client(CONFIG);
    
    Json::Value result;
    
    // not available for my development key
    // result = test_client.query(endpoint, std::string("v3"), std::vector<std::string>{ROUTING});
    result = test_client.Lol_Status.v4(ROUTING);
    REQUIRE(result["id"] == "KR");
    REQUIRE(result.isMember("maintenances"));
    REQUIRE(result.isMember("incidents"));
    REQUIRE(result.isMember("locales"));
}
//TEST_CASE("LOR-MATCH-V1") {
//    RiotApiClient test_client(CONFIG);
//
//    Json::Value result;
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
    RiotApiClient test_client(CONFIG);

    Json::Value result;

    result = test_client.Lor_Ranked.leaderboards("AMERICAS");
    REQUIRE(result.isMember("players"));
    REQUIRE(result["players"].isArray());
}
TEST_CASE("LOR-STATUS-V1") {
    RiotApiClient test_client(CONFIG);

    Json::Value result;

    result = test_client.Lor_Status.v1("AMERICAS");
    REQUIRE(result["id"].asString() == "Americas");
    REQUIRE(result["name"].asString() == "Americas");
    REQUIRE(result["locales"].isArray());
    REQUIRE(result.isMember("maintenances"));
    REQUIRE(result.isMember("incidents"));
}
TEST_CASE("SPECTATOR-V4") {
    RiotApiClient test_client(CONFIG);

    Json::Value result;
    std::string endpoint = "SPECTATOR-V4";

    result = test_client.Spectator.featured_games(ROUTING);
    REQUIRE(result.isMember("gameList"));
    REQUIRE(result["gameList"].isArray());
    REQUIRE(result["gameList"][0].isMember("participants"));
    std::string summoner_name = result["gameList"][0]["participants"][0]["summonerName"].asString();
    result = test_client.Summoner.by_name(ROUTING, summoner_name);
    std::string summoner_id = result["id"].asString();
    result = test_client.Spectator.by_summoner_id(ROUTING, summoner_id);
    REQUIRE(result.isMember("participants"));
    REQUIRE(result["participants"].isArray());
    REQUIRE(result["participants"][0].isMember("summonerId"));
    { // find participant in the game to see if the correct game was found
        bool participant_found = false;
        bool temp;
        for (Json::Value::ArrayIndex i = 0; i != result["participants"].size(); i++) {
            temp = result["participants"][i]["summonerId"].asString() == summoner_id;
            participant_found = participant_found || temp;
        }
        REQUIRE(participant_found);
    }
}
TEST_CASE("TFT-LEAGUE-V1") {
    RiotApiClient test_client(CONFIG);

    Json::Value result;
    std::string endpoint = "TFT-LEAGUE-V1";

    result = test_client.Tft_League.challenger(ROUTING);
    result = test_client.Tft_League.grandmaster(ROUTING);
    result = test_client.Tft_League.master(ROUTING);
    result = test_client.Tft_League.by_summoner_id(ROUTING, SUMMONER_ID);
    REQUIRE(result.isArray());
    REQUIRE(result[0].isMember("summonerId"));
    REQUIRE(result[0].isMember("leagueId"));
    REQUIRE(result[0]["summonerId"].asString() == SUMMONER_ID);
    std::string league_id = result[0]["leagueId"].asString();
    result = test_client.Tft_League.by_league_id(ROUTING, league_id);
    REQUIRE(result.isMember("leagueId"));
    REQUIRE(result["leagueId"] == league_id);
    result = test_client.Tft_League.queue_top(ROUTING, "RANKED_TFT_TURBO");
    REQUIRE(result.isArray());
    result = test_client.Tft_League.by_tier_division(ROUTING, "DIAMOND", "II", {"page", 2});
    REQUIRE(result.isArray());
    REQUIRE(result[0].isMember("tier"));
    REQUIRE(result[0].isMember("rank"));
    REQUIRE(result[0]["tier"] == "DIAMOND");
    REQUIRE(result[0]["rank"] == "II");
}
TEST_CASE("TFT-MATCH-V1") {
    RiotApiClient test_client(CONFIG);

    Json::Value result;
    std::string endpoint = "TFT-MATCH-V1";

    result = test_client.Summoner.by_name("oc1", "Monkeys R Us");
    std::string puuid = result["puuid"].asString();
    
    result = test_client.Tft_Match.by_puuid("AMERICAS", puuid, {"start", 5}, {"startTime", 0}, {"count", 20});
    REQUIRE(result.isArray());
    REQUIRE(result.size() == 20);
    std::string match_id = result[0].asString();
    result = test_client.Tft_Match.by_match("AMERICAS", match_id);
    REQUIRE(result.isMember("metadata"));
    REQUIRE(result.isMember("info"));
    REQUIRE(result["metadata"].isMember("match_id"));
    REQUIRE(result["metadata"]["match_id"].asString() == match_id);
}
TEST_CASE("TFT-STATUS-V1") {
    RiotApiClient test_client(CONFIG);

    Json::Value result;

    result = test_client.Tft_Status.v1(ROUTING);
    REQUIRE(result.isMember("id"));
    REQUIRE(result["id"].asString() == "KR");

}
TEST_CASE("TFT-SUMMONER-V1") { 
    RiotApiClient test_client(CONFIG);

    Json::Value result;

    std::string summoner_name = "Monkeys R US";
    std::string reg = "oc1";

    result = test_client.Tft_Summoner.by_name(reg, summoner_name);
    REQUIRE(result.isMember("name"));
    REQUIRE(result["name"] == summoner_name);
    REQUIRE(result.isMember("id"));
    REQUIRE(result.isMember("puuid"));
    REQUIRE(result.isMember("accountId"));
    std::string puuid = result["puuid"].asString();
    std::string summoner_id = result["id"].asString();
    std::string account_id = result["accountId"].asString();
    result = test_client.Tft_Summoner.by_account(reg, account_id);
    REQUIRE(result["accountId"] == account_id);
    result = test_client.Tft_Summoner.by_puuid(reg, puuid);
    REQUIRE(result["puuid"] == puuid);
    result = test_client.Tft_Summoner.by_summoner_id(reg, summoner_id);
    REQUIRE(result["id"] == summoner_id);
}
TEST_CASE("VAL-CONTENT-V1") {
    RiotApiClient test_client(CONFIG);

    Json::Value result;

    result = test_client.Val_Content.content(ROUTING);
    REQUIRE(result.isMember("version"));
    REQUIRE(result.isMember("characters"));
    REQUIRE(result.isMember("maps"));
}
// not available with my development
//TEST_CASE("VAL-MATCH-V1") {
//}
//TEST_CASE("VAL-RANKED-V1") {
//
//    std::string act_id = "0df5adb9-4dcb-6899-1306-3e9860661dd3";
//}
//TEST_CASE("VAL-STATUS-V1") {
//}
