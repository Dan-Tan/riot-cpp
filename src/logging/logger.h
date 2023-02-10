#pragma once
#include <jsoncpp/json/writer.h>
#include <queue>
#include <jsoncpp/json/json.h>
#include <string>
#include <fstream>

namespace logging {

    enum LEVEL : int {
        DEBUG,
        INFO,
        WARNING,
        ERROR,
        CRITICAL
    };

    class Logger {
        private:
            std::string _log_path;
            LEVEL _log_level;

            std::ofstream _log_file;
            bool _incoming = false;

        public:
            Logger(std::string log_path, LEVEL log_level = LEVEL::INFO);
            ~Logger();

            Logger& operator<<(const LEVEL& log_level);
            Logger& operator<<(const std::string& message);
            //Logger& operator<<(const unsigned int err_code); todo
            //Logger& operator<<(const std::shared_ptr<query::query> request);
            Logger& operator<<(const int terminate) {
                if (this->_incoming && !terminate) {
                    this->_log_file << '\n';
                    this->_incoming = false;
                    return *this;
                } else {
                    return *this;
                }
            };
    };
}
