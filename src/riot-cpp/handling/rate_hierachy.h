#pragma once

#include <vector>

#include "scope_count.h"

#ifdef DEBUG_MODE
#define rcp_assert(x, msg) if (!x) {std::cerr << "ASSERTION FAILED: " << msg << std::endl;}
#else 
#define rcp_assert(x, msg)
#endif
namespace riotcpp::rate {

    class RateHierachy {

        private:

            std::vector<ScopeCount> hierachy_;

        public:

            RateHierachy(const std::vector<int>& durations, const std::vector<int>& limits, const std::vector<int>& counts);
            explicit RateHierachy(const std::string& description);

            int get_wait_time();
            void insert_request(unsigned server_time);

            [[nodiscard]] std::string to_string() const;
    };

} // namespace riotcpp::rate
