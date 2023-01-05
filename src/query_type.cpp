#include "query_type.h"

using namespace query;

QueryType::QueryType() {};
QueryType::~QueryType() {};

std::string QueryType::base_url(std::string region) {
    return std::string("https://" + region + ".api.riotgames.com");
}

U::U(std::string u1) {
    this->u1 = u1;
}

U::~U() {};

std::string U::construct_url(std::vector<std::string> query_params) {
    return this->base_url(query_params[0]) + this->base_url(query_params[1]) + this->u1;
}

UP::UP(std::string u1) {
    this->u1 = u1;
}

UP::~UP() {};

std::string UP::construct_url(std::vector<std::string> query_params) {
    return this->base_url(query_params[0]) + this->base_url(query_params[1]) + this->u1 + query_params[2];
}

UPU::UPU(std::string u1, std::string u2) {
    this->u1 = u1;
    this->u2 = u2;
}

UPU::~UPU() {};

std::string UPU::construct_url(std::vector<std::string> query_params) {
    return this->base_url(query_params[0]) + this->u1 + query_params[1] + this->u2;
}

UPP::UPP(std::string u1) {
    this->u1 = u1;
}

UPP::~UPP() {};

std::string UPP::construct_url(std::vector<std::string> query_params) {
    return this->base_url(query_params[0]) + this->u1 + query_params[1] + query_params[2];
}

UPUP::UPUP(std::string u1, std::string u2) {
    this->u1 = u1;
    this->u2 = u2;
}

UPUP::~UPUP() {};

std::string UPUP::construct_url(std::vector<std::string> query_params) {
    return this->base_url(query_params[0]) + this->u1 + query_params[1] + this->u2 + query_params[2];
}

UPPP::UPPP(std::string u1) {
    this->u1 = u1;
}

UPPP::~UPPP() {};

std::string UPPP::construct_url(std::vector<std::string> query_params) {
    return this->base_url(query_params[0]) + this->u1 + query_params[1] + query_params[2] + query_params[3];
}
