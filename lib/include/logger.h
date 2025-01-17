#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <fstream>
#include <memory>
#include <ctime>

enum class LoggerLevel: unsigned char {INFO, WARN, ERROR};

class Logger {
public:
    Logger(const std::string filename, LoggerLevel default_level);

    Logger(const Logger& src);

    ~Logger() = default;

    // There is no need for using these constructors and operators
    Logger() = delete;
    Logger(Logger&& src) = delete;
    Logger& operator=(const Logger& src) = delete;
    Logger& operator=(Logger&& src) = delete;

    void changeLevel(LoggerLevel new_level) noexcept;

    void logInfo(std::string_view message) noexcept;

    void logWarn(std::string_view message) noexcept;

    void logError(std::string_view message) noexcept;

private:
    void createLog(std::string_view message, std::string_view level) noexcept;
    
    // не забыть про выравнивание!
    std::shared_ptr<std::ofstream> file_ptr_;
    const std::string filename_;
    LoggerLevel default_level_;

    const std::string INFO_P = " [INFO]  ";
    const std::string WARN_P = " [WARN]  ";
    const std::string ERROR_P = " [ERROR] ";
};

#endif // LOGGER_H
