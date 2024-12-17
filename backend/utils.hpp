#ifndef UTILS_H
#define UTILS_H

#include <fstream>
#include <sstream>
#include <iomanip>
#include <string>
#include <ctime>
#include <mutex>

namespace utils {

    class Logger {
    private:
        std::ofstream logFile;
        std::string logFileName;
        mutable std::shared_mutex mtx;

        Logger() {
            /*
                * Constructor of the Logger class
                * Open the log file
             */

            logFileName = generateLogFileName();
            logFile.open(logFileName, std::ios::out | std::ios::app);
        }

        ~Logger() {
            /*
                * Destructor of the Logger class
                * Close the log file
             */

            if (logFile.is_open()) {
                logFile.close();
            }
        }

        Logger(const Logger&) = delete;             // disable copy constructor
        Logger& operator=(const Logger&) = delete;  // disable copy assignment

        std::string generateLogFileName() {
            /*
                * Args: 
                *     None
                * Returns:
                *     std::string format "log_YYYYMMDD_HHMMSS.txt"
                * Generate a log file name in the format "log_YYYYMMDD_HHMMSS.txt"
             */

            auto t = std::time(nullptr);                                    // get time now
            auto tm = *std::localtime(&t);                                  // convert to a structure

            std::ostringstream oss;                                         // create an output string stream
            oss << "log_" << std::put_time(&tm, "%Y%m%d_%H%M%S") << ".txt"; // format the time
            return oss.str();                                               // return the formatted time
        }

        std::string getCurrentTime() {
            /* 
                * Args: 
                *     None
                * Returns:
                *     std::string format "YYYY-MM-DD HH:MM:SS"
                * Get the current time in the format "YYYY-MM-DD HH:MM:SS"
             */

            auto t = std::time(nullptr);                    // get time now
            auto tm = *std::localtime(&t);                  // convert to a structure

            std::ostringstream oss;                         // create an output string stream
            oss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S"); // format the time
            return oss.str();                               // return the formatted time
        }

    public:
        static Logger& getInstance() {
            /*
                * Args: 
                *     None
                * Returns:
                *     Logger& instance
                * Get the instance of the Logger class
             */

            static Logger instance; // Guaranteed to be destroyed.
            return instance;        // Instantiated on first use.
        }

        void log(const std::string& message) {
            /*
                * Args: 
                *     const std::string& message
                * Returns:
                *     None
                * Write a message to the log file
             */

            std::unique_lock<std::shared_mutex> lock(mtx);                    // lock the mutex
            if (logFile.is_open()) {
                logFile << getCurrentTime() << " - " << message << std::endl; // write the message to the log file
            }
        }
    };

} // namespace utils

#endif // UTILS_H