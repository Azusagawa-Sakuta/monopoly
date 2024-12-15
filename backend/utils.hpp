#ifndef UTILS_H
#define UTILS_H

#include <fstream>
#include <sstream>
#include <iomanip>
#include <string>
#include <ctime>

namespace utils {

    class Logger {
    private:
        std::ofstream logFile;
        std::string logFileName;

        Logger() {
            logFileName = generateLogFileName();
            logFile.open(logFileName, std::ios::out | std::ios::app);
        }

        ~Logger() {
            if (logFile.is_open()) {
                logFile.close();
            }
        }

        Logger(const Logger&) = delete;
        Logger& operator=(const Logger&) = delete;

        std::string generateLogFileName() {
            auto t = std::time(nullptr);
            auto tm = *std::localtime(&t);

            std::ostringstream oss;
            oss << "log_" << std::put_time(&tm, "%Y%m%d_%H%M%S") << ".txt";
            return oss.str();
        }

        std::string getCurrentTime() {
            auto t = std::time(nullptr);
            auto tm = *std::localtime(&t);

            std::ostringstream oss;
            oss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");
            return oss.str();
        }

    public:
        static Logger& getInstance() {
            static Logger instance;
            return instance;
        }

        void log(const std::string& message) {
            if (logFile.is_open()) {
                logFile << getCurrentTime() << " - " << message << std::endl;
            }
        }
    };

} // namespace utils

#endif // UTILS_H