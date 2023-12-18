#pragma once

#include "region_count.h"
#include "../query/query.h"
#include "../types/args.h"
#include "../logging/logger.h"

namespace riotcpp {
namespace rate {

    struct ResponseHandler {
        ResponseHandler(logging::Logger *logger) {this->_logger = logger;};
        bool review_request(std::shared_ptr<query::query> request);
        bool validate_request(std::shared_ptr<query::query> request) {return true;};

        std::array<std::array<int, 2>, 20> response_errors;
        logging::Logger *_logger;

        int MAX_INTERNAL_DENIALS = 2; // 500
        int MAX_SERVICE_UNAVAILABLE = 2; // 503
    };


    class RequestHandler {
        public:
            RequestHandler(logging::Logger *logger) : rate_handler(logger), response_handler(logger) {};
            ~RequestHandler() = default;

            inline bool review_request(std::shared_ptr<query::query> request) {
                this->rate_handler.review_request(request); // insert_request only 200
                return this->response_handler.review_request(request);
            };
            inline bool validate_request(std::shared_ptr<query::query> request) {
                return this->rate_handler.validate_request(request);
            };
        private:
            struct RateHandler rate_handler;
            struct ResponseHandler response_handler;
    };
};
}
