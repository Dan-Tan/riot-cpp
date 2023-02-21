#include <ctime>
#include <jsoncpp/json/json.h>
#include <jsoncpp/json/writer.h>
#include <stdexcept>
#include "logger.h"

namespace logging {
    static std::string Err_Codes(const unsigned int code) { // official message
        switch (code) {
            case 200:
                return std::string("Successful request");
            case 400:
                return std::string("Bad request");
            case 401:
                return std::string("Unauthorized");
            case 403:
                return std::string("Forbidden");
            case 404:
                return std::string("Data not found");
            case 405:
                return std::string("Method not found");
            case 415:
                return std::string("Unsupported media type");
            case 429:
                return std::string("Rate limit exceeded");
            case 500:
                return std::string("Internal service error");
            case 502:
                return std::string("Bad gateway");
            case 503:
                return std::string("Service unavailable");
            case 504:
                return std::string("Gateway timeout");
            default:
                throw std::invalid_argument("Invalide error code passed" + std::to_string(code));
        }
    }

    static std::string Code_Meaning(const unsigned int code) { // more informative (ty shieldbow riot api)
        switch (code) {
            case 200:
                return std::string("Successful request");
            case 400:
                return std::string("Verify the request URL, headers, and body parameters.");
            case 401:
                return std::string("No API key was found, please make sure you're providing one.");
            case 403:
                return std::string("Either your API key is invalid/blacklisted or you don't have access to the requested resource.");
            case 404:
                return std::string("The requested resource was not found.");
            case 415:
                return std::string("The provided body is of an invalid type.");
            case 429:
                return std::string("You have reached the rate limit.");
            case 500:
                return std::string("The server encountered an error. Please try again later.");
            case 502:
                return std::string("Bad gateway");
            case 503:
                return std::string("The server is down. Please try again later.");
            case 504:
                return std::string("Gateway timeout");
            default:
                throw std::invalid_argument("Invalid error code passed: " + std::to_string(code));
        }
    };

    static std::string level_string(const LEVEL &log_level) {
        switch (log_level) {
            case LEVEL::DEBUG:
                return std::string("DEBUG");
            case LEVEL::INFO:
                return std::string("INFO");
            case LEVEL::WARNING:
                return std::string("WARNING");
            case LEVEL::ERROR:
                return std::string("ERROR");
            case LEVEL::CRITICAL:
                return std::string("CRITICAL");
            default:
                throw std::invalid_argument("Invalid log level passed"); // somehow
        }
    }
    
    static const std::string get_current_time() { // riot format
        const std::time_t cur_time = std::time(NULL);
        std::tm *gmt_time = std::gmtime(&cur_time);
        char time_buf[sizeof("Sun, 05 Feb 2023 08:36:28 GMT") + 2];
        strftime(time_buf, sizeof time_buf, "[%a, %d %b %y %H:%M:%S GMT]", gmt_time);
        std::string log_msg{time_buf};
        return log_msg;
    };

    Logger& Logger::operator<<(const LEVEL& log_level) {
        if (this->_log_level <= log_level) {
            std::string log_msg = get_current_time();
            log_msg = log_msg + " " + level_string(log_level) + ":";
            this->_log_file << log_msg;
            this->_incoming = true;
        }
        return *this;
    };

    Logger& Logger::operator<<(const std::string& message) {
        if (this->_incoming) {
            this->_log_file << "\n  " << message;
        }
        return *this;
    }

    Logger& Logger::operator<<(const unsigned int err_code) {
        if (this->_incoming) {
            this->_log_file << "\n  " << err_code << ": " << Err_Codes(err_code);
            if (this->_verbose){
                this->_log_file << "\n  " << Code_Meaning(err_code);
            }
        }
        return *this;
    }

    Logger& Logger::operator<<(const std::shared_ptr<query::query> request) {
        if (this->_incoming) {
            this->_log_file << "\n  Endpoint: " << request->method_key;
            if (this->_verbose) {
                Json::StreamWriterBuilder builder;
                this->_log_file << Json::writeString(builder, request->response_header);
                this->_log_file << Json::writeString(builder, request->response_content);
            }
        }
        return *this;
    }
    
    Logger::Logger(std::string log_path, LEVEL log_level, bool verbose) {
        this->_log_path = log_path;
        this->_log_level = log_level;
        this->_verbose = verbose;
        this->_log_file.open(log_path, std::ios::app); 
        std::string init_time = get_current_time();
        if (!this->_log_file.is_open()) {
            throw std::runtime_error("Unable to open log file");
        } else {
            this->_log_file << init_time << " Log Initialisation:\n  Log file successfully opened. \n";
        }
    }

    Logger::~Logger() {
        this->_log_file.close();
    }
}
