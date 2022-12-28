#include <string>
#include <jsoncpp/json/json.h>
#include <curl/curl.h>
#include "client.h"
#include <chrono>
#include <thread>

#define DEBUG 1

#include <fstream>
#include <iostream>

using namespace client;

RiotApiClient::RiotApiClient(std::string path_to_config, std::string path_to_log) {
    curl_global_init(CURL_GLOBAL_ALL);

    this->n_attempts = 5;

    std::ifstream config(path_to_config);
    Json::Reader reader;

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
        std::cout << "Retrieved API key" << std::endl;
        this->header = curl_slist_append(header, (std::string("X-RIOT-TOKEN: ") + api_key).c_str());
    }
    
    this->path_to_log = path_to_log;
    this->easy_handle = curl_easy_init();

    std::cout << "Client initialised" << std::endl;
}

RiotApiClient::~RiotApiClient() {
    curl_global_cleanup();
}

std::string RiotApiClient::get_BASE_URL(std::string region) {
    return BASE_URL_START + region + BASE_URL_END;
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

Json::Value RiotApiClient::get(std::string end_url, std::string region, int attempt) {

    Json::Reader reader;
    Json::Value result;

    // construct query url
    std::string address = this->get_BASE_URL(region) + end_url; 

    // initialise response buffer
    res_buffer *response = init_empty_res_buffer();

    CURLcode res_;

    curl_easy_setopt(this->easy_handle, CURLOPT_URL, address.c_str());
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
    if (response_code == 429) {
        if (attempt == 0) {
           this->handle_rate(true);
           attempt++;
           return this->get(end_url, region, attempt);
        } else if (attempt > this->n_attempts) {
           std::cout << "Aborting query, too many rate denials" << std::endl;
           return result;
        } else {
           this->handle_rate(false);
           attempt++;
           return this->get(end_url, region, attempt);
        }
    } else if (response_code != 200) {
        std::cout << response_code << std::endl;
        std::cout << address << std::endl;
    }

    return result;
}

void RiotApiClient::handle_rate(bool wait_type) {
    if (!wait_type) {
        std::cout << "Long timeout: waiting..." << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(120));
    } else {
        std::cout << "waiting..." << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    std::cout << "trying again..." << std::endl;
    return;
}

bool RiotApiClient::handle_response(long response_code, bool log) {
    if (log) {
        return true;
    } else {
        return (response_code == 0);
    }
}

