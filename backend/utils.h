#ifndef UTILS_H
#define UTILS_H

#include <fstream>
#include <string>
#include <ctime>

namespace utils {

    class Logger {
    private:
        std::ofstream logFile;
        std::string logFileName;

        Logger();
        ~Logger();

        Logger(const Logger&) = delete;
        Logger& operator=(const Logger&) = delete;

        std::string generateLogFileName();
        std::string getCurrentTime();

    public:
        static Logger& getInstance();
        void log(const std::string& message);
    };

} // namespace utils

#endif // UTILS_H
