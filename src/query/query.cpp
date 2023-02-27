#include <stdexcept>
#include <functional>
#include <iostream>
#include <chrono>
#include <thread>
#include <queue>
#include <ctime>
#include "../client/client.h"
#include "query.h"

namespace client {

static inline void wait_until(std::time_t send_time) {
    const std::time_t c_time = std::time(NULL);
    std::time_t current_time = std::mktime(std::gmtime(&c_time));
    if (current_time >= send_time) {
        return;
    } else {
        std::this_thread::sleep_for(std::chrono::seconds(send_time - current_time));
        return;
    }
}

Json::Value RiotApiClient::query(std::shared_ptr<query::query> request) {

    this->logger << logging::LEVEL::DEBUG << "--Query Call--" << request->url << 0;

    while (this->request_handler.review_request(request)) {
        if (request->last_response == 200) {
            return request->response_content;
        } 
        if (!this->request_handler.validate_request(request)) {
            this->logger << logging::LEVEL::WARNING << "Request sent was invalid or the server is unavailable" << 0;
            throw std::runtime_error("Request sent was invalid or the server is unavailable");
        }
        wait_until(request->send_time);
        this->get(request);
    }

    this->logger << logging::LEVEL::ERROR << "Failed request" << request->method_key << request->last_response << 0; 
    return request->response_content;
}
}

