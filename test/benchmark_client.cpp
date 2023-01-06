#include <catch2/catch_test_macros.hpp>
#include <catch2/benchmark/catch_benchmark.hpp>
#include <string>
#include "../src/client.h"
#include <jsoncpp/json/json.h>

#define CONFIG "../../.api_keys/riot_config.json", "../test/test_log.txt", false, false

using namespace client;

TEST_CASE( "TESTING ACCOUNT_V1 QUERIES", "[!benchmark]") {
    BENCHMARK ("Client Initialisation") {
        RiotApiClient test_client(CONFIG);
        return;
    };
    RiotApiClient test_client(CONFIG);

    std::string region = "americas";
    std::string puuid = "nvf_tmMcUOXhaWI4neMvma8WG3iymFlqS32-FpUFsLk3FqH9pGB_oQh5DoecV-Uiue2NIYddgww50A";
    std::string gamename = "DanTan1"; std::string tagline = "fresn";
    std::string game = "val";

    std::string endpoint = "ACCOUNT-V1";

    BENCHMARK ("Benchmark puuid account query") {
        return test_client.query(endpoint, std::string("by-puuid"), std::vector<std::string>{region, puuid});
    };
    BENCHMARK ("benchmark riot id account query") {
        return test_client.query(endpoint, std::string("by-riot-id"), std::vector<std::string>{region, gamename, tagline});
    };
    BENCHMARK ("Benchmark game account query") {
        return test_client.query(endpoint, std::string("by-game"), std::vector<std::string>{region, game, puuid});
    };
}

// League of Legend Queries

TEST_CASE( "TESTING LEAGUE_V4 QUERIES", "[!benchmark]") {
    BENCHMARK ("Client Initialisation") {
        RiotApiClient test_client(CONFIG);
        return;
    };
    RiotApiClient test_client(CONFIG);
    
    std::string region = "na1";
    std::string queue = "RANKED_SOLO_5x5"; 
    std::string division = "I";
    std::string tier = "DIAMOND";
    std::string summoner_id = "Ob0sRhCSzqtSRNHfAk_kS6Ac9TNOzKVhqj_kYr74HOhf2II";
    std::string league_id = "16dfc561-5064-4a5d-b4c4-9d4bfa03773f";

    std::string endpoint = "LEAGUE-V4";


    SECTION("Testing Challenger, Grandmaster and Master queue queries") {
        BENCHMARK ("LEAGUE-V4 challenger query") {
            return test_client.query(endpoint, std::string("challenger"), std::vector<std::string>{region, queue});
        };
        BENCHMARK ("LEAGUE-V4 grandmaster query") {
            return test_client.query(endpoint, std::string("grandmaster"), std::vector<std::string>{region, queue});
        };
        BENCHMARK ("LEAGUE-V4 master query") {
            return test_client.query(endpoint, std::string("master"), std::vector<std::string>{region, queue});
        };
    }

    SECTION("Testing specific queue queries") { // completely unnessecaru
        BENCHMARK ("Benchmark Diamond queue query") {
            return test_client.query(endpoint, std::string("specific-league"), std::vector<std::string>{region, queue, tier, division});
        };
        BENCHMARK ("Benchmark Platinum queue query") {
            return test_client.query(endpoint, std::string("specific-league"), std::vector<std::string>{region, queue, "PLATINUM", division});
        };
        BENCHMARK ("Benchmark Gold queue query") {
            return test_client.query(endpoint, std::string("specific-league"), std::vector<std::string>{region, queue, "GOLD", division});
        };
    }

    SECTION("Testing summoner id and league id") {
        BENCHMARK ("Benchmark summoner-id queue query") {
            return test_client.query(endpoint, std::string("by-summoner-id"), std::vector<std::string>{region, summoner_id});
        };
        BENCHMARK ("Benchmark summoner-id queue query") {
            return test_client.query(endpoint, std::string("by-league-id"), std::vector<std::string>{region, league_id});
        };
    }
}

TEST_CASE("TESTING SUMMONER QUERIES", "[RiotApiClient::summoner_v4]") {
    BENCHMARK ("Client Initialisation") {
        RiotApiClient test_client(CONFIG);
        return;
    };
    RiotApiClient test_client(CONFIG);

    std::string region = "kr";
    std::string account_id = "3y8psnoe72wockhi3aj8vydibzlpmy2lhoqyyzmz_g20";
    std::string summoner_name = "hide on bush";
    std::string puuid = "6dgdp5y88rxqomvmv1grogacmpp-uabmlsvrhkqj4g0kdih_gxqcee6w0jrmhrxstzbtxmfgypjzig";
    std::string summoner_id = "migc0lcqphu_dwe38s5hia5vtdongr-rdee8_ic5cj0gha";

    std::string endpoint = "SUMMONER-V4";
    

    BENCHMARK ("Benchmark acoount-id summoner query") {
        return test_client.query(endpoint, std::string("by-account-id"), std::vector<std::string>{region, account_id});
    };
    BENCHMARK ("Benchmark name summoner query") {
        return test_client.query(endpoint, std::string("by-name"), std::vector<std::string>{region, summoner_name});
    };
    BENCHMARK ("Benchmark puuid summoner query") {
        return test_client.query(endpoint, std::string("by-puuid"), std::vector<std::string>{region, puuid});
    };
    BENCHMARK ("Benchmark summoner-id summoner query") {
        return test_client.query(endpoint, std::string("by-summoner-id"), std::vector<std::string>{region, summoner_id});
    };
}

TEST_CASE( "TESTING MATCH QUERIES", "[!benchmark]" ) {
    RiotApiClient test_client(CONFIG);

    std::string region = "asia";
    std::string puuid = "6dgdp5y88rxqomvmv1grogacmpp-uabmlsvrhkqj4g0kdih_gxqcee6w0jrmhrxstzbtxmfgypjzig";
    std::string match_id = "kr_6279823690";

    std::string endpoint = "MATCH-V5";

    BENCHMARK ("Benchmark puuid match query") {
        return test_client.query(endpoint, std::string("by-puuid"), std::vector{region, puuid});
    };
    BENCHMARK ("Benchmark match-id match query") {
        return test_client.query(endpoint, std::string("by-match-id"), std::vector<std::string>{region, match_id});
    };
    BENCHMARK ("Benchmark match-timeline query") {
        return test_client.query(endpoint, std::string("match-timeline"), std::vector<std::string>{region, match_id});
    };
}

