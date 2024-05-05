#include <catch2/catch_test_macros.hpp>

#include "../src/riot-cpp/types/args.h" 

namespace riotcpp {
namespace args {

    TEST_CASE("PLATFORM SANITISATION") {
        /*
         * User may enter the routing as "BR1" or "br1", we should accept both
         */
        REQUIRE(valid_platform("BR1"));
        REQUIRE(valid_platform("EUN1"));
        REQUIRE(valid_platform("EUW1"));
        REQUIRE(valid_platform("JP1"));
        REQUIRE(valid_platform("KR"));
        REQUIRE(valid_platform("LA"));
        REQUIRE(valid_platform("LA"));
        REQUIRE(valid_platform("NA"));
        REQUIRE(valid_platform("OC"));
        REQUIRE(valid_platform("TR"));
        REQUIRE(valid_platform("RU"));
        REQUIRE(valid_platform("PH"));
        REQUIRE(valid_platform("SG"));
        REQUIRE(valid_platform("TH"));
        REQUIRE(valid_platform("TW"));
        REQUIRE(valid_platform("VN"));

        REQUIRE(valid_platform("br1"));
        REQUIRE(valid_platform("euN1"));
        REQUIRE(valid_platform("euW1"));
        REQUIRE(valid_platform("jp1"));
        REQUIRE(valid_platform("kr"));
        REQUIRE(valid_platform("la"));
        REQUIRE(valid_platform("la"));
        REQUIRE(valid_platform("na"));
        REQUIRE(valid_platform("oc"));
        REQUIRE(valid_platform("tr"));
        REQUIRE(valid_platform("ru"));
        REQUIRE(valid_platform("ph"));
        REQUIRE(valid_platform("sg"));
        REQUIRE(valid_platform("th"));
        REQUIRE(valid_platform("tw"));
        REQUIRE(valid_platform("vn"));
    }

    TEST_CASE("REGIONAL SANITISATION") {
        /*
         * User may enter the routing as "americas" or "AMERICAS", we should accept both
         */
        REQUIRE(valid_regional("AMERICAS"));
        REQUIRE(valid_regional("americas"));
        REQUIRE(valid_regional("AMERicas"));
        REQUIRE(valid_regional("amerICAS"));

        REQUIRE(valid_regional("ASIA"));
        REQUIRE(valid_regional("asia"));
        REQUIRE(valid_regional("asIA"));
        REQUIRE(valid_regional("AsiA"));

        REQUIRE(valid_regional("EUROPE"));
        REQUIRE(valid_regional("europe"));
        REQUIRE(valid_regional("eurOPE"));
        REQUIRE(valid_regional("EUROpe"));

        REQUIRE(valid_regional("SEA"));
        REQUIRE(valid_regional("sea"));
        REQUIRE(valid_regional("sEA"));
        REQUIRE(valid_regional("Sea"));
    }

    TEST_CASE("PLATFORM TO STRING") {
        REQUIRE("BR1"  == platform_to_str(platform::BR1));
        REQUIRE("EUN1" == platform_to_str(platform::EUN1));
        REQUIRE("EUW1" == platform_to_str(platform::EUW1));
        REQUIRE("JP1"  == platform_to_str(platform::JP1));
        REQUIRE("KR"   == platform_to_str(platform::KR));
        REQUIRE("LA1"  == platform_to_str(platform::LA1));
        REQUIRE("LA2"  == platform_to_str(platform::LA2));
        REQUIRE("NA1"  == platform_to_str(platform::NA1));
        REQUIRE("OC1"  == platform_to_str(platform::OC1));
        REQUIRE("TR1"  == platform_to_str(platform::TR1));
        REQUIRE("RU"   == platform_to_str(platform::RU));
        REQUIRE("PH2"  == platform_to_str(platform::PH2));
        REQUIRE("SG2"  == platform_to_str(platform::SG2));
        REQUIRE("TH2"  == platform_to_str(platform::TH2));
        REQUIRE("TW2"  == platform_to_str(platform::TW2));
        REQUIRE("VN2"  == platform_to_str(platform::VN2));
    }

