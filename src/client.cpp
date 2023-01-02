#include <string>
#include <string_view>
#include <jsoncpp/json/json.h>
#include <curl/curl.h>
#include "client.h"
#include <chrono>
#include <thread>
#include <ctime>

#define DEBUG 1

#include <fstream>
#include <iostream>
#include <stdio.h>

using namespace client;

RiotApiClient::RiotApiClient(std::string path_to_config, std::string path_to_log, bool log_all, bool overwrite) {
    curl_global_init(CURL_GLOBAL_ALL);

    this->log_all = log_all;

    FILE* log;
    const char* write_type;
    if (overwrite) {
        write_type = "w";
    } else {
        write_type = "a";
    }
    log = fopen(path_to_log.c_str(), write_type);

    std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
    std::time_t now_c = std::chrono::system_clock::to_time_t(now);
    std::tm now_tm = *std::localtime(&now_c);
    char curr_time[70];
    strftime(curr_time, sizeof(curr_time), "%A %c", &now_tm);

    fprintf(log, "--- Client Initialised Started ---\n");
    fprintf(log, "Time initialised: %s\n", curr_time);
    fprintf(log, "Path to log file: %s\n", path_to_log.c_str());
    fprintf(log, "overwite: %s, log_all queries: %s\n", overwrite ? "true" : "false", log_all ? "true" : "false");

    // initialised libcurl handle and header
    std::ifstream config(path_to_config);
    Json::Reader reader;

    this->internal_attempts = 2;
    this->internal_wait_type = true;
    this->service_attempts = 2;
    this->service_wait_type = true;

    if (!config) {
        std::string err_msg("Cannot open config file: ");
        err_msg = err_msg + path_to_config;
        throw std::domain_error(err_msg);
    }
    
    Json::Value root;
    bool retrieving_key = reader.parse(config, root);

    if (!retrieving_key) {
        std::string err_msg("Unable to read api json");
        throw std::domain_error(err_msg);
    }
    else {
        std::string api_key = root["api-key"].asString();
        this->header = curl_slist_append(header, (std::string("X-RIOT-TOKEN: ") + api_key).c_str());
        fprintf(log, "Retrieved API key\n");
    }
    
    this->path_to_log = path_to_log;
    this->easy_handle = curl_easy_init();

    fprintf(log, "Curl initialised\n");
    
    fprintf(log, "MAX SERVICE DENIALS (503): %d \n", this->service_attempts);
    fprintf(log, "MAX INTERNAL SERVICE ERRORS (500): %d \n", this->internal_attempts);
    
    fprintf(log, "--- Client Successfully Initialised ---\n \n");
    fclose(log);
}

RiotApiClient::~RiotApiClient() {
    curl_global_cleanup();
}

typedef struct res_buffer {
    char* mem;
    size_t size;
} res_buffer;

res_buffer* init_empty_res_buffer() {
    res_buffer *buffer = (res_buffer *)malloc(sizeof(res_buffer));
    buffer->mem = (char *)malloc(1);
    buffer->size = 0;
    return buffer;
}

void free_res_buffer(res_buffer* buffer) {
    free(buffer->mem);
    free(buffer);
}

static size_t parse_mem(void* contents, size_t size, size_t nmemb, void* user_data) {
    size_t added_size = size * nmemb;
    res_buffer *resp = (res_buffer *)user_data;


    char* ptr = (char *) realloc(resp->mem, resp->size + added_size + 1);
    if (!ptr) {
        std::cout << "Memory reassignment failed: Not enough memory." << std::endl;
        return 0;
    }

    resp->mem = ptr;
    memcpy(&(resp->mem[resp->size]), contents, added_size);
    resp->size += added_size;
    resp->mem[resp->size] = 0;
    return added_size;
}

Json::Value RiotApiClient::get(std::string_view address, query_attempts* attempt) {

    Json::Reader reader;
    Json::Value result;

    // construct query url

    if (!attempt) {
        attempt = init_attempt_count();
    }

    // initialise response buffer
    res_buffer *response = init_empty_res_buffer();

    CURLcode res_;

    curl_easy_setopt(this->easy_handle, CURLOPT_URL, address.data());
    curl_easy_setopt(this->easy_handle, CURLOPT_HTTPGET, 1);
    curl_easy_setopt(this->easy_handle, CURLOPT_HTTPHEADER, this->header);
    curl_easy_setopt(this->easy_handle, CURLOPT_VERBOSE, 0);

    curl_easy_setopt(this->easy_handle, CURLOPT_WRITEFUNCTION, parse_mem);
    curl_easy_setopt(this->easy_handle, CURLOPT_WRITEDATA, response);

    res_ = curl_easy_perform(this->easy_handle);
    std::string result_(response->mem);
    bool json_conversion = reader.parse(result_, result);
    if (!json_conversion) {
        std::domain_error("Unable to cast output string to Json");
    }

    if (res_ != CURLE_OK) {
        std::cout << curl_easy_strerror(res_) << std::endl;
        std::cout << "Query failed" << std::endl;
        std::cout << address << std::endl;
        return result;
    }

    long response_code = 0;
    curl_easy_getinfo(this->easy_handle, CURLINFO_RESPONSE_CODE, &response_code);

    bool repeat = this->handle_response(address, response_code, attempt);
    if (repeat) {
        return this->get(address, attempt);
    } else {
        free_query_counter(attempt);
    }

    return result;
}
