#include "logger.h"
#include "spdlog/sinks/basic_file_sink.h"
#include <stdexcept>
#include <set>
#include <sstream>
#include <algorithm>

namespace riotcpp {
namespace logging {

    // This holds the single, global logger instance.
    static std::shared_ptr<spdlog::logger> logger_instance;

    void initialize(const std::string& log_path, spdlog::level::level_enum level, const std::string& logger_name) {
        // Only initialize once.
        if (!logger_instance) {
            try {
                // Create a thread-safe, file-based logger.
                // The 'true' argument truncates the file on open.
                auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(log_path, false);
                
                // Create the logger.
                logger_instance = std::make_shared<spdlog::logger>(logger_name, file_sink);
                
                // Set the logging levels.
                logger_instance->set_level(level);
                logger_instance->flush_on(level); // Auto-flush on messages of this level and higher.
                
                // Set the default logger for the spdlog registry.
                spdlog::set_default_logger(logger_instance);

                // Define a formatting pattern for the log messages.
                spdlog::set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%^%l%$] [thread %t] %v");

            } catch (const spdlog::spdlog_ex& ex) {
                // If spdlog fails to initialize (e.g., file permissions), throw an exception.
                throw std::runtime_error("Log initialization failed: " + std::string(ex.what()));
            }
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
            }
        }
        get()->debug(ss.str());
    }

}
}