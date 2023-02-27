#include <chrono>
#include <iostream>
#include <iomanip>
#include <thread>
#include <vector>
#include <ctime>
#include <stdexcept>
#include <queue>
#include <unordered_map>
#include <jsoncpp/json/json.h>
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

inline static time_t request_time(std::string server_time) {
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

bool RateHandler::validate_request(std::shared_ptr<query::query> request) {
    int wait_time = this->routing_queues.at(routing_to_int(request->routing_value)).validate_request(request->method_key);
    if (wait_time == 0) {
        (*this->_logger) << logging::LEVEL::DEBUG << request->method_key << "No wait time" << 0;
        return true;
    } else {
        const std::time_t c_time = std::time(NULL);
        char wait = wait_time + '0';
        (*this->_logger) << logging::LEVEL::INFO << request->method_key << std::string("Rate Limiting, Waiting ") + wait + " seconds" << 0;
        request->send_time = std::mktime(std::gmtime(&c_time)) + wait_time;
        return false;
    }
}

void RateHandler::update_queues() {
    for (auto& region: this->routing_queues) {
        region.update_scopes();
    }
}

void RateHandler::init_queues(std::shared_ptr<query::query> request) {
    try {
        std::string limits_str = request->response_header["X-App-Rate-Limit"].asString();
        std::string method_str = request->response_header["X-Method-Rate-Limit"].asString();
        std::vector<int> limits;
        std::vector<int> durations;

        int comma_index = 0;
        int beginning_index = 0;
        
        while (comma_index != -1) {
            comma_index = limits_str.find(',', beginning_index);
            limitDurationExtraction(limits_str.substr(beginning_index), comma_index-beginning_index, limits, durations);
            beginning_index = comma_index+1;
        }

        for (auto& reg : this->routing_queues) {
            reg = handler_structs::init_region(limits, durations);
        }

        this->routing_queues.at(routing_to_int(request->routing_value)).insert_request(request_time(request->response_header["Date"].asString()), request->method_key, method_str);
    }
    catch (Json::Exception& exc ) {
        throw Json::Exception("Response header key value not found");
    }
}

void RateHandler::review_request(std::shared_ptr<query::query> request) {
    if (!this->initialised && request->last_response != -2) {
        this->initialised = true;
        this->init_queues(request);
        return;
    }
    if (request->last_response == 429) {
        const std::time_t c_time = std::time(NULL);
        request->send_time = std::mktime(std::gmtime(&c_time)) + static_cast<std::time_t>(fast_atoi(request->response_header["Retry-After"].asString().data()));
        try {
            std::string limit_type = request->response_header["X-Rate-Limit-Type"].asString();
            if (limit_type == "application") {
                std::cout << "application" << '\n';
            } else if (limit_type == "method") {
                std::cout << "method" << '\n';
            } else if (limit_type == "service") {
                std::cout << "service" << '\n';
            } else { // unknown limit type
                return;
            }
        } 
        catch (Json::Exception& exc) { //underlying service limit
            std::cout << "503: underlying" << '\n';
        }
    }
    if (request->last_response != 200) {
        return;
    } else {
        this->routing_queues.at(routing_to_int(request->routing_value)).insert_request(request_time(request->response_header["Date"].asString()), request->method_key, request->response_header["X-Method-Rate-Limit"].asString());
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
