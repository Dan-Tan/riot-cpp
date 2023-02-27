#pragma once
#include <jsoncpp/json/writer.h>
#include <queue>
#include <jsoncpp/json/json.h>
#include <string>
#include <fstream>
#include "../query/query.h"

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
            bool _verbose;

        public:
            Logger(std::string log_path, LEVEL log_level = LEVEL::INFO, bool verbose = false);
            ~Logger();

            Logger& operator<<(const LEVEL& log_level);
            Logger& operator<<(const std::string& message);
            Logger& operator<<(const char* message);
            Logger& operator<<(const int err_code);
            Logger& operator<<(const Json::Value& response);
    };
}
