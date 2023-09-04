#include <catch2/catch_test_macros.hpp>
#include <string>
#include <iostream>
#include <ctime>
#include <queue>
#include <unordered_map>
#include <memory>
#include "../src/riot-cpp/client/client.h"

using namespace handler;

void update_time(std::shared_ptr<query::query> request) {
    const std::time_t c_time = std::time(NULL);
    const std::tm* current_time = std::gmtime(&c_time);
    char cur_time[std::size("Mon, 13 Jan 2023 18:48:45 GMT")];
    std::strftime(cur_time, std::size("Mon, 16 Jan 2023 08:48:45 GMT"), "%a, %d %b %Y %T GMT", current_time);
    strncpy(request->response_header.date, cur_time, 32);
}

TEST_CASE("SCOPE HISTORY TESTS") {
    handler_structs::ScopeHistory test_scope = {.duration = 10, .limit = 10};

    SECTION("BASIC INITIALISATION") {
        REQUIRE(test_scope.duration == 10);
        REQUIRE(test_scope.limit == 10);
    }

    SECTION("FILL QUEUE") {
        std::time_t cur_time;
        int available;
        for (int i = 0; i < test_scope.limit; i++) {
            cur_time = std::time(NULL);
            test_scope.insert_request(std::mktime(std::gmtime(&cur_time)));
            available = test_scope.available_requests();
            REQUIRE(available == test_scope.limit - i - 1);
        }
    }
    
    while (test_scope.history.size() != 0) {
        test_scope.history.pop();
    }

    SECTION("PURGE QUEUE") {
        std::time_t to_remove = 0;
        for (int i = 0; i < test_scope.limit; i++) {
            test_scope.insert_request(to_remove);
            REQUIRE(test_scope.history.size() == i+1);
        }
        test_scope.update_history();
        REQUIRE(test_scope.history.size() == 0);
    }

    SECTION("SERVER OVERRIDE") {
        int server_counter = 5;
        int server_limit = 20;
        int server_duration = 30;
        test_scope.correct_history(server_counter, server_limit, server_duration);
        REQUIRE(test_scope.duration == 30);
        REQUIRE(test_scope.limit == 20);
        REQUIRE(test_scope.history.size() == 5);
    }
    // purge for next section
    while (test_scope.history.size() != 0) {
        test_scope.history.pop();
    }

    SECTION("VALIDATE REQUEST TEST") {
        std::time_t out = test_scope.validate_request();
        REQUIRE(out == 0);

        std::time_t c_time;
        for (int i = 0; i < test_scope.limit - 1; i++) {
            c_time = std::time(NULL);
            test_scope.insert_request(std::mktime(std::gmtime(&c_time)));
        }
        out = test_scope.validate_request();
        REQUIRE(out == 0);

        c_time = std::time(NULL);
        test_scope.insert_request(std::mktime(std::gmtime(&c_time)));
        out = test_scope.validate_request();
        REQUIRE(test_scope.history.size() == test_scope.limit);
        REQUIRE(out != 0);
    }
}

TEST_CASE("REGION HISTORY TESTS") {
    std::vector<int> limits = {100, 1000, 10000};
    std::vector<int> durations = {1, 60, 3600};
    std::time_t server_time = std::time(NULL)-1;
    handler_structs::RegionHistory region = handler_structs::init_region(limits, durations);
    
    SECTION("INITIALISATION CHECKS") {
        for (int i = 0; i < region.application_hierachy.size(); i++) {
            REQUIRE(region.application_hierachy.at(limits.size() - i - 1).limit == limits.at(i));
            REQUIRE(region.application_hierachy.at(limits.size() - i - 1).duration == durations.at(i));
            REQUIRE(region.application_hierachy.at(limits.size() - i - 1).history.size() == 0);
        }
    }

    SECTION("METHOD INSERTION CHECKS") {
        std::vector<std::string> method_keys = {"ACCOUNT-V1-by-puuid", "ACCOUNT-V1-by-game", "SUMMONER-V4-by-name"};
        std::vector<std::string> method_limits = {"100:10", "3600:60", "300:5"};
        limits = {100, 3600, 300};
        durations = {10, 60, 5};
        bool update = true;
        for (int i = 0; i < limits.size(); i++) {
            region.insert_request(server_time, method_keys[i], method_limits[i]);
            REQUIRE(region.method_queues.at(method_keys[i]).at(0).limit == limits.at(i));
            REQUIRE(region.method_queues.at(method_keys[i]).at(0).duration == durations.at(i));
            REQUIRE(region.method_queues.at(method_keys[i]).at(0).history.size() == 1);
        }
    }

    SECTION("QUERY VALIDATION") {
        std::string method = "ACCOUNT-V1-by-puuid";
        std::string method_limit = "100:10";
        int lim = 100;
        int endpoint_availability;
        endpoint_availability = region.validate_request(method);
        REQUIRE(endpoint_availability == 0);
        for (int j = 0; j < lim; j++) {
            server_time = std::time(NULL);
            region.insert_request(std::mktime(std::gmtime(&server_time)), method, method_limit);
        }
        endpoint_availability = region.validate_request(method);
        REQUIRE_FALSE(endpoint_availability == 0);
    }


    SECTION("APPLICATION LIMIT TESTING") {
        std::string method = "ACCOUNT-V1-by-puuid";
        std::string method_limit = "1000:10";
        std::vector<int> lim = {100, 1000, 10000};
        durations = {10, 60, 3600};
        
        handler_structs::RegionHistory application_testing = handler_structs::init_region(lim, durations);
        int endpoint_availability;
        endpoint_availability = application_testing.validate_request(method);
        REQUIRE(endpoint_availability == 0);
        for (int i = 0; i < lim.at(0); i++) {
            server_time = std::time(NULL);
            application_testing.insert_request(std::mktime(std::gmtime(&server_time)), method, method_limit);
        }
        endpoint_availability = application_testing.validate_request(method);
        REQUIRE_FALSE(endpoint_availability == 0);
    }

}

