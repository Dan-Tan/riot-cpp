#pragma once

#include "region_count.h"
#include "../query/query.h"
#include "../types/args.h"
#include "../logging/logger.h"
#include "rate_handler.h"

namespace riotcpp {
namespace rate {

    struct ResponseHandler {
        ResponseHandler(logging::Logger *logger) {this->_logger = logger;};
        bool review_request(std::shared_ptr<query::query> request);
        bool validate_request(std::shared_ptr<query::query> request) {return true;};

        std::array<std::array<int, 2>, NUM_PLATFORMS>     platform_errors;
        std::array<std::array<int, 2>, NUM_REGIONS>       region_errors;
        std::array<std::array<int, 2>, NUM_VAL_PLATFORMS> val_platform_errors;
        logging::Logger *_logger;

        int MAX_INTERNAL_DENIALS = 2; // 500
        int MAX_SERVICE_UNAVAILABLE = 2; // 503

        private:
            bool handle_server_error(const long code, const args::routing);
            void reset_server_error_count(const args::routing&);
    };


    class RequestHandler {
        public:
            RequestHandler(logging::Logger *logger) : rate_handler(), response_handler(logger) {};
            ~RequestHandler() = default;

            inline bool review_request(std::shared_ptr<query::query> request) {
                this->rate_handler.check_rate_limits(request); // insert_request only 200
                return this->response_handler.review_request(request);
            };
            inline bool validate_request(std::shared_ptr<query::query> request) {
                this->rate_handler.insert_request(request);
                return true;
            };
        private:
            RateHandler rate_handler;
            ResponseHandler response_handler;
    };
};
}
