#pragma once

#include <array>
#include <memory>

#include "region_count.h"
#include "../query/query.h"
#include "../types/args.h"

namespace riotcpp {
namespace rate {
    
    /**
     * All request sent by this client need to be approved by the rate limiter.
     * Rate Counts are instantiated on the first request
     */
    class RateHandler {
        private:
            std::array<RegionCount, NUM_PLATFORMS>     platform_counts;
            std::array<RegionCount, NUM_REGIONS>       region_counts;
            std::array<RegionCount, NUM_VAL_PLATFORMS> val_plaform_counts;

            bool initialised = false;

            bool initialise_counts(const query::RiotHeader&);

        public:
            RateHandler() = default;
            ~RateHandler() = default;
            
            /**
             * @param (out) request if a wait is required the send_time field will be update the earliest time the request can be sent
             * @return true if request can be sent immediately, false the client should wait at least until send_time to send
             */
            bool check_rate_limits(std::shared_ptr<query::query>);

            /**
             * update the rate counts with a request. Counts are only incremented upon successful request
             * @param request that was sent 
             */
            void insert_request(std::shared_ptr<query::query>);
    };
}
}
