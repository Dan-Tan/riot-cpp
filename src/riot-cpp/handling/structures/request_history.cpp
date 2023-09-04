#include <string>
#include <numeric>
#include <stdexcept>
#include <memory>
#include <iomanip>
#include "rate_structures.h"

namespace handler_structs {


static int fast_atoi(const char * str, int iters = -1)
{
    int val = 0;
    if (iters < 0) {
        while ( *str ) {
            val = val*10 + (*str++ - '0');
        }
        return val;
    }
    for (int i = 0; i < iters; i++ ) {
        val = val*10 + (*str++ - '0');
    }
    return val;
}

static void limitDurationExtraction(std::string_view header_strings, int iters, std::vector<int>& limits, std::vector<int>& durations) {
    int colon_index = header_strings.find(':');
    int limit = fast_atoi(header_strings.data(), colon_index);
    int duration = fast_atoi(header_strings.substr(colon_index+1).data(), iters - colon_index - 1);
    limits.push_back(limit);
    durations.push_back(duration);
}

static std::vector<ScopeHistory> init_method_hierachy(std::string_view method_limits, std::time_t server_time) {
    
    std::vector<ScopeHistory> method_hierachy;
    std::vector<int> limits;
    std::vector<int> durations;

    int comma_index = 0;
    int beginning_index = 0;

    while (comma_index != -1) {
        comma_index = method_limits.find(',', beginning_index);
        limitDurationExtraction(method_limits.substr(beginning_index), comma_index - beginning_index, limits, durations);
        beginning_index = comma_index + 1;
    }

    for (int i = 0; i < limits.size(); i++) {
        method_hierachy.push_back({durations.at(i), limits.at(i)});
        method_hierachy.back().insert_request(server_time);
    }

    return method_hierachy;
}

void ScopeHistory::update_history() {


    if (this->history.size() == 0) {return;};
    bool remove = true;
    const std::time_t ct = std::time(NULL);
    std::time_t current_t = std::mktime(std::gmtime(&ct));
    while (remove) {
        if (current_t - history.front() > duration) {
            this->history.pop();
            remove = !(this->history.empty());
        } else {
            return;
        }
    }
    return;
}

int ScopeHistory::validate_request() {
    update_history();

    if (history.size() < limit) {
        return 0;
    } else {
        const std::time_t ct = std::time(NULL);
        std::time_t c_time = std::mktime(std::gmtime(&ct));
        int delay = duration - static_cast<int>(c_time - history.front()) + 1; // add one to catch rounding errors :(
        return delay;
    }
}

void ScopeHistory::correct_history(int server_counter, int server_limit, int server_duration) {
    update_history();
    int residue = history.size() - server_counter;
    limit = server_limit;
    duration = server_duration;
    
    if (residue == 0) {
        return;
    } else if (residue > 0) {
        for (int i = 0; i < residue; i++) {
            history.pop();
        }
    } else {
        const std::time_t ct = std::time(NULL);
        std::time_t current = std::mktime(std::gmtime(&ct));
        for (int i = 0; i < -residue; i++) {
            insert_request(current);
        }
    }
}

void RegionHistory::update_scopes() {
    for (auto& scope : application_hierachy) {
        scope.update_history();
    }
    for (auto& [method_key, method_queue] : this->method_queues) {
        for (auto& m_queue : method_queue) {
            m_queue.update_history();
        }
    }
}

int RegionHistory::validate_request(std::string method_key) {
    
    int wait_time = 0;
    bool no_limits = true;
    int hierachy = 0;
    wait_time = std::accumulate(application_hierachy.begin(), application_hierachy.end(), 0, 
            [](int acc, ScopeHistory new_scope){if (int n = new_scope.validate_request();n > acc) {return n;} else {return acc;}});
    int method_time;
    try {
        method_time = std::accumulate(method_queues.at(method_key).begin(), method_queues.at(method_key).end(), 0, [](int acc, ScopeHistory new_scope){if (int n = new_scope.validate_request();n > acc) {return n;} else {return acc;}});
    }
    catch (std::out_of_range& exc) {
        method_time = 0;
    }
    if (method_time > wait_time) {
        return method_time;
    }
    return wait_time;
}

void RegionHistory::insert_request(std::time_t server_time, std::string_view method_key, std::string_view method_limits) {

    update_scopes();
    
    for (auto& history : this->application_hierachy) {
        history.insert_request(server_time);
    }

    std::string n_method_key{method_key.data()};

    try {
        for (auto& scope : method_queues.at(n_method_key)) {
            scope.insert_request(server_time);
        }
    }
    catch (std::out_of_range& exc) {// insert new method scope
        std::vector<ScopeHistory> new_method_hierachy = init_method_hierachy(method_limits, server_time);
        method_queues.insert(std::pair<std::string_view, std::vector<ScopeHistory>>(n_method_key, new_method_hierachy));
    }
}

RegionHistory init_region(std::vector<int> limits, std::vector<int> durations) {

    RegionHistory new_history;
    
    for (int i = limits.size()-1; i >= 0; i--) {
        new_history.application_hierachy.push_back({.duration = durations[i], .limit = limits[i]});
    }

    return new_history;
}

const std::string ScopeHistory::queue_state() const {
    if (this->history.empty()) {
        return "Empty History";
    }
    std::stringstream state;
    state << "oldest: " << std::put_time(std::gmtime(&this->history.front()), "%a, %d %m %Y %H:%M:%S GMT");
    state << "  newest: " << std::put_time(std::gmtime(&this->history.back()), "%a, %d %m %Y %H:%M:%S GMT");
    return state.str();
}

}
