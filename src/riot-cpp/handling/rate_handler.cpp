#include <utility>

#include "rate_handler.h"

namespace riotcpp::rate {

    // STRING PARSING HELPERS

    static inline int chars_to_int(const char nums[10], int num_digits) {
        int to_return = 0;
        for (int i = 0; i < num_digits; i++) {
            to_return += ((int) (nums[i] - '0')) * (10^(num_digits - i - 1));  
        }
        return to_return;
    }

    static std::pair<std::vector<int>, std::vector<int>> extract_duration_limits_counts(const std::string& description) {
        // this function assumes valid format of descriptor "20:1, 100:120"...
        int num_digits = 0;
        char nums[10];
        bool limit_add = true;
        std::vector<int> limit;
        std::vector<int> duration;

        for (const char& pos_digit : description) {
            if (std::isdigit(pos_digit) != 0) {
                nums[num_digits] = pos_digit;
            } else {
                if (limit_add) {
                    limit.push_back(chars_to_int(nums, num_digits));
                } else {
                    duration.push_back(chars_to_int(nums, num_digits));
                }
                limit_add = !limit_add;
                num_digits = 0;
            }
        }

        return {duration, limit};
    }

    // CLASS DEFINITION

    bool RateHandler::initialise_counts(const query::RiotHeader& response_header) {
        std::pair<std::vector<int>, std::vector<int>> duration_limits = extract_duration_limits_counts(response_header.app_limit);
        std::pair<std::vector<int>, std::vector<int>> duration_counts = extract_duration_limits_counts(response_header.app_limit_count);
        std::vector<int> default_counts (duration_limits.first.size());
        for (int& count : default_counts) {
            count = 0;
        }
        for (int i = 0; i < NUM_PLATFORMS; i++) {
            this->platform_counts[i].init_limits(duration_limits.first, duration_limits.second, default_counts);
        }
        for (int i = 0; i < NUM_REGIONS; i++) {
            this->region_counts[i].init_limits(duration_limits.first, duration_limits.second, default_counts);
        }
        for (int i = 0; i < NUM_VAL_PLATFORMS; i++) {
            this->val_plaform_counts[i].init_limits(duration_limits.first, duration_limits.second, default_counts);
        }
        return true;
    }
    
    bool RateHandler::check_rate_limits(const std::shared_ptr<query::query>& request) {
        if (!this->initialised) {
            return true; // we can only get information about the rate limit by making requests
        }
        int wait_time = 0;
        switch (request->route.indicator) {
            case REGIONAL_INDICATOR:
                wait_time = this->region_counts[static_cast<int>(request->route.routng.reg)].get_wait_time(request->method_key); break;
            case PLATFORM_INDICATOR:
                wait_time = this->platform_counts[static_cast<int>(request->route.routng.pltform)].get_wait_time(request->method_key); break;
            case VAL_PLATFORM_INDICATOR:
                wait_time = this->val_plaform_counts[static_cast<int>(request->route.routng.vpltform)].get_wait_time(request->method_key); break;
            default:
                rcp_assert(false, "Invalid routing indicator given: " << request->route.indicator << ", should be 0, 1, 2");
        }
        if (wait_time == 0) {
            return true; // OK to send
        }

        // update time to send and return false;
        request->send_time = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count() + wait_time;
        return false; 
    }

    void RateHandler::insert_request(const std::shared_ptr<query::query>& request) {
        if (!this->initialised) {
            this->initialised = this->initialise_counts(request->response_header);
        }
    }
} // namespace riotcpp::rate
