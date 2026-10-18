#include <utility>
#include "../logging/logger.h"
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

            if (!limit_str.empty() && !duration_str.empty()) {
                try {
                    limit.push_back(std::stoi(limit_str));
                    duration.push_back(std::stoi(duration_str));
                } catch (...) {}
            }

            if (com_pos == std::string::npos) {
                break;
            }

            current_pos = com_pos + 1;
        }

        return {duration, limit};
    }

    bool RateHandler::initialise_counts(const query::RiotHeader& response_header) {
        std::pair<std::vector<int>, std::vector<int>> duration_limits = extract_duration_limits(response_header.app_limit);
        if (duration_limits.first.empty()) {
            duration_limits.first = {1, 120};
            duration_limits.second = {20, 100};
        }
        std::vector<int> default_counts(duration_limits.first.size(), 0);
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
        std::lock_guard<std::mutex> lock(this->mutex_);
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
                logging::get()->critical("Invalid routing indicator given: {}, should be 0, 1, 2", request->route.indicator);
                throw std::invalid_argument("Invalid routing indicator");
        }
        if (wait_time == 0) {
            logging::get()->debug("[Query {}] Rate limit check passed.", request->query_id);
            request->send_time = 0;
            return true; // OK to send
        }

        // update time to send and return false;
        logging::get()->info("[Query {}] Rate limit requires waiting. Delay: {}s.", request->query_id, wait_time);
        request->send_time = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count() + wait_time;
        return false; 
    }

    void RateHandler::insert_request(const std::shared_ptr<query::query>& request) {
        std::lock_guard<std::mutex> lock(this->mutex_);
        if (!this->initialised) {
            this->initialised = this->initialise_counts(request->response_header);
        }

        int retry_after_sec = 0;
        if (!request->response_header.retry_after.empty()) {
            try {
                retry_after_sec = std::max(1, std::stoi(request->response_header.retry_after));
            } catch (...) {
                retry_after_sec = 1;
            }
        } else if (request->last_response == 429) {
            retry_after_sec = 2;
        }

        unsigned server_time = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();
        switch (request->route.indicator) {
            case REGIONAL_INDICATOR:
                this->region_counts[static_cast<int>(request->route.routng.reg)].insert_request_with_headers(
                    request->query_id, server_time, request->method_key,
                    request->response_header.app_limit, request->response_header.app_limit_count,
                    request->response_header.method_limit, request->response_header.method_limit_count,
                    retry_after_sec);
                break;
            case PLATFORM_INDICATOR:
                this->platform_counts[static_cast<int>(request->route.routng.pltform)].insert_request_with_headers(
                    request->query_id, server_time, request->method_key,
                    request->response_header.app_limit, request->response_header.app_limit_count,
                    request->response_header.method_limit, request->response_header.method_limit_count,
                    retry_after_sec);
                break;
            case VAL_PLATFORM_INDICATOR:
                this->val_plaform_counts[static_cast<int>(request->route.routng.vpltform)].insert_request_with_headers(
                    request->query_id, server_time, request->method_key,
                    request->response_header.app_limit, request->response_header.app_limit_count,
                    request->response_header.method_limit, request->response_header.method_limit_count,
                    retry_after_sec);
                break;
            default:
                logging::get()->critical("[Query {}] Invalid routing indicator encountered during request insertion: {}", request->query_id, request->route.indicator);
                throw std::invalid_argument("Invalid routing indicator");
        }
    }

    WaitTimeBreakdown RateHandler::get_wait_time_breakdown(const args::routing& route, const std::string& method_key) {
        std::lock_guard<std::mutex> lock(this->mutex_);
        WaitTimeBreakdown res;
        if (!this->initialised) return res;

        RegionCount* rc = nullptr;
        switch (route.indicator) {
            case REGIONAL_INDICATOR:
                rc = &this->region_counts[static_cast<int>(route.routng.reg)];
                break;
            case PLATFORM_INDICATOR:
                rc = &this->platform_counts[static_cast<int>(route.routng.pltform)];
                break;
            case VAL_PLATFORM_INDICATOR:
                rc = &this->val_plaform_counts[static_cast<int>(route.routng.vpltform)];
                break;
            default:
                return res;
        }
        res.app_wait_sec = rc->get_app_wait_time();
        res.method_wait_sec = rc->get_method_wait_time(method_key);
        res.total_wait_sec = std::max(res.app_wait_sec, res.method_wait_sec);
        return res;
    }

    static RegionRateSnapshot build_region_snapshot(RegionCount& rc, const std::string& name, const std::string& category) {
        RegionRateSnapshot snap;
        snap.routing_name = name;
        snap.routing_category = category;
        snap.active = rc.is_active();
        snap.app_wait_time_sec = rc.get_app_wait_time();

        for (auto& scope : rc.app_limits().scopes()) {
            if (scope.duration() <= 0 || scope.limit() <= 0) continue;
            BucketSnapshot b;
            b.duration_sec = scope.duration();
            b.limit = scope.limit();
            b.count = scope.count();
            b.seconds_until_reset = scope.seconds_until_reset();
            snap.app_buckets.push_back(b);
        }

        for (auto& [mkey, mhier] : rc.method_limits()) {
            MethodRateSnapshot msnap;
            msnap.method_key = mkey;
            msnap.wait_time_sec = mhier.get_wait_time();
            for (auto& scope : mhier.scopes()) {
                if (scope.duration() <= 0 || scope.limit() <= 0) continue;
                BucketSnapshot b;
                b.duration_sec = scope.duration();
                b.limit = scope.limit();
                b.count = scope.count();
                b.seconds_until_reset = scope.seconds_until_reset();
                msnap.buckets.push_back(b);
            }
            if (!msnap.buckets.empty()) {
                snap.method_limits.push_back(std::move(msnap));
            }
        }
        return snap;
    }

    std::vector<RegionRateSnapshot> RateHandler::get_snapshot() {
        std::lock_guard<std::mutex> lock(this->mutex_);
        std::vector<RegionRateSnapshot> result;

        for (int i = 0; i < NUM_REGIONS; ++i) {
            if (this->region_counts[i].is_active()) {
                result.push_back(build_region_snapshot(
                    this->region_counts[i],
                    args::regional_to_str(static_cast<args::regional>(i)),
                    "Regional"));
            }
        }
        for (int i = 0; i < NUM_PLATFORMS; ++i) {
            if (this->platform_counts[i].is_active()) {
                result.push_back(build_region_snapshot(
                    this->platform_counts[i],
                    args::platform_to_str(static_cast<args::platform>(i)),
                    "Platform"));
            }
        }
        for (int i = 0; i < NUM_VAL_PLATFORMS; ++i) {
            if (this->val_plaform_counts[i].is_active()) {
                result.push_back(build_region_snapshot(
                    this->val_plaform_counts[i],
                    args::val_platform_to_str(static_cast<args::val_platform>(i)),
                    "Valorant"));
            }
        }
        return result;
    }
} // namespace riotcpp::rate
