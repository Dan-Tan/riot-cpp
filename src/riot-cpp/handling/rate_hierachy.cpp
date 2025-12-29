#include <numeric>
#include <utility>
#include <vector>

#include "rate_hierachy.h"

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

    RateHierachy::RateHierachy(const std::vector<int>& durations, const std::vector<int>& limits, const std::vector<int>& counts) {
        rcp_assert(durations.size() == limits.size(), "Mismatched Duration and Limit sized given");
        rcp_assert(counts.size() == limits.size(), "Mismatched Count and Limit sized given");
        this->hierachy_.reserve(durations.size());
        for (int i = 0; i < durations.size(); i++) {
            this->hierachy_.emplace_back(durations[i], limits[i], counts[i]);
        }
    }

    RateHierachy::RateHierachy(const std::string& description) {

        std::pair<std::vector<int>, std::vector<int>> description_limit = extract_duration_limits(description);

        int size = description_limit.first.size();
        this->hierachy_.reserve(size);

        for (int i = 0; i < size; i++) {
            this->hierachy_.emplace_back(description_limit.first[i], description_limit.second[i], 0);
        }
    }

    int RateHierachy::get_wait_time() {
        rcp_assert(this->hierachy_.size() > 0, "Empty scope hierachy, check if initialised properly");
        const auto find_longest = [](int current_max, ScopeCount& next_scope){
            int next_wait = next_scope.get_wait_time();
            return (next_wait * (next_wait > current_max)) + (current_max * (next_wait <= current_max));
        };
        return std::accumulate(this->hierachy_.begin(), this->hierachy_.end(), 0, find_longest);
    }

    void RateHierachy::insert_request(unsigned server_time) {
        for (ScopeCount& scope_count : this->hierachy_) {
            scope_count.insert_request(server_time);
        }
    }

    std::string RateHierachy::to_string() const {
        std::stringstream rh_stream;
        for (const auto& scope_count : this->hierachy_) {
            rh_stream << scope_count.to_string() << '\n';
        }
        return rh_stream.str();
    }
} // namespace riotcpp::rate
