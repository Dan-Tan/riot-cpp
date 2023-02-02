#include <string>
#include <numeric>
#include <ctime>
#include <vector>
#include <utility>
#include <stdexcept>
#include "query.h"

namespace query {

using opt_args = std::pair<std::string, std::string>;

QueryType::~QueryType() {};

static inline std::string construct_args(const std::string& accumm, const opt_args& optional_arg) {
    return accumm + optional_arg.first + "=" + optional_arg.second + "&";
}

std::string QueryType::query_params(const std::vector<opt_args> optional_args){
    if (!this->takes_optional_arguments && optional_args.size() != 0) {
        throw std::invalid_argument("This query type does not accept optional arguements");
    } else if (optional_args.size() == 0) {
        return std::string("");
    } else {
        return std::accumulate(optional_args.begin(), --optional_args.end(), std::string("?"), construct_args);
    }
}

U::~U() {};

std::string U::construct_url(const std::vector<std::string>& query_params, const std::vector<opt_args>& optional_args) {
    return this->base_url(query_params.at(0)) + this->u1 + this->query_params(optional_args);
}

UP::~UP() {};

std::string UP::construct_url(const std::vector<std::string>& query_params, const std::vector<opt_args>& optional_args) {
    return this->base_url(query_params.at(0)) + this->u1 + query_params.at(1)  + this->query_params(optional_args);
}

UPU::~UPU() {};

std::string UPU::construct_url(const std::vector<std::string>& query_params, const std::vector<opt_args>& optional_args) {
    return this->base_url(query_params.at(0)) + this->u1 + query_params.at(1) + this->u2 + this->query_params(optional_args);
}

UPP::~UPP() {};

std::string UPP::construct_url(const std::vector<std::string>& query_params, const std::vector<opt_args>& optional_args) {
    return this->base_url(query_params.at(0)) + this->u1 + query_params.at(1) + "/" + query_params.at(2) + this->query_params(optional_args);
}

UPUP::~UPUP() {};

std::string UPUP::construct_url(const std::vector<std::string>& query_params, const std::vector<opt_args>& optional_args) {
    return this->base_url(query_params.at(0)) + this->u1 + query_params.at(1) + this->u2 + query_params.at(2) + this->query_params(optional_args);
}

UPPP::~UPPP() {};

std::string UPPP::construct_url(const std::vector<std::string>& query_params, const std::vector<opt_args>& optional_args) {
    return this->base_url(query_params.at(0)) + this->u1 + query_params.at(1) + "/" + query_params.at(2) + "/" + query_params.at(3) + this->query_params(optional_args);
}
}
