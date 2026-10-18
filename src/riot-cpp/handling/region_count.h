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
            bool active_ = false;

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
            int get_app_wait_time();
            int get_method_wait_time(const std::string& method_key);
            void insert_request(long query_id,
                                unsigned server_time, 
                                const std::string& method_key, 
                                const std::string& method_limits);
            void insert_request_with_headers(long query_id,
                                             unsigned server_time,
                                             const std::string& method_key,
                                             const std::string& app_limits,
                                             const std::string& app_counts,
                                             const std::string& method_limits,
                                             const std::string& method_counts,
                                             int retry_after_sec = 0);

            bool is_active() const { return active_; }
            RateHierachy& app_limits() { return app_limits_; }
            std::unordered_map<std::string, RateHierachy>& method_limits() { return method_limits_; }

            std::string to_string() const;

    };
} // namespace riotcpp::rate
