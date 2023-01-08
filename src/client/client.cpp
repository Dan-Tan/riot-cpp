#include "client.h"
#include <chrono>
#include <thread>
#include <ctime>
#include <functional>

#include <fstream>
#include <stdio.h>

using namespace client;

RiotApiClient::RiotApiClient(std::string path_to_config, std::string path_to_log, bool log_all, bool overwrite) {
    curl_global_init(CURL_GLOBAL_ALL);

    this->log_all = log_all;
    this->reader = std::shared_ptr<Json::Reader>();

    FILE* log;

    log = fopen(path_to_log.c_str(), overwrite ? "w" : "a");

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
        fclose(log);
        throw std::domain_error(err_msg);
    }
    
    Json::Value root;
    bool retrieving_key = reader.parse(config, root);

    if (!retrieving_key) {
        std::string err_msg("Unable to read api json");
        fclose(log);
        throw std::domain_error(err_msg);
    }
    else {
        std::string api_key = root["api-key"].asString();
        this->header = curl_slist_append(header, (std::string("X-RIOT-TOKEN: ") + api_key).c_str());
        fprintf(log, "Retrieved API key\n");
    }
    
    this->path_to_log = path_to_log;
    this->easy_handle = curl_easy_init();
    this->rate_handler = RateLimiter(this->easy_handle, this->reader);

    fprintf(log, "Curl initialised\n");
    
    fprintf(log, "MAX SERVICE DENIALS (503): %d \n", this->service_attempts);
    fprintf(log, "MAX INTERNAL SERVICE ERRORS (500): %d \n", this->internal_attempts);
    
    fprintf(log, "--- Client Successfully Initialised ---\n \n");
    fclose(log);
}

RiotApiClient::~RiotApiClient() {
    curl_global_cleanup();
}

static size_t WriteCallBack(void* contents, size_t size, size_t nmemb, void* buffer) {
    
    size_t real_size = size * nmemb;
    std::vector<char> *new_buffer = (std::vector<char> *) buffer;
    size_t current_size = new_buffer->size();
    char *new_chars = (char *)contents;
    new_buffer->insert(new_buffer->end(), &new_chars[0], &new_chars[real_size]);
    return real_size;
}

Json::Value RiotApiClient::get(std::string_view address, std::shared_ptr<query_attempts> attempt) {

    Json::Value result;

    CURLcode res_;
    curl_easy_setopt(this->easy_handle, CURLOPT_URL, address.data());
    curl_easy_setopt(this->easy_handle, CURLOPT_HTTPGET, 1);
    curl_easy_setopt(this->easy_handle, CURLOPT_HTTPHEADER, this->header);
    curl_easy_setopt(this->easy_handle, CURLOPT_VERBOSE, 0);

    curl_easy_setopt(this->easy_handle, CURLOPT_WRITEFUNCTION, WriteCallBack);
    curl_easy_setopt(this->easy_handle, CURLOPT_WRITEDATA, &this->buffer);

    curl_easy_setopt(this->easy_handle, CURLOPT_HEADERFUNCTION, WriteCallBack);
    curl_easy_setopt(this->easy_handle, CURLOPT_HEADERDATA, &this->rate_handler->header_buffer);

    res_ = curl_easy_perform(this->easy_handle);

    if (res_ != CURLE_OK) {
        this->log_request(address, -1, attempt, &res_);
        return result;
    }

    long response_code = 0;
    curl_easy_getinfo(this->easy_handle, CURLINFO_RESPONSE_CODE, &response_code);

    bool repeat = this->handle_response(address, response_code, attempt);
    if (repeat) {
        return this->get(address, attempt);
    } 

    if (response_code = 200) {
        std::string result_(this->buffer.data());
        bool json_conversion = this->reader->parse(result_, result);
        if (!json_conversion) {
            std::domain_error("Unable to cast output string to Json");
        }
    }

    this->buffer.clear();

    return result;
}
