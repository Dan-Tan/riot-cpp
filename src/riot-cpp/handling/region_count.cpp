
#include "region_count.h"
namespace riotcpp {
namespace rate {

    int RegionCount::get_wait_time(const std::string& method_key) {
        int app_wait_time = this->app_limits_.get_wait_time();

        auto method_hierachy = this->method_limits_.find(method_key);

        if (method_hierachy == this->method_limits_.end()) {
            return app_wait_time;
        }

        int method_wait_time = method_hierachy->second.get_wait_time();
        return std::max(app_wait_time, method_wait_time);
    }

    void RegionCount::insert_request(unsigned server_time, const std::string& method_key, const std::string& method_limits) {
        this->app_limits_.insert_request(server_time);
        
        // if exists insert else construct and insert
        auto method_hierachy = this->method_limits_.find(method_key);
        if (method_hierachy != this->method_limits_.end()) {
            method_hierachy->second.insert_request(server_time);
            return;
        } 

        RateHierachy new_hierachy = RateHierachy(method_limits);
        new_hierachy.insert_request(server_time);
        this->method_limits_.insert_or_assign(method_key, RateHierachy(method_limits));
    }

    std::string RegionCount::to_string() const {
        std::stringstream ss;
        ss << "Application Limit\n " << this->app_limits_.to_string() << "Method Limits\n";
        for (auto& key_pair : this->method_limits_) {
            ss << key_pair.first;
            ss << key_pair.second.to_string();
        }
        return ss.str();
    }

    void RegionCount::init_limits(const std::vector<int>& durations, const std::vector<int>& limits, const std::vector<int>& counts) {
        this->app_limits_ = RateHierachy(durations, limits, counts);
    }

    void RegionCount::init_limits(const std::string& description) {
        this->app_limits_ = RateHierachy(description);
    }


}
}
