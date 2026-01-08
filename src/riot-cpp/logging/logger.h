#pragma once

#include "spdlog/spdlog.h"
#include <memory>
#include <string>
#include <cpr/response.h>

// This macro sets the most verbose level of logging that will be compiled.
// It can be overridden by the build system (e.g., passing -DSPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_INFO)
// to strip out lower-level logs for performance in release builds.
#ifndef SPDLOG_ACTIVE_LEVEL
#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_DEBUG
#endif

namespace riotcpp {
namespace logging {

    /**
     * @brief Initializes the logging system.
     * 
     * This must be called once by the client before any logging can be performed.
     * It sets up a thread-safe logger that writes to a specified file.
     * 
     * @param log_path Path to the output log file.
     * @param level The minimum level of messages to log (e.g., spdlog::level::info).
     * @param logger_name The internal name of the logger instance.
     */
    void initialize(const std::string& log_path, spdlog::level::level_enum level, const std::string& logger_name = "riot-cpp");

    /**
     * @brief Retrieves the globally shared logger instance.
     * 
     * @throws std::runtime_error if the logger has not been initialized via initialize().
     * @return A shared pointer to the logger instance.
     */
    std::shared_ptr<spdlog::logger> get();

    /**
     * @brief Logs the key-value pairs of a cpr::Header object, filtering sensitive information.
     * 
     * @param title A title for the log entry (e.g., "Response Headers").
     * @param headers The cpr::Header object to log.
     */
    void log_headers(const std::string& title, const cpr::Header& headers);

}
}