#pragma once
#include "structures/rate_structures.h"
#include "../query/query.h"

namespace handler {

    struct ResponseHandler {
        bool review_request(std::shared_ptr<query::query> request);
        bool validate_request(std::shared_ptr<query::query> request) {return true;};

        std::array<std::array<int, 2>, 20> response_errors;

        int MAX_INTERNAL_DENIALS = 2; // 500
        int MAX_SERVICE_UNAVAILABLE = 2; // 503
    };

    struct RateHandler {
        bool validate_request(std::shared_ptr<query::query> request);
        void review_request(std::shared_ptr<query::query> request);

        void init_queues(std::shared_ptr<query::query> request);
        void update_queues();

        std::array<handler_structs::RegionHistory, 20> routing_queues;

        bool initialised = false;
    };

    class RequestHandler {
        public:
            RequestHandler() = default;
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
