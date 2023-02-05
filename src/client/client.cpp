#include <chrono>
#include <iostream>
#include <memory>
#include <thread>
#include <ctime>
#include <algorithm>
#include <stdexcept>

#include <fstream>
#include <stdio.h>
#include <queue>
#include "client.h"

namespace client {

    RiotApiClient::RiotApiClient(std::string path_to_config, std::string path_to_log, logging::level report_level, bool log_verbose, bool log_frequency) {
        curl_global_init(CURL_GLOBAL_ALL);

        // initialised libcurl handle and header
        Json::Reader reader;

        std::ifstream config(path_to_config);

        if (!config.is_open()) {
            std::string err_msg("Failed to open config file");
            err_msg = err_msg + path_to_config;
            throw std::invalid_argument(err_msg);
        }
        
        Json::Value root;
        bool retrieving_key = reader.parse(config, root);

        if (!retrieving_key) {
            throw Json::Exception("Unable to parse config file to Json format");
        }
        else {
            std::string api_key = root["api-key"].asString();
            this->header = curl_slist_append(header, (std::string("X-RIOT-TOKEN: ") + api_key).c_str());
        }
        
        this->easy_handle = curl_easy_init();
        this->request_handler = std::make_unique<handler::RequestHandler>();
        this->logger = std::make_unique<logging::Logger>(path_to_log, report_level, log_verbose, log_frequency);
    }

    RiotApiClient::~RiotApiClient() {
        curl_slist_free_all(this->header);
        curl_easy_cleanup(this->easy_handle);
        curl_global_cleanup();
    }

    static size_t WriteCallBack(void* contents, size_t size, size_t nmemb, void* buffer) {
        
        size_t real_size = size * nmemb;
        char *new_chars = (char *)contents;
        if (*new_chars == 'H') {
            return real_size;
        }
        std::vector<char> *new_buffer = reinterpret_cast<std::vector<char>*>(buffer);
        new_buffer->insert(new_buffer->end(), &new_chars[0], &new_chars[nmemb]);

        return real_size;
    }

    static size_t WriteCallBack_header(void* contents, size_t size, size_t nmemb, void* buffer) { 
        // format the header as chunks are received
        // fk riot games for not formatting the response header

        size_t real_size = size * nmemb;
        char *new_chars = (char *)contents;
        if (*new_chars == 'H') {
            return real_size;
        }
        char to_insert[3] = {'\"', ':', '\"'};
        char* colon_ind = std::find(new_chars, new_chars + nmemb, ':');
        if (colon_ind == new_chars + strlen(new_chars)) {
            return real_size;
        }
        std::vector<char> *new_buffer = reinterpret_cast<std::vector<char>*>(buffer);
        if (new_buffer->size() != 1) {
            new_buffer->push_back(',');
        }
        new_buffer->push_back('\"');
        new_buffer->insert(new_buffer->end(), &new_chars[0], &colon_ind[0]);
        new_buffer->insert(new_buffer->end(), &to_insert[0], to_insert + 3);
        new_buffer->insert(new_buffer->end(), &colon_ind[2], new_chars + strlen(new_chars) - 1);
        new_buffer->back() = '\"';
        return real_size;
    }

    bool RiotApiClient::get(std::shared_ptr<query::query> request) {

        Json::Reader reader;
        std::vector<char> content_buffer = {};
        std::vector<char> header_buffer = {'{'};

        curl_easy_setopt(this->easy_handle, CURLOPT_URL, request->url.data());
        curl_easy_setopt(this->easy_handle, CURLOPT_HTTPGET, 1);
        curl_easy_setopt(this->easy_handle, CURLOPT_HTTPHEADER, this->header);

        curl_easy_setopt(this->easy_handle, CURLOPT_WRITEFUNCTION, WriteCallBack);
        curl_easy_setopt(this->easy_handle, CURLOPT_WRITEDATA, &content_buffer);

        curl_easy_setopt(this->easy_handle, CURLOPT_HEADERFUNCTION, WriteCallBack_header);
        curl_easy_setopt(this->easy_handle, CURLOPT_HEADERDATA, &header_buffer);
     
        CURLcode res_ = curl_easy_perform(this->easy_handle);

        if (res_ != CURLE_OK) {
            request->last_response = -1; // CURL ERROR
            return false;
        }
        header_buffer.back() = '\"';
        header_buffer.push_back('}');

        curl_easy_getinfo(this->easy_handle, CURLINFO_RESPONSE_CODE, &(request->last_response));
        if (!reader.parse(header_buffer.data(), request->response_header)) {
            request->last_response = -1;
            return false;
        }

        Json::StreamWriterBuilder builder;
        
        if (request->last_response == 200) { // only parse content to json if request was successful
            reader.parse(content_buffer.data(), request->response_content);
        }

        return true;
    }
}
