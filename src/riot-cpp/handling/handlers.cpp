#include <chrono>
#include <iomanip>
#include <thread>
#include <vector>
#include <ctime>
#include <stdexcept>
#include <queue>
#include <unordered_map>
#include "handlers.h"

namespace handler {

static std::size_t routing_to_int(std::string_view routing_value);

static int fast_atoi(const char * str, int iters = -1)
{
    int val = 0;
    if (iters < 0) {
        while( *str ) {
            val = val*10 + (*str++ - '0');
        }
        return val;
    } else {
        for (int i = 0; i < iters; i++) {
            val = val*10 + (*str++ - '0');
        }
    }
    return val;
}

inline static std::time_t request_time(std::string server_time) {
    std::tm tm_time = {0};
    std::stringstream(server_time) >> std::get_time(&tm_time, "%a, %d %b %y %H:%M:%S GMT"); //riot api time format
    return std::mktime(&tm_time);
};

static void limitDurationExtraction(std::string_view header_strings, int iters, std::vector<int>& limits, std::vector<int>& durations) {
    int colon_index = header_strings.find(':');
    int limit = fast_atoi(header_strings.data(), colon_index);
    int duration = fast_atoi(header_strings.substr(colon_index+1).data(), iters - colon_index - 1);
    limits.push_back(limit);
    durations.push_back(duration);
}

void RateHandler::init_counts(std::string_view routing, std::string method_key, const std::vector<int>& app_counts, const std::vector<int>& method_counts, const std::time_t server_time) {

    std::queue<std::time_t>& history_ref = this->routing_queues.at(routing_to_int(routing)).application_hierachy[0].history;

    for (int i = 0; i < app_counts.size(); i++) {
        history_ref = this->routing_queues.at(routing_to_int(routing)).application_hierachy[i].history;
        for (int j = 0; j < app_counts[i]-1; j++) {
            history_ref.push(server_time);
        }
    }

    for (int i = 0; i < method_counts.size(); i++) {
        history_ref = this->routing_queues[routing_to_int(routing)].method_queues[method_key][i].history;
        for (int j = 0; j < method_counts[i]-1; j++) {
            history_ref.push(server_time);
        }
    }
}

bool RateHandler::validate_request(std::shared_ptr<query::query> request) {
    if (request->last_response == 429 || request->last_response == 500 || request->last_response == 503) { // send time already calculated
        (*this->_logger) << logging::LEVEL::DEBUG << request->method_key << "Retrying" << request->last_response << 0;
        return true;
    }
    if (request->last_response == -2) {
        int wait_time = this->routing_queues.at(routing_to_int(request->routing_value)).validate_request(request->method_key);
        (*this->_logger) << logging::LEVEL::DEBUG << this->routing_queues.at(routing_to_int(request->routing_value)).application_hierachy << 0; // continuing log
        if (wait_time == 0) {
            (*this->_logger) << logging::LEVEL::DEBUG << request->method_key << "No wait time" << 0;
            return true;
        } else {
            const std::time_t c_time = std::time(NULL);
            (*this->_logger) << logging::LEVEL::INFO << request->method_key << std::string("Self Rate Limiting, Waiting ") + std::to_string(wait_time) + " seconds" << request->response_header << 0; // continuing log
            (*this->_logger) << logging::LEVEL::DEBUG << this->routing_queues.at(routing_to_int(request->routing_value)).application_hierachy << 0; // continuing log
            request->send_time = std::mktime(std::gmtime(&c_time)) + wait_time + 1;
            return true;
        }
    }
    (*this->_logger) << logging::LEVEL::DEBUG << this->routing_queues.at(routing_to_int(request->routing_value)).application_hierachy << 0; // continuing log
    return false;
}

void RateHandler::update_queues() {
    for (auto& region: this->routing_queues) {
        region.update_scopes();
    }
}

void get_limits_and_counts(std::string_view rep, std::vector<int>& count, std::vector<int>& duration) {

    int comma_index = 0;
    int beginning_index = 0;
    
    while (comma_index != -1) {
        comma_index = rep.find(',', beginning_index);
        limitDurationExtraction(rep.substr(beginning_index), comma_index-beginning_index, count, duration);
        beginning_index = comma_index+1;
    }
}

void RateHandler::init_queues(std::shared_ptr<query::query> request) {
    this->initialised = true;
    std::string limits_str = request->response_header.app_limit;
    std::string counts_str = request->response_header.app_limit_count;
    std::string method_str = request->response_header.method_limit;
    std::string method_counts_str = request->response_header.method_limit_count;
    std::vector<int> method_counts;
    std::vector<int> counts;
    std::vector<int> limits;
    std::vector<int> durations;
    std::vector<int> temp;
    
    get_limits_and_counts(limits_str, limits, durations);
    get_limits_and_counts(counts_str, counts, temp);
    get_limits_and_counts(method_counts_str, method_counts, temp);

    for (auto& reg : this->routing_queues) {
        reg = handler_structs::init_region(limits, durations);
    }

    const std::time_t server_time = request_time(request->response_header.date);

    this->routing_queues.at(routing_to_int(request->routing_value)).insert_request(server_time, request->method_key, method_str);
    this->init_counts(request->routing_value, request->method_key, counts, method_counts, server_time);
}


void RateHandler::review_request(std::shared_ptr<query::query> request) {
    if (!this->initialised && request->last_response != -2) {
        this->init_queues(request);
        (*this->_logger) << logging::LEVEL::DEBUG << "Queues Initialised" << 0;
        return;
    }
    if (request->last_response == 429) {
        const std::time_t c_time = std::time(NULL);
        request->send_time = std::mktime(std::gmtime(&c_time)) + static_cast<std::time_t>(fast_atoi(request->response_header.retry_after));
        (*this->_logger) << logging::LEVEL::ERRORS << this->routing_queues.at(routing_to_int(request->routing_value)).application_hierachy << 0;
        (*this->_logger) << logging::LEVEL::ERRORS << request->response_header << 0;
    }
    if (request->last_response == -2) {
        (*this->_logger) << logging::LEVEL::DEBUG << "Ignoring... request not sent yet" << 0;
    }
    else if (request->last_response != 200) {
        (*this->_logger) << logging::LEVEL::INFO << "Unsuccussful Request" << 0;
        return;
    } else {
        (*this->_logger) << logging::LEVEL::DEBUG << "Successful Request" << 0;
        this->routing_queues.at(routing_to_int(request->routing_value)).insert_request(request_time(request->response_header.date), request->method_key, request->response_header.method_limit);
        return;
    }
}

bool ResponseHandler::review_request(std::shared_ptr<query::query> request) {
    long last_response = request->last_response;
    if (last_response == 200 || last_response == -2 || last_response == 429) {
        auto& errs = this->response_errors.at(routing_to_int(request->routing_value));
        errs[0] = 0; errs[1] = 0;
        return true;
    } else if (last_response == 500) {
        this->response_errors.at(routing_to_int(request->routing_value))[0] += 1;
        if (this->response_errors.at(routing_to_int(request->routing_value))[0] >= this->MAX_INTERNAL_DENIALS) {
            return false;
        };
    } else if (last_response == 503) {
        this->response_errors.at(routing_to_int(request->routing_value))[1] += 1;
        if (this->response_errors.at(routing_to_int(request->routing_value))[1] >= this->MAX_SERVICE_UNAVAILABLE) {
            return false;
        };
    } else {
        return false;
    }
    return true;
}

static std::size_t routing_to_int(std::string_view routing_value) { //name a more disgusting function
    int sz = routing_value.size();
    char first_char = routing_value[0];
    if (first_char < 110) {
        if (first_char <= 98) {
            if (first_char < 69) {
                return routing_value[1] == 'M' ? 0 : 1;
            } else {
                return first_char == 'b' ? 2 : routing_value[1] == 'S' ? 3 : 4;
            }
        } else {
            if (first_char <= 106) {
                return first_char == 'j' ? 5 : routing_value[2] == 'w' ? 6 : 7;
            } else {
                return first_char == 'k' ? 8 : routing_value[2] == '1' ? 9 : 10;
            }
        }
    } else {
        if (first_char <= 115) {
            if (first_char <= 112) {
                return first_char == 'p' ? 11 : first_char == 'o' ? 12 : 13;
            } else {
                return first_char == 'r' ? 14 : 15;
            }
        } else {
            if (first_char == 'v') {
                return 16;
            } else {
                if (routing_value[2] == '1') {
                    return 17;
                } else {
                    if (routing_value[1] == 'h') {
                        return 18;
                    } else {
                        return 19;
                    }
                }
            }
        }
    }
}

}
