#pragma once

#include <array>
#include <memory>
#include <mutex>
#include <vector>
#include <string>

#include "region_count.h"
#include "../query/endpoints.h"
#include "../types/args.h"

namespace riotcpp::rate {

    struct BucketSnapshot {
        int duration_sec = 0;
        int limit = 0;
        int count = 0;
        int seconds_until_reset = 0;
    };

    struct MethodRateSnapshot {
        std::string method_key;
        std::vector<BucketSnapshot> buckets;
        int wait_time_sec = 0;
    };

    struct RegionRateSnapshot {
        std::string routing_name;
        std::string routing_category;
        bool active = false;
        std::vector<BucketSnapshot> app_buckets;
        std::vector<MethodRateSnapshot> method_limits;
        int app_wait_time_sec = 0;
    };

    struct WaitTimeBreakdown {
        int total_wait_sec = 0;
        int app_wait_sec = 0;
        int method_wait_sec = 0;
    };
    
    /**
     * All request sent by this client need to be approved by the rate limiter.
     * Rate Counts are instantiated after the first request
     */
    class RateHandler {
        private:
            mutable std::mutex mutex_;
            std::array<RegionCount, NUM_PLATFORMS>     platform_counts;
            std::array<RegionCount, NUM_REGIONS>       region_counts;
            std::array<RegionCount, NUM_VAL_PLATFORMS> val_plaform_counts;

            bool initialised = false;

            bool initialise_counts(const query::RiotHeader&);

        public:
            RateHandler() = default;
            ~RateHandler() = default;
            
            bool is_initialized() const {
                std::lock_guard<std::mutex> lock(this->mutex_);
                return this->initialised;
            }

            /**
             * @param (out) request if a wait is required the send_time field will be update the earliest time the request can be sent
             * @return true if request can be sent immediately, false the client should wait at least until send_time to send
             */
            bool check_rate_limits(const std::shared_ptr<query::query>&);

            /**
             * update the rate counts with a request. Counts are only incremented upon successful request
             * @param request that was sent 
             */
            void insert_request(const std::shared_ptr<query::query>&);

            WaitTimeBreakdown get_wait_time_breakdown(const args::routing& route, const std::string& method_key);
            std::vector<RegionRateSnapshot> get_snapshot();
    };
} // namespace riotcpp::rate
