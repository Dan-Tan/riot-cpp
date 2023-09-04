#pragma once
#include <queue>
#include <string>
#include <fstream>
#include "../query/query.h"
#include "../handling/structures/rate_structures.h"

namespace logging {

    enum LEVEL : int {
        DEBUG,
        INFO,
        WARNING,
        ERRORS,
        CRITICAL
    };

    class Logger {
        private:
            std::string _log_path;
            LEVEL _log_level;

            std::ofstream _log_file;
            bool _incoming = false;
            bool _verbose;
            bool _log_q_time;

        public:
            Logger(std::string log_path, LEVEL log_level = LEVEL::INFO, bool verbose = false, bool log_q_time = false);
            ~Logger();

            Logger& operator<<(const LEVEL& log_level);
            Logger& operator<<(const std::string& message);
            Logger& operator<<(const char* message);
            Logger& operator<<(const int err_code);
            Logger& operator<<(const std::vector<handler_structs::ScopeHistory>& method_history);
            Logger& operator<<(const query::RiotHeader& response);
    };
}
