#include <ctime>
#include <stdexcept>
#include "logger.h"

namespace logging {

    static std::string Err_Codes(int code) { // official message
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

    static std::string Code_Meaning(int code) { // more informative (ty shieldbow riot api)
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


    static std::string level_string(level &log_level) {
        switch (log_level) {
            case debug:
                return std::string("DEBUG   ");
            case info:
                return std::string("INFO    ");
            case warning:
                return std::string("WARNING ");
            case error:
                return std::string("ERROR   ");
            case critical:
                return std::string("CRITICAL");
            default:
                return std::string("INVALID LOG LEVEL");
        }
    }
    
    Logger::Logger(std::string log_path, level log_level, bool verbose, bool write_immediately) {
        this->_log_path = log_path;
        this->_log_level = log_level;
        this->_verbose = verbose;
        this->_log_file.open(log_path, std::ios::app); 
        this->_frequency = write_immediately;
        if (!this->_log_file.is_open()) {
            throw std::runtime_error("Unable to open log file");
        } else {
            this->_log_file << "Log file successfully opened. \n";
        }
    }

    Logger::~Logger() {
        this->dump();
        this->_log_file.close();
    }

    std::string Logger::log_message(const std::string &message, level report_level) {

        const std::time_t cur_time = std::time(NULL);
        std::tm *gmt_time = std::gmtime(&cur_time);
        char time_buf[sizeof("Sun, 05 Feb 2023 08:36:28 GMT") + 2];
        strftime(time_buf, sizeof time_buf, "[%a, %d %b %y %H:%M:%S GMT]", gmt_time);
        std::string log_msg{time_buf};
        log_msg = log_msg + ": " + level_string(report_level) + message + "\n";

        return log_msg;
    }

    void Logger::log(std::string message, level report_level) {

        if (report_level < this->_log_level) {
            return;
        }
        std::string log_msg = this->log_message(message, report_level);

        if (this->_frequency) {
            this->_log_file << log_msg;
            return;
        }
        this->_buffer.push(log_msg);
        
    }

    void Logger::log(std::string message, level report_level, const query::query &request) {

        if (report_level < this->_log_level) {
            return;
        }
        std::string log_msg = this->log_message(message, report_level);
        log_msg = log_msg + "  " + "[Method Key: " + request.method_key + "], " + "[Last Response Code: " + std::to_string(request.last_response) + "]\n";
        log_msg = log_msg + "  " + Err_Codes(request.last_response) + ": " + Code_Meaning(request.last_response);

        if (this->_verbose) {
            log_msg = log_msg + ": " + Json::writeString(this->_builder, request.response_header);
        }

        if (this->_frequency) {
            this->_log_file << log_msg;
            return;
        }

        this->_buffer.push(log_msg);
    }

    void Logger::dump() {
        while (!this->_buffer.empty()) {
            this->_log_file << this->_buffer.front();
            this->_buffer.pop();
        }
    }
}
