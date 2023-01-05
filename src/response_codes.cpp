#include "client.h"
#include <iostream>
#include <stdio.h>
#include <string>
#include <curl/curl.h>
#include <chrono>
#include <thread>
#include <ctime>

using namespace client;

const std::unordered_map<int, std::string> RiotApiClient::Err_Codes = {{200, "Successful"},
                                        {400, "Bad request"},
                                        {401, "Unauthorized"},
                                        {403, "Forbidden"},
                                        {404, "Data not found"},
                                        {405, "Method not allowed"},
                                        {415, "Unsupported media type"},
                                        {429, "Rate limit exceeded"},
                                        {500, "Internal server error"},
                                        {502, "Bad gateway"},
                                        {503, "Service unavailable"},
                                        {504, "Gateway timeout"}};


bool RiotApiClient::handle_response(std::string_view address, long response_code, std::shared_ptr<query_attempts> attempt) {

    bool repeat;
    bool _log = false;

    if (response_code == 200) {
        repeat = false;
    } else if (response_code == 429) {
        bool wait_type = !(attempt->rate_denials >= 1);
        this->handle_rate(wait_type);
        attempt->rate_denials += 1;
        repeat = true;

    } else if (response_code == 500) {
        if (attempt->internal_errors > this->internal_attempts) {
            repeat = false;
        } else {
            attempt->internal_errors += 1;
            this->handle_rate(this->internal_wait_type);
            repeat = true;
        }
    } else if (response_code == 503) {
        if (attempt->service_denials > this->service_attempts) {
            repeat = false;
        } else {
            attempt->service_denials += 1;
            this->handle_rate(this->service_wait_type);
            repeat = true;
        }
    } else {
        repeat = false;
        _log = true;
    }
    if (!repeat) {
        if (attempt->service_denials != 0 || attempt->internal_errors != 0 || attempt->rate_denials != 0 || this->log_all || _log) {
            this->log_request(address, response_code, attempt);
        }
    }
    return repeat;
}

void RiotApiClient::log_request(std::string_view address_sent, long response_code, std::shared_ptr<query_attempts> attempts, CURLcode* res_) {

    FILE* log;
    log = fopen(this->path_to_log.c_str(), "a");

    std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
    std::time_t now_c = std::chrono::system_clock::to_time_t(now);
    std::tm now_tm = *std::localtime(&now_c);
    char curr_time[70];
    strftime(curr_time, sizeof(curr_time), "%A %c", &now_tm);

    fprintf(log, "--- NEW QUERY --- \n");
    fprintf(log, "Address_sent: %s\n", address_sent.data());
    fprintf(log, "Time finished: %s\n", curr_time);
    if (response_code != -1) {
        fprintf(log, "Final Response Code: %ld\n", response_code);
        fprintf(log, "Final Response Message: %s\n", RiotApiClient::Err_Codes.at(response_code).c_str());
    } else {
        fprintf(log, "Final Response Code: -1 (CURL ERROR) \n");
        fprintf(log, "Final Response Message: %s\n", curl_easy_strerror(*res_));
    }
    fprintf(log, "Attempts: rate_denials: %d, internal_errors: %d, service_denials: %d \n \n", attempts->rate_denials, attempts->internal_errors, attempts->service_denials);

    fclose(log);
}

void RiotApiClient::handle_rate(bool wait_type) {
    if (!wait_type) {
        std::this_thread::sleep_for(std::chrono::seconds(120));
    } else {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    return;
}
