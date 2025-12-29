#pragma once

#include <vector>
#include <unordered_map>
#include <string>

#include "rate_hierachy.h"
namespace riotcpp::rate {

    class RegionCount {
      
        private:
            RateHierachy app_limits_ = {{0}, {0}, {0}};
            std::unordered_map<std::string, RateHierachy> method_limits_;

        public:
            RegionCount() = default;
            ~RegionCount() = default;
            
            /**
             * Initialise Application Limits, simple wrapper of rate hierachy limits
             */
            void init_limits(const std::vector<int>& durations, const std::vector<int>& limits, const std::vector<int>& counts);
            /**
             * Initialise Application Limits, simple wrapper of rate hierachy limits using string
             */
            void init_limits(const std::string& description);
            int get_wait_time(const std::string& method_key);
            void insert_request(unsigned server_time, 
                                const std::string& method_key, 
                                const std::string& method_limits);

            std::string to_string() const;

    };
} // namespace riotcpp::rate
