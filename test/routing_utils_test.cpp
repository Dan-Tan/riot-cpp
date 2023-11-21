#include <catch2/catch_test_macros.hpp>

#include "../src/riot-cpp/handling/region_enums.h" 

TEST_CASE("PLATFORM SANITISATION") {
    /*
     * User may enter the routing as "BR1" or "br1", we should accept both
     */
    REQUIRE(routing::valid_platform("BR1"));
    REQUIRE(routing::valid_platform("EUN1"));
    REQUIRE(routing::valid_platform("EUW1"));
    REQUIRE(routing::valid_platform("JP1"));
    REQUIRE(routing::valid_platform("KR"));
    REQUIRE(routing::valid_platform("LA"));
    REQUIRE(routing::valid_platform("LA"));
    REQUIRE(routing::valid_platform("NA"));
    REQUIRE(routing::valid_platform("OC"));
    REQUIRE(routing::valid_platform("TR"));
    REQUIRE(routing::valid_platform("RU"));
    REQUIRE(routing::valid_platform("PH"));
    REQUIRE(routing::valid_platform("SG"));
    REQUIRE(routing::valid_platform("TH"));
    REQUIRE(routing::valid_platform("TW"));
    REQUIRE(routing::valid_platform("VN"));

    REQUIRE(routing::valid_platform("br1"));
    REQUIRE(routing::valid_platform("euN1"));
    REQUIRE(routing::valid_platform("euW1"));
    REQUIRE(routing::valid_platform("jp1"));
    REQUIRE(routing::valid_platform("kr"));
    REQUIRE(routing::valid_platform("la"));
    REQUIRE(routing::valid_platform("la"));
    REQUIRE(routing::valid_platform("na"));
    REQUIRE(routing::valid_platform("oc"));
    REQUIRE(routing::valid_platform("tr"));
    REQUIRE(routing::valid_platform("ru"));
    REQUIRE(routing::valid_platform("ph"));
    REQUIRE(routing::valid_platform("sg"));
    REQUIRE(routing::valid_platform("th"));
    REQUIRE(routing::valid_platform("tw"));
    REQUIRE(routing::valid_platform("vn"));
}

TEST_CASE("REGIONAL SANITISATION") {
    /*
     * User may enter the routing as "americas" or "AMERICAS", we should accept both
     */
    REQUIRE(routing::valid_regional("AMERICAS"));
    REQUIRE(routing::valid_regional("americas"));
    REQUIRE(routing::valid_regional("AMERicas"));
    REQUIRE(routing::valid_regional("amerICAS"));

    REQUIRE(routing::valid_regional("ASIA"));
    REQUIRE(routing::valid_regional("asia"));
    REQUIRE(routing::valid_regional("asIA"));
    REQUIRE(routing::valid_regional("AsiA"));

    REQUIRE(routing::valid_regional("EUROPE"));
    REQUIRE(routing::valid_regional("europe"));
    REQUIRE(routing::valid_regional("eurOPE"));
    REQUIRE(routing::valid_regional("EUROpe"));

    REQUIRE(routing::valid_regional("SEA"));
    REQUIRE(routing::valid_regional("sea"));
    REQUIRE(routing::valid_regional("sEA"));
    REQUIRE(routing::valid_regional("Sea"));
}

TEST_CASE("PLATFORM TO STRING") {
    REQUIRE("BR1"  == routing::platform_to_str(routing::platform::BR1));
    REQUIRE("EUN1" == routing::platform_to_str(routing::platform::EUN1));
    REQUIRE("EUW1" == routing::platform_to_str(routing::platform::EUW1));
    REQUIRE("JP1"  == routing::platform_to_str(routing::platform::JP1));
    REQUIRE("KR"   == routing::platform_to_str(routing::platform::KR));
    REQUIRE("LA1"  == routing::platform_to_str(routing::platform::LA1));
    REQUIRE("LA2"  == routing::platform_to_str(routing::platform::LA2));
    REQUIRE("NA1"  == routing::platform_to_str(routing::platform::NA1));
    REQUIRE("OC1"  == routing::platform_to_str(routing::platform::OC1));
    REQUIRE("TR1"  == routing::platform_to_str(routing::platform::TR1));
    REQUIRE("RU"   == routing::platform_to_str(routing::platform::RU));
    REQUIRE("PH2"  == routing::platform_to_str(routing::platform::PH2));
    REQUIRE("SG2"  == routing::platform_to_str(routing::platform::SG2));
    REQUIRE("TH2"  == routing::platform_to_str(routing::platform::TH2));
    REQUIRE("TW2"  == routing::platform_to_str(routing::platform::TW2));
    REQUIRE("VN2"  == routing::platform_to_str(routing::platform::VN2));
}

TEST_CASE("STRING TO PLATFORM") {
    REQUIRE(routing::platform::BR1  == routing::str_to_platform("BR1"));
    REQUIRE(routing::platform::EUN1 == routing::str_to_platform("EUN1"));
    REQUIRE(routing::platform::EUW1 == routing::str_to_platform("EUW1"));
    REQUIRE(routing::platform::JP1  == routing::str_to_platform("JP1"));
    REQUIRE(routing::platform::KR   == routing::str_to_platform("KR"));
    REQUIRE(routing::platform::LA1  == routing::str_to_platform("LA1"));
    REQUIRE(routing::platform::LA2  == routing::str_to_platform("LA2"));
    REQUIRE(routing::platform::NA1  == routing::str_to_platform("NA1"));
    REQUIRE(routing::platform::OC1  == routing::str_to_platform("OC1"));
    REQUIRE(routing::platform::TR1  == routing::str_to_platform("TR1"));
    REQUIRE(routing::platform::RU   == routing::str_to_platform("RU"));
    REQUIRE(routing::platform::PH2  == routing::str_to_platform("PH2"));
    REQUIRE(routing::platform::SG2  == routing::str_to_platform("SG2"));
    REQUIRE(routing::platform::TH2  == routing::str_to_platform("TH2"));
    REQUIRE(routing::platform::TW2  == routing::str_to_platform("TW2"));
    REQUIRE(routing::platform::VN2  == routing::str_to_platform("VN2"));
}

TEST_CASE("REGIONAL TO STRING") {
    REQUIRE("AMERICAS"  == routing::regional_to_str(routing::regional::AMERICAS));
    REQUIRE("ASIA"      == routing::regional_to_str(routing::regional::ASIA));
    REQUIRE("EUROPE"    == routing::regional_to_str(routing::regional::EUROPE));
    REQUIRE("SEA"       == routing::regional_to_str(routing::regional::SEA));
}

TEST_CASE("STRING TO REGIONAL") {
    REQUIRE(routing::regional::AMERICAS  == routing::str_to_regional("AMERICAS"));
    REQUIRE(routing::regional::ASIA      == routing::str_to_regional("ASIA"));
    REQUIRE(routing::regional::EUROPE    == routing::str_to_regional("EUROPE"));
    REQUIRE(routing::regional::SEA       == routing::str_to_regional("SEA"));
}
