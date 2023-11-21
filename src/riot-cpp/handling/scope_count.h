#pragma once

#include <string>
#include <vector>
#include <queue>
#include <chrono>
#include <string_view>
#include <unordered_map>

namespace rate {
    
    /**
     * ScopeCount attempts to track the state of Riot's rate limiter to avoid breaching limits
     * A ScopeCount tracks only one limit of either an application or method limit
     * Any request made to a given endpoint within a certain region must call get_wait_time() before sending
     */
    class ScopeCount {

        private: 

            int duration_            = 0;
            int limit_               = 0;
            int count_               = 0;
            unsigned next_reset_     = 0;

            unsigned update_count(bool update_reset = false);

        public:

            ScopeCount(int duration, int limit, int count);
            ~ScopeCount() = default;

            inline void correct_count(int server_counter, int server_limit, int server_duration);
            inline void insert_request(unsigned server_time);
            inline int  get_wait_time();
            inline int  n_available();

            std::string to_string() const;

    };
  
    /**
     * @param  update flag, if we haven't made a new request we do not want to update the reset time as this will possibly desync with 
     * riot's timer
     * @return current time to avoid multiple time requests
     */
    inline unsigned ScopeCount::update_count(bool update_reset) {
        unsigned current_time = std::chrono::utc_clock::now().time_since_epoch().count();

        if (current_time <= this->next_reset_) {
            return current_time;
        }
        this->count_ = 0;

        if (update_reset) {
            this->next_reset_ = current_time + this->duration_;
        }
        return current_time;
    }

    inline ScopeCount::ScopeCount(int duration, int limit, int count) {
        this->duration_ = duration;
        this->limit_ = limit;
        this->count_ = count;
    }

    inline void ScopeCount::insert_request(unsigned server_time) {
        this->update_count();
        this->count_ = this->count_ + 1 * (server_time < this->next_reset_); // only increment if occured within current window 
    }

    inline int ScopeCount::n_available() {
        this->update_count();
        return this->limit_ - this->count_;
    }

    inline int ScopeCount::get_wait_time() {
        unsigned current_time = this->update_count(true);
        return 0 + (this->next_reset_ - current_time) * (this->count_ >= this->limit_);
    }

    inline void ScopeCount::correct_count(int server_count, int server_limit, int server_duration) {
        this->duration_ = server_duration;
        this->limit_    = server_limit;
        this->count_    = server_count;
    }

    inline std::string ScopeCount::to_string() const {
        std::stringstream ss;
        ss << "Next Reset: " << this->next_reset_ << " Duration: " << this->duration_ << " Limit: " << this->limit_ << " Count: " << this->count_;
        return ss.str();
    }
}
