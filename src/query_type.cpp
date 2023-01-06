#include "query_type.h"

using namespace query;

QueryType::~QueryType() {};

U::~U() {};

std::string U::construct_url(std::vector<std::string> query_params) {
    return this->base_url(query_params[0]) + this->u1;
}

UP::~UP() {};

std::string UP::construct_url(std::vector<std::string> query_params) {
    return this->base_url(query_params[0]) + this->u1 + query_params[1] ;
}

UPU::~UPU() {};

std::string UPU::construct_url(std::vector<std::string> query_params) {
    return this->base_url(query_params[0]) + this->u1 + query_params[1] + this->u2;
}

UPP::~UPP() {};

std::string UPP::construct_url(std::vector<std::string> query_params) {
    return this->base_url(query_params[0]) + this->u1 + query_params[1] + "/" + query_params[2];
}

UPUP::~UPUP() {};

std::string UPUP::construct_url(std::vector<std::string> query_params) {
    return this->base_url(query_params[0]) + this->u1 + query_params[1] + this->u2 + query_params[2];
}

UPPP::~UPPP() {};

std::string UPPP::construct_url(std::vector<std::string> query_params) {
    return this->base_url(query_params[0]) + this->u1 + query_params[1] + "/" + query_params[2] + "/" + query_params[3];
}
