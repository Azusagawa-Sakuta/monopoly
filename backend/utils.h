#ifndef UTILS_H
#define UTILS_H

#include <fstream>
#include <string>
#include <ctime>
#include <shared_mutex>

namespace utils {

    class Logger {
    private:
        std::ofstream logFile;
        std::string logFileName;
        mutable std::shared_mutex mtx;

        Logger();
        ~Logger();

        Logger(const Logger&) = delete;             // disable copy constructor
        Logger& operator=(const Logger&) = delete;  // disable copy assignment

        std::string generateLogFileName();
        std::string getCurrentTime();

    public:
        static Logger& getInstance();
        void log(const std::string& message);
    };

} // namespace utils

#endif // UTILS_H
