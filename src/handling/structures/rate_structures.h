#pragma once
namespace handler_structs {

    typedef struct ScopeHistory {

        int duration;
        int limit;
        std::queue<time_t> history;

        void update_history();
        void correct_history(int server_counter, int server_limit, int server_duration);
        std::time_t validate_request();

        inline void insert_request(std::time_t server_time) {
            history.push(server_time); 
        }

        inline void remove_request() {
            history.pop();
        }

        inline int available_requests() {
            update_history();
            return limit - history.size();
        }

    } ScopeHistory;

    typedef struct RegionHistory {
        
        std::vector<ScopeHistory> application_hierachy;
        std::unordered_map<std::string_view, std::vector<ScopeHistory>> method_queues;

        void update_scopes();
        void insert_request(std::time_t server_time, std::string_view method_key, std::string_view method_limits);
        std::time_t validate_request(std::string_view method_key);

    } RegionHistory;

    RegionHistory init_region(std::vector<int> limits, std::vector<int> durations);
}

