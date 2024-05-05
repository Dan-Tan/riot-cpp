#pragma once

#include <vector>

#include "scope_count.h"

#ifdef DEBUG_MODE
#define rcp_assert(x, msg) if (!x) {std::cerr << "ASSERTION FAILED: " << msg << std::endl;}
#else 
#define rcp_assert(x, msg)
#endif
namespace riotcpp {
namespace rate {

    class RateHierachy {

        private:

            std::vector<ScopeCount> hierachy_;

        public:

            RateHierachy(const int durations[], const int limits[], const int counts[], const unsigned size);
            RateHierachy(const std::vector<int>& durations, const std::vector<int>& limits, const std::vector<int> counts);
            // TODO: There shouldn't be a string constructor as the arguments may be invalid
            RateHierachy(const std::string& description);

            int get_wait_time();
            void insert_request(unsigned server_time);

            std::string to_string() const;
    };

}
}
