#pragma once
#include <jsoncpp/json/writer.h>
#include <queue>
#include <jsoncpp/json/json.h>
#include <string>
#include <fstream>
#include "../query/query.h"

namespace logging {

    enum level : int {
        debug,
        info,
        warning,
        error,
        critical
    };

    class Logger {
        private:
            std::queue<std::string> _buffer;
            std::string _log_path;
            level _log_level;
            bool _verbose;
            bool _frequency;
            std::ofstream _log_file;

        public:
            Logger(std::string log_path, level log_level = warning, bool verbose = false, bool write_immediately = true);
            ~Logger();

            void log(std::string message, level report_level, const query::query &request);
            void log(std::string message, const level report_level);
            void dump();

        private:
            std::string log_message(const std::string &message, const level log_level);
            Json::StreamWriterBuilder _builder;
    };
}
