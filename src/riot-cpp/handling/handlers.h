#pragma once

#include "../query/endpoints.h"
#include "../types/args.h"
#include "../logging/logger.h"
#include "rate_handler.h"

namespace riotcpp::rate {

    struct ResponseHandler {
        explicit ResponseHandler() = default;
        bool review_request(const std::shared_ptr<query::query>& request);
        static bool validate_request(const std::shared_ptr<query::query>& request) {return true;};

        std::array<std::array<int, 2>, NUM_PLATFORMS>     platform_errors;
        std::array<std::array<int, 2>, NUM_REGIONS>       region_errors;
        std::array<std::array<int, 2>, NUM_VAL_PLATFORMS> val_platform_errors;

        int MAX_INTERNAL_DENIALS = 2; // 500
        int MAX_SERVICE_UNAVAILABLE = 2; // 503

        private:
            bool handle_server_error(long code, args::routing);
            void reset_server_error_count(const args::routing&);
    };


    class RequestHandler {
        public:
            explicit RequestHandler() = default;
            ~RequestHandler() = default;

            bool is_initialized() const {
                return this->rate_handler.is_initialized();
            }

            void check_rate_limits(const std::shared_ptr<query::query>& request) {
                this->rate_handler.check_rate_limits(request);
            }

            bool review_request(const std::shared_ptr<query::query>& request) {
                return this->response_handler.review_request(request);
            };
            
            bool validate_request(const std::shared_ptr<query::query>& request) {
                this->rate_handler.insert_request(request);
                return true;
            };

            WaitTimeBreakdown get_wait_time_breakdown(const args::routing& route, const std::string& method_key) {
                return this->rate_handler.get_wait_time_breakdown(route, method_key);
            }

            std::vector<RegionRateSnapshot> get_snapshot() {
                return this->rate_handler.get_snapshot();
            }
        private:
            RateHandler rate_handler;
            ResponseHandler response_handler;
    };
} // namespace riotcpp::rate