TEST_CASE("RATE TESTS") {
    logging::Logger logger("../test/log_file.txt", logging::LEVEL::INFO);
    struct RateHandler handler(&logger);
    std::string region = "na1";

    query::RiotHeader example_header = {"Mon, 16 Jan 2023 09:48:45 GMT", "100:1,1000:10,60000:600,360000:3600", "1:1,2:10,2:600,2:3600", "1000:1", "1:1"};
    
    std::string method = "SUMMONER-V4-by-puuid";
    std::time_t server_time = std::time(NULL);
    std::string urls = "fake_url";

    std::shared_ptr<query::query> test_query = std::make_shared<query::query>(method, region, urls, 0, nullptr, example_header);
    update_time(test_query);

    handler.init_queues(test_query);
    handler_structs::RegionHistory region_history;
    std::vector<int> limits = {100, 1000, 60000, 360000};
    std::vector<int> durations = {1, 10, 600, 3600};

    SECTION("APPLICATION INITIALISATION") {
        for (int i = 0; i < handler.routing_queues.size(); i++) {
            REQUIRE(handler.routing_queues.at(i).application_hierachy.at(3).limit == limits.at(0));
            REQUIRE(handler.routing_queues.at(i).application_hierachy.at(2).limit == limits.at(1));
            REQUIRE(handler.routing_queues.at(i).application_hierachy.at(1).limit == limits.at(2));
            REQUIRE(handler.routing_queues.at(i).application_hierachy.at(0).limit == limits.at(3));
            REQUIRE(handler.routing_queues.at(i).application_hierachy.at(3).duration == durations.at(0));
            REQUIRE(handler.routing_queues.at(i).application_hierachy.at(2).duration == durations.at(1));
            REQUIRE(handler.routing_queues.at(i).application_hierachy.at(1).duration == durations.at(2));
            REQUIRE(handler.routing_queues.at(i).application_hierachy.at(0).duration == durations.at(3));
        }
    }

    SECTION("QUERY VALIDATION") {
        test_query->routing_value = "na1";
        test_query->method_key = "SUMMONER-V4-by-puuid";
        test_query->send_time = 0;
        test_query->last_response = -2;
        bool wait_time = handler.validate_request(test_query);
        REQUIRE(wait_time);
        REQUIRE(test_query->send_time == 0);
        test_query->last_response = 200;
        for (int i = 0; i < limits.at(1) + 1; i++){
            update_time(test_query);
            handler.review_request(test_query);
        }
        test_query->last_response = -2;
        handler.validate_request(test_query);
        REQUIRE_FALSE(test_query->send_time == 0);
    }
}

TEST_CASE("Test with server") {
    bool verbose = true;
    client::RiotApiClient test_client("../../.api_keys/riot_config.json", "../test/log_file.txt", logging::LEVEL::DEBUG, verbose);

    // we want to try trigger the server rate limiter and check if we stop our queries or get sent a 429 error
    int n_attempts = 500;
    for (int i = 0; i < n_attempts; i++) {
        test_client.Summoner.by_name("oc1", "Monkeys R Us");
    }
}