    TEST_CASE("STRING TO PLATFORM") {
        REQUIRE(platform::BR1  == str_to_platform("BR1"));
        REQUIRE(platform::EUN1 == str_to_platform("EUN1"));
        REQUIRE(platform::EUW1 == str_to_platform("EUW1"));
        REQUIRE(platform::JP1  == str_to_platform("JP1"));
        REQUIRE(platform::KR   == str_to_platform("KR"));
        REQUIRE(platform::LA1  == str_to_platform("LA1"));
        REQUIRE(platform::LA2  == str_to_platform("LA2"));
        REQUIRE(platform::NA1  == str_to_platform("NA1"));
        REQUIRE(platform::OC1  == str_to_platform("OC1"));
        REQUIRE(platform::TR1  == str_to_platform("TR1"));
        REQUIRE(platform::RU   == str_to_platform("RU"));
        REQUIRE(platform::PH2  == str_to_platform("PH2"));
        REQUIRE(platform::SG2  == str_to_platform("SG2"));
        REQUIRE(platform::TH2  == str_to_platform("TH2"));
        REQUIRE(platform::TW2  == str_to_platform("TW2"));
        REQUIRE(platform::VN2  == str_to_platform("VN2"));
    }

    TEST_CASE("REGIONAL TO STRING") {
        REQUIRE("AMERICAS"  == regional_to_str(regional::AMERICAS));
        REQUIRE("ASIA"      == regional_to_str(regional::ASIA));
        REQUIRE("EUROPE"    == regional_to_str(regional::EUROPE));
        REQUIRE("SEA"       == regional_to_str(regional::SEA));
    }

    TEST_CASE("STRING TO REGIONAL") {
        REQUIRE(regional::AMERICAS  == str_to_regional("AMERICAS"));
        REQUIRE(regional::ASIA      == str_to_regional("ASIA"));
        REQUIRE(regional::EUROPE    == str_to_regional("EUROPE"));
        REQUIRE(regional::SEA       == str_to_regional("SEA"));
    }

    TEST_CASE("PLATFORM TO REGIONAL") {
        REQUIRE(regional::AMERICAS == platform_to_regional(platform::NA1));
        REQUIRE(regional::AMERICAS == platform_to_regional(platform::BR1));
        REQUIRE(regional::AMERICAS == platform_to_regional(platform::LA1));
        REQUIRE(regional::AMERICAS == platform_to_regional(platform::LA2));

        REQUIRE(regional::ASIA == platform_to_regional(platform::KR));
        REQUIRE(regional::ASIA == platform_to_regional(platform::JP1));

        REQUIRE(regional::EUROPE == platform_to_regional(platform::EUN1));
        REQUIRE(regional::EUROPE == platform_to_regional(platform::EUW1));
        REQUIRE(regional::EUROPE == platform_to_regional(platform::TR1));
        REQUIRE(regional::EUROPE == platform_to_regional(platform::RU));

        REQUIRE(regional::SEA == platform_to_regional(platform::OC1));
        REQUIRE(regional::SEA == platform_to_regional(platform::PH2));
        REQUIRE(regional::SEA == platform_to_regional(platform::SG2));
        REQUIRE(regional::SEA == platform_to_regional(platform::TH2));
        REQUIRE(regional::SEA == platform_to_regional(platform::TW2));
        REQUIRE(regional::SEA == platform_to_regional(platform::VN2));
    }

    TEST_CASE("VALORANT PLATFORM TO STRING") {
        REQUIRE("AP" == val_platform_to_str(val_platform::AP));
        REQUIRE("BR" == val_platform_to_str(val_platform::BR));
        REQUIRE("EU" == val_platform_to_str(val_platform::EU));
        REQUIRE("KR" == val_platform_to_str(val_platform::KR));
        REQUIRE("LATAM" == val_platform_to_str(val_platform::LATAM));
        REQUIRE("NA" == val_platform_to_str(val_platform::NA));
        REQUIRE("ESPORTS" == val_platform_to_str(val_platform::ESPORTS));
    }

