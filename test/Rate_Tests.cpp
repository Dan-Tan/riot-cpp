#include <catch2/catch_test_macros.hpp>
#include <string>
#include <iostream>
#include <ctime>
#include <queue>
#include <unordered_map>
#include <memory>
#include <thread>
#include "../src/riot-cpp/client/client.h"
namespace riotcpp {
namespace rate {

    static inline unsigned current_time_secs() {
        return std::chrono::duration_cast<std::chrono::seconds>(std::chrono::utc_clock::now().time_since_epoch()).count();
    }

    //TEST_CASE("Scope Count - No Requests") {
    //    ScopeCount scope_count = ScopeCount(120, 100, 0);
    //    REQUIRE(0 == scope_count.get_wait_time());
    //    REQUIRE(100 == scope_count.n_available());
    //}

    //TEST_CASE("Scope Count - Insertions") {
    //    int duration   = 120;
    //    int limit      = 100;
    //    int init_count = 0;

    //    ScopeCount scope_count = ScopeCount(duration, limit, init_count);

    //    scope_count.insert_request(current_time_secs());
    //    REQUIRE(limit-1 == scope_count.n_available());
    //    
    //    int n_insertions = 20;
    //    for (int i = 0; i < n_insertions; i++) {
    //        scope_count.insert_request(current_time_secs());
    //    }
    //    REQUIRE(limit - 1 - n_insertions == scope_count.n_available()); 
    //}

    //TEST_CASE("Scope Count - Wait time") {
    //    int duration   = 120;
    //    int limit      = 100;
    //    int init_count = 0;

    //    ScopeCount scope_count = ScopeCount(duration, limit, init_count);
    //    
    //    unsigned first_request_time = current_time_secs();
    //    for (int i = 0; i < limit; i++) {
    //        scope_count.insert_request(first_request_time);
    //    }

    //    unsigned current_time = current_time_secs();
    //    REQUIRE(duration - (current_time - first_request_time) == scope_count.get_wait_time());

    //    std::this_thread::sleep_for(std::chrono::seconds(10));

    //    current_time = current_time_secs();
    //    REQUIRE(duration - (current_time - first_request_time) == scope_count.get_wait_time());
    //}

    //TEST_CASE("Scope Count - Availability after wait") {
    //    int duration   = 120;
    //    int limit      = 100;
    //    int init_count = 0;

    //    ScopeCount scope_count = ScopeCount(duration, limit, init_count);
    //    
    //    unsigned first_request_time = current_time_secs();
    //    for (int i = 0; i < limit; i++) {
    //        scope_count.insert_request(first_request_time);
    //    }

    //    unsigned current_time = current_time_secs();
    //    REQUIRE(duration - (current_time - first_request_time) == scope_count.get_wait_time());

    //    std::this_thread::sleep_for(std::chrono::seconds(scope_count.get_wait_time()));

    //    REQUIRE(0 == scope_count.get_wait_time());
    //}

    //TEST_CASE("Rate Hierachy - Construction") {
    //    constexpr int durations[3] = {1, 10, 120};
    //    constexpr int limits[3] = {10, 50, 100};
    //    constexpr int counts[3] = {1, 1, 1};
    //    constexpr int size = 3;
    //    std::string header_repr = "10:1, 50:10, 100:120";

    //    RateHierachy explicit_construction(durations, limits, counts, size);
    //    RateHierachy string_construction(header_repr);

    //    REQUIRE(string_construction.to_string() == explicit_construction.to_string());
    //}

    TEST_CASE("Rate Hierachy - Wait time") {
        constexpr int size = 2;
        constexpr int durations[size] = {10, 120};
        constexpr int limits[size] = {50, 75};
        constexpr int counts[size] = {1, 1};

        RateHierachy rate_hierachy(durations, limits, counts, size);
        // No wait time when rate limit is not exceeded
        REQUIRE(rate_hierachy.get_wait_time() == 0);
        
        unsigned current_time = current_time_secs();
        unsigned reset_times[size] = {
            current_time + durations[0], current_time + durations[1]
        };
        rate_hierachy.insert_request(current_time);
        
        REQUIRE(rate_hierachy.get_wait_time() == 0);
        for (int i = 0; i < 49; i++) {
            rate_hierachy.insert_request(current_time);
        }
        current_time = current_time_secs();
        REQUIRE(reset_times[0] - current_time == rate_hierachy.get_wait_time());

        std::this_thread::sleep_for(std::chrono::seconds(rate_hierachy.get_wait_time()));
        REQUIRE(rate_hierachy.get_wait_time() == 0);

        current_time = current_time_secs();
        for (int i = 0; i < 25; i++) {
            rate_hierachy.insert_request(current_time);
        }
        current_time = current_time_secs();
        REQUIRE(reset_times[1] - current_time == rate_hierachy.get_wait_time());
    }
}


}
