#include "logger.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include <stdexcept>
#include <set>
#include <sstream>
#include <algorithm>
#include <filesystem>
#include <iostream>

namespace riotcpp {
namespace logging {

    // This holds the single, global logger instance.
    static std::shared_ptr<spdlog::logger> logger_instance;

    void initialize(const std::string& log_path, spdlog::level::level_enum level, const std::string& logger_name) {
        // Only initialize once.
        if (!logger_instance) {
            std::string effective_log_path = log_path.empty() ? "logs.log" : log_path;
            try {
                std::filesystem::path p(effective_log_path);
                if (p.has_parent_path()) {
                    std::filesystem::create_directories(p.parent_path());
                }

                auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(effective_log_path, false);
                logger_instance = std::make_shared<spdlog::logger>(logger_name, file_sink);
            } catch (const std::exception& ex) {
                // Fallback to console sink if file cannot be opened (e.g. sandbox or permission constraints)
                auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
                logger_instance = std::make_shared<spdlog::logger>(logger_name, console_sink);
            }

            logger_instance->set_level(level);
            logger_instance->flush_on(level);
            spdlog::set_default_logger(logger_instance);
            spdlog::set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%^%l%$] [thread %t] %v");
        }
    }
    
    std::shared_ptr<spdlog::logger> get() {
        if (!logger_instance) {
            // Ensure that the user has called initialize() before trying to get the logger.
            throw std::runtime_error("Logger not initialized. Please call riotcpp::logging::initialize() first.");
        }
        return logger_instance;
    }

    void log_headers(long query_id, const std::string& title, const cpr::Header& headers) {
        // A static set of lower-case header keys to filter out from logs for security.
        static const std::set<std::string> sensitive_keys = {"x-riot-token", "authorization"};

        std::stringstream ss;
        ss << "[Query " << query_id << "] " << title << ":";
        for (const auto& [key, val] : headers) {
            std::string lower_key = key;
            std::transform(lower_key.begin(), lower_key.end(), lower_key.begin(),
                           [](unsigned char c){ return std::tolower(c); });

            if (sensitive_keys.find(lower_key) == sensitive_keys.end()) {
                ss << "\n\t" << key << ": " << val;
            } else {
                ss << "\n\t" << key << ": [REDACTED]";
            }
        }
        get()->debug(ss.str());
    }

}
}