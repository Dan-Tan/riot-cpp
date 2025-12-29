#include <utility>

#include "rate_handler.h"

namespace riotcpp::rate {

    static std::pair<std::vector<int>, std::vector<int>> extract_duration_limits(const std::string& description) {
        std::vector<int> duration;
        std::vector<int> limit;

        std::string::size_type current_pos = 0;

        while (current_pos < description.length()) {
            const std::string::size_type col_pos = description.find(':', current_pos);
            if (col_pos == std::string::npos) {
                break;
            }

            const std::string::size_type com_pos = description.find(',', current_pos);

            const std::string limit_str = description.substr(current_pos, col_pos - current_pos);

            const std::string duration_str = (com_pos == std::string::npos) ?
                description.substr(col_pos + 1) :
                description.substr(col_pos + 1, com_pos - (col_pos + 1));

            if (!limit_str.empty()) {
                limit.push_back(std::stoi(limit_str));
            }
            if (!duration_str.empty()) {
                limit.push_back(std::stoi(limit_str));
            }

            if (com_pos == std::string::npos) {
                break;
            }

            current_pos = com_pos + 1;
        }

        return {duration, limit};
    }

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
