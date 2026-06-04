#include "utils.h"
#include <sstream>
#include <iomanip>

namespace utils {

    Logger::Logger() {
        logFileName = generateLogFileName();
        logFile.open(logFileName, std::ios::out | std::ios::app);
    }

    Logger::~Logger() {
        if (logFile.is_open()) {
            logFile.close();
        }
    }

    std::string Logger::generateLogFileName() {
        auto t = std::time(nullptr);
        auto tm = *std::localtime(&t);

        std::ostringstream oss;
        oss << "log_" << std::put_time(&tm, "%Y%m%d_%H%M%S") << ".txt";
        return oss.str();
    }

    std::string Logger::getCurrentTime() {
        auto t = std::time(nullptr);
        auto tm = *std::localtime(&t);

        std::ostringstream oss;
        oss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");
        return oss.str();
    }

    Logger& Logger::getInstance() {
        static Logger instance;
        return instance;
    }

    void Logger::log(const std::string& message) {
        if (logFile.is_open()) {
            logFile << getCurrentTime() << " - " << message << std::endl;
        }
    }

} // namespace utils
