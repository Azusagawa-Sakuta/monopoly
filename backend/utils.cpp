#include "utils.h"
#include <mutex>
#include <sstream>
#include <iomanip>

namespace utils {

    /**
     * @brief Constructor of the Logger class
     * @param None
     * @return None
     */
    Logger::Logger() {
        logFileName = generateLogFileName(); // Generate log file name
        logFile.open(logFileName, std::ios::out | std::ios::app); // Open the log file
    }

    /**
     * @brief Destructor of the Logger class
     * @param None
     * @return None
     */
    Logger::~Logger() {
        if (logFile.is_open()) {
            logFile.close(); // Close the log file
        }
    }

    /**
     * @brief Generate a log file name in the format "log_YYYYMMDD_HHMMSS.txt"
     * @param None
     * @return std::string Log file name
     */
    std::string Logger::generateLogFileName() {
        auto t = std::time(nullptr); // Get current time
        auto tm = *std::localtime(&t); // Convert to a structure

        std::ostringstream oss; // Create an output string stream
        oss << "log_" << std::put_time(&tm, "%Y%m%d_%H%M%S") << ".txt"; // Format the time
        return oss.str(); // Return the formatted time
    }

    /**
     * @brief Get the current time in the format "YYYY-MM-DD HH:MM:SS"
     * @param None
     * @return std::string Current time
     */
    std::string Logger::getCurrentTime() {
        auto t = std::time(nullptr); // Get current time
        auto tm = *std::localtime(&t); // Convert to a structure

        std::ostringstream oss; // Create an output string stream
        oss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S"); // Format the time
        return oss.str(); // Return the formatted time
    }

    /**
     * @brief Get the instance of the Logger class
     * @param None
     * @return Logger& Instance of Logger
     */
    Logger& Logger::getInstance() {
        static Logger instance; // Singleton instance, guaranteed to be destroyed
        return instance; // Instantiated on first use
    }

    /**
     * @brief Write a message to the log file
     * @param message The message to log
     * @return None
     */
    void Logger::log(const std::string& message) {
        std::unique_lock<std::shared_mutex> lock(mtx); // Lock the mutex for thread-safe access
        if (logFile.is_open()) {
            logFile << getCurrentTime() << " - " << message << std::endl; // Write the message to the log file
        }
    }

} // namespace utils
