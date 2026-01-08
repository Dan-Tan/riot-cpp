#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include "../src/riot-cpp/query/url.h"

namespace riotcpp {
namespace url {

    TEST_CASE("URL Path Construction") {
        std::string routing = "kr";
        std::string base_url = "/base/v1";

        SECTION("No arguments") {
            std::string expected = "https://kr.api.riotgames.com/base/v1/path";
            std::string received = construct_url_path(routing, base_url, "/path", std::array<std::string, 0>{});
            REQUIRE(received == expected);
        }

        SECTION("One argument") {
            std::string expected = "https://kr.api.riotgames.com/base/v1/summoners/12345";
            std::string received = construct_url_path(routing, base_url, "", std::array<std::string, 1>{"/summoners/"}, 12345);
            REQUIRE(received == expected);
        }

        SECTION("Multiple arguments with encoding") {
            std::string expected = "https://kr.api.riotgames.com/base/v1/summoners/by-name/Hello%20World/details";
            std::string received = construct_url_path(routing, base_url, "/details", std::array<std::string, 1>{"/summoners/by-name/"}, "Hello World");
            REQUIRE(received == expected);

            expected = "https://kr.api.riotgames.com/base/v1/one/1/two/some%20name/three";
            received = construct_url_path(routing, base_url, "/three", std::array<std::string, 2>{"/one/", "/two/"}, 1, "some name");
            REQUIRE(received == expected);
        }

        SECTION("With additional fragment") {
            std::string expected = "https://kr.api.riotgames.com/base/v1/matches/by-puuid/PUUID_HERE/ids";
            std::string received = construct_url_path(routing, base_url, "/ids", std::array<std::string, 1>{"/matches/by-puuid/"}, "PUUID_HERE");
            REQUIRE(received == expected);
        }
    }

}
}
