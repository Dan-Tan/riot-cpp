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
            void sync_from_headers(const std::string& limit_header, const std::string& count_header, unsigned server_time, int retry_after_sec = 0);

            std::vector<ScopeCount>& scopes() { return this->hierachy_; }
            const std::vector<ScopeCount>& scopes() const { return this->hierachy_; }

            [[nodiscard]] std::string to_string() const;
    };

} // namespace riotcpp::rate
