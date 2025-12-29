#include <numeric>
#include <utility>
#include <vector>

#include "rate_hierachy.h"

namespace riotcpp::rate {

    static inline int chars_to_int(char nums[10], int num_digits) {
        int to_return = 0;
        for (int i = 0; i < num_digits; i++) {
            to_return += ((int) (nums[i] - '0')) * (10^(num_digits - i - 1));  
        }
        return to_return;
    }

    static std::pair<std::vector<int>, std::vector<int>> extract_duration_limits(const std::string& description) {
        // this function assumes valid format of descriptor "20:1, 100:120"...
        int num_digits = 0;
        char nums[10];
        bool limit_add = true;
        std::vector<int> limit;
        std::vector<int> duration;

        for (const char& pos_digit : description) {
            if (std::isdigit(pos_digit)) {
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

    RateHierachy::RateHierachy(const int durations[], const int limits[], const int counts[], const unsigned size) {
        this->hierachy_.reserve(size);
        for (int i = 0; i < size; i++) {
            this->hierachy_.push_back(ScopeCount(durations[i], limits[i], counts[i]));
        }
    }

    RateHierachy::RateHierachy(const std::vector<int>& durations, const std::vector<int>& limits, const std::vector<int>& counts) {
        rcp_assert(durations.size() == limits.size(), "Mismatched Duration and Limit sized given");
        rcp_assert(counts.size() == limits.size(), "Mismatched Count and Limit sized given");
        this->hierachy_.reserve(durations.size());
        for (int i = 0; i < durations.size(); i++) {
            this->hierachy_.push_back(ScopeCount(durations[i], limits[i], counts[i]));
        }
    }

    RateHierachy::RateHierachy(const std::string& description) {

        std::pair<std::vector<int>, std::vector<int>> description_limit = extract_duration_limits(description);

        int size = description_limit.first.size();
        this->hierachy_.reserve(size);

        for (int i = 0; i < size; i++) {
            this->hierachy_.push_back(ScopeCount(description_limit.first[i], description_limit.second[i], 0));
        }
    }

    int RateHierachy::get_wait_time() {
        rcp_assert(this->hierachy_.size() > 0, "Empty scope hierachy, check if initialised properly");
        const auto find_longest = [](int current_max, ScopeCount& next_scope){
            int next_wait = next_scope.get_wait_time();
            return next_wait * (next_wait > current_max) + current_max * (next_wait <= current_max);
        };
        return std::accumulate(this->hierachy_.begin(), this->hierachy_.end(), 0, find_longest);
    }

    void RateHierachy::insert_request(unsigned server_time) {
        for (ScopeCount& scope_count : this->hierachy_) {
            scope_count.insert_request(server_time);
        }
    }

    std::string RateHierachy::to_string() const {
        std::stringstream ss;
        for (auto& scope_count : this->hierachy_) {
            ss << scope_count.to_string() << '\n';
        }
        return ss.str();
    }
} // namespace riotcpp::rate
