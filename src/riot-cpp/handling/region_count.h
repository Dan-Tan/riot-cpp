#pragma once

#include <vector>
#include <unordered_map>
#include <string>

#include "rate_hierachy.h"

namespace rate {

    class RegionCount {
      
        private:
            RateHierachy app_limits_ = {{0}, {0}, {0}};
            std::unordered_map<std::string, RateHierachy> method_limits_ {};

        public:
            RegionCount() = default;
            ~RegionCount() = default;
            
            void init_limits(const std::vector<int>& durations, const std::vector<int>& limits, const std::vector<int>& counts);
            int get_wait_time(const std::string& method_key);
            void insert_request(unsigned server_time, 
                                const std::string& method_key, 
                                const std::string& method_limits);

            std::string to_string() const;

    };
}