    TEST_CASE("STRING TO VALORANT PLATFORM") {
        REQUIRE(val_platform::AP == str_to_val_platform("AP"));
        REQUIRE(val_platform::BR == str_to_val_platform("BR"));
        REQUIRE(val_platform::EU == str_to_val_platform("EU"));
        REQUIRE(val_platform::KR == str_to_val_platform("KR"));
        REQUIRE(val_platform::LATAM == str_to_val_platform("LATAM"));
        REQUIRE(val_platform::NA == str_to_val_platform("NA"));
        REQUIRE(val_platform::ESPORTS == str_to_val_platform("ESPORTS"));
        REQUIRE(std::nullopt == str_to_val_platform("blah blahblah"));
    }

    TEST_CASE("DIVISION TO STRING") {
        REQUIRE("I" == division_to_str(division::I));
        REQUIRE("II" == division_to_str(division::II));
        REQUIRE("III" == division_to_str(division::III));
        REQUIRE("IV" == division_to_str(division::IV));
    }

    TEST_CASE("STRING TO DIVISION") {
        REQUIRE(division::I == str_to_division("I"));
        REQUIRE(division::II == str_to_division("II"));
        REQUIRE(division::III == str_to_division("III"));
        REQUIRE(division::IV == str_to_division("IV"));
        REQUIRE(std::nullopt == str_to_division("invalid string"));
    }

    TEST_CASE("TIER TO STRING") {
        REQUIRE("IRON" == tier_to_str(tier::IRON));
        REQUIRE("BRONZE" == tier_to_str(tier::BRONZE));
        REQUIRE("SILVER" == tier_to_str(tier::SILVER));
        REQUIRE("GOLD" == tier_to_str(tier::GOLD));
        REQUIRE("PLATINUM" == tier_to_str(tier::PLATINUM));
        REQUIRE("EMERALD" == tier_to_str(tier::EMERALD));
        REQUIRE("DIAMOND" == tier_to_str(tier::DIAMOND));
    }

    TEST_CASE("STRING TO TIER") {
        REQUIRE(tier::IRON == str_to_tier("IRON"));
        REQUIRE(tier::BRONZE == str_to_tier("BRONZE"));
        REQUIRE(tier::SILVER == str_to_tier("SILVER"));
        REQUIRE(tier::GOLD == str_to_tier("GOLD"));
        REQUIRE(tier::PLATINUM == str_to_tier("PLATINUM"));
        REQUIRE(tier::EMERALD == str_to_tier("EMERALD"));
        REQUIRE(tier::DIAMOND == str_to_tier("DIAMOND"));
        REQUIRE(std::nullopt == str_to_tier("invalid blah blah"));
    }

    TEST_CASE("QUEUE TO STRING") {
        REQUIRE("RANKED_SOLO_5x5" == queue_to_str(queue::RANKED_SOLO_5x5));
        REQUIRE("RANKED_FLEX_SR" == queue_to_str(queue::RANKED_FLEX_SR));
        REQUIRE("RANKED_FLEX_TT" == queue_to_str(queue::RANKED_FLEX_TT));
    }

    TEST_CASE("STRING TO QUEUE") {
        REQUIRE(queue::RANKED_SOLO_5x5 == str_to_queue("RANKED_SOLO_5x5"));
        REQUIRE(queue::RANKED_FLEX_SR == str_to_queue("RANKED_FLEX_SR"));
        REQUIRE(queue::RANKED_FLEX_TT == str_to_queue("RANKED_FLEX_TT"));
        REQUIRE(std::nullopt == str_to_queue("invalid blhlhlh"));
    }
}
}
