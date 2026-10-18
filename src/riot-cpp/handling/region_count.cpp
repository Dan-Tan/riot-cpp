#include "region_count.h"
#include "../logging/logger.h"

namespace riotcpp::rate {

    int RegionCount::get_app_wait_time() {
        return this->app_limits_.get_wait_time();
    }

    int RegionCount::get_method_wait_time(const std::string& method_key) {
        auto method_hierachy = this->method_limits_.find(method_key);
        if (method_hierachy == this->method_limits_.end()) {
            return 0;
        }
        return method_hierachy->second.get_wait_time();
    }

    int RegionCount::get_wait_time(const std::string& method_key) {
        int app_wait_time = this->get_app_wait_time();
        int method_wait_time = this->get_method_wait_time(method_key);
        return std::max(app_wait_time, method_wait_time);
    }

    void RegionCount::insert_request(long query_id, unsigned server_time, const std::string& method_key, const std::string& method_limits) {
        this->active_ = true;
        this->app_limits_.insert_request(server_time);
        
        // if exists insert else construct and insert
        auto method_hierachy = this->method_limits_.find(method_key);
        if (method_hierachy != this->method_limits_.end()) {
            logging::get()->debug("[Query {}] Incrementing request count for app limit and method '{}'", query_id, method_key);
            method_hierachy->second.insert_request(server_time);
            return;
        } 
        
        logging::get()->debug("[Query {}] Discovered new method limit for '{}': {}", query_id, method_key, method_limits);
        logging::get()->debug("[Query {}] Incrementing request count for app limit and method '{}'", query_id, method_key);
        RateHierachy new_hierachy = RateHierachy(method_limits);
        new_hierachy.insert_request(server_time);
        this->method_limits_.insert_or_assign(method_key, new_hierachy);
    }

    void RegionCount::insert_request_with_headers(long query_id,
                                                  unsigned server_time,
                                                  const std::string& method_key,
                                                  const std::string& app_limits,
                                                  const std::string& app_counts,
                                                  const std::string& method_limits,
                                                  const std::string& method_counts,
                                                  int retry_after_sec) {
        this->active_ = true;
        this->app_limits_.sync_from_headers(app_limits, app_counts, server_time, retry_after_sec);

        auto method_hierachy = this->method_limits_.find(method_key);
        if (method_hierachy == this->method_limits_.end()) {
            RateHierachy new_hierachy = RateHierachy(method_limits);
            new_hierachy.sync_from_headers(method_limits, method_counts, server_time, retry_after_sec);
            this->method_limits_.insert_or_assign(method_key, std::move(new_hierachy));
        } else {
            method_hierachy->second.sync_from_headers(method_limits, method_counts, server_time, retry_after_sec);
        }
    }

    std::string RegionCount::to_string() const {
        std::stringstream reg_stream;
        reg_stream << "Application Limit\n " << this->app_limits_.to_string() << "Method Limits\n";
        for (const auto& key_pair : this->method_limits_) {
            reg_stream << key_pair.first;
            reg_stream << key_pair.second.to_string();
        }
        return reg_stream.str();
    }

    void RegionCount::init_limits(const std::vector<int>& durations, const std::vector<int>& limits, const std::vector<int>& counts) {
        this->app_limits_ = RateHierachy(durations, limits, counts);
    }

    void RegionCount::init_limits(const std::string& description) {
        this->app_limits_ = RateHierachy(description);
    }


} // namespace riotcpp::rate
