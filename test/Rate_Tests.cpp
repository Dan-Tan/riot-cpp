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

    TEST_CASE("Scope Count - No Requests") {
        ScopeCount scope_count = ScopeCount(120, 100, 0);
        REQUIRE(0 == scope_count.get_wait_time());
        REQUIRE(100 == scope_count.n_available());
    }

    TEST_CASE("Scope Count - Insertions") {
        int duration   = 120;
        int limit      = 100;
        int init_count = 0;

        ScopeCount scope_count = ScopeCount(duration, limit, init_count);

        scope_count.insert_request(current_time_secs());
        REQUIRE(limit-1 == scope_count.n_available());
        
        int n_insertions = 20;
        for (int i = 0; i < n_insertions; i++) {
            scope_count.insert_request(current_time_secs());
        }
        REQUIRE(limit - 1 - n_insertions == scope_count.n_available()); 
    }

    TEST_CASE("Scope Count - Wait time") {
        int duration   = 120;
        int limit      = 100;
        int init_count = 0;

        ScopeCount scope_count = ScopeCount(duration, limit, init_count);
        
        unsigned first_request_time = current_time_secs();
        for (int i = 0; i < limit; i++) {
            scope_count.insert_request(first_request_time);
        }

        unsigned current_time = current_time_secs();
        REQUIRE(duration - (current_time - first_request_time) == scope_count.get_wait_time());

        std::this_thread::sleep_for(std::chrono::seconds(10));

        current_time = current_time_secs();
        REQUIRE(duration - (current_time - first_request_time) == scope_count.get_wait_time());
    }

    TEST_CASE("Scope Count - Availability after wait") {
        int duration   = 120;
        int limit      = 100;
        int init_count = 0;

        ScopeCount scope_count = ScopeCount(duration, limit, init_count);
        
        unsigned first_request_time = current_time_secs();
        for (int i = 0; i < limit; i++) {
            scope_count.insert_request(first_request_time);
        }

        unsigned current_time = current_time_secs();
        REQUIRE(duration - (current_time - first_request_time) == scope_count.get_wait_time());

        std::this_thread::sleep_for(std::chrono::seconds(scope_count.get_wait_time()));

        REQUIRE(0 == scope_count.get_wait_time());
    }
}


}
