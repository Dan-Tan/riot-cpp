#include <chrono>
#include <iomanip>
#include <thread>
#include <vector>
#include <ctime>
#include <stdexcept>
#include <queue>
#include <unordered_map>
#include "handlers.h"

namespace riotcpp {
namespace rate {

    /**
     * Update server error tracker and indicate whether or not a request should be resent.
     * Return false to not resent and true to resend.
     */
    bool ResponseHandler::handle_server_error(const long response_code, const args::routing route) {
        std::size_t ind = response_code == 500 ? 0 : 1; 
        int limit = response_code == 500 ? this->MAX_INTERNAL_DENIALS : this->MAX_SERVICE_UNAVAILABLE;
        switch (route.indicator) {
            case REGIONAL_INDICATOR:
                this->region_errors[static_cast<int>(route.routng.reg)][ind] += 1;
                return !(this->region_errors[static_cast<int>(route.routng.reg)][ind] >= limit);
            case PLATFORM_INDICATOR:
                this->platform_errors[static_cast<int>(route.routng.pltform)][ind] += 1;
                return !(this->platform_errors[static_cast<int>(route.routng.pltform)][ind] >= limit);
            case VAL_PLATFORM_INDICATOR:
                this->val_platform_errors[static_cast<int>(route.routng.vpltform)][ind] += 1;
                return !(this->val_platform_errors[static_cast<int>(route.routng.vpltform)][ind] >= limit);
            default:
                throw std::invalid_argument("Invalid route indicator: " + std::to_string(route.indicator));
        }
        return false;
    }

    void ResponseHandler::reset_server_error_count(const args::routing& route) {

        switch (route.indicator) {
            case REGIONAL_INDICATOR:
                this->region_errors[static_cast<int>(route.routng.reg)] ={0, 0};
            case PLATFORM_INDICATOR:
                this->platform_errors[static_cast<int>(route.routng.pltform)] ={0, 0};
            case VAL_PLATFORM_INDICATOR:
                this->val_platform_errors[static_cast<int>(route.routng.vpltform)] ={0, 0};
            default:
                throw std::invalid_argument("Invalid route indicator: " + std::to_string(route.indicator));
        }
    }

    bool ResponseHandler::review_request(std::shared_ptr<query::query> request) {
        long response_code = request->last_response;
        if (response_code == 200 || response_code == -2 || response_code == 429) {
            this->reset_server_error_count(request->route);
            return true;
        } else if (response_code == 500) {
            return this->handle_server_error(response_code, request->route);
        } else {
            return false;
        }
        return true;
    }
}
}
