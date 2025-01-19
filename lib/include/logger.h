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

    // Сopy constructor is available. It just creates another shared_ptr to the underlying std::ofstream
    Logger(const Logger& src);

    ~Logger() = default;

    /*
        There is no need for using these constructors and operator.
        The only one reason for using copy construcor is the passing by value into functions.
        BUT we recommend to use passing by constant reference in order to avoid creation copy and allocation memory
        P.s. passing by value using move semantics is useless for essence of logger
    */
    Logger() = delete;
    Logger(Logger&& src) = delete;
    Logger& operator=(const Logger& src) = delete;
    Logger& operator=(Logger&& src) = delete;

    // Setter and getter for default_level_ 
    void changeLevel(LoggerLevel new_level) noexcept;
    LoggerLevel getDefaultLevel() const noexcept;

    // The 3 functions for sending logs. inside these functions the message level is checked
    // If the level is less than the default_level_, then the message is not sent
    void logInfo(std::string_view message) const noexcept;
    void logWarn(std::string_view message) const noexcept;
    void logError(std::string_view message) const noexcept;

private:
    // Functions for sending into std::ofstream source message, level and time
    void createLog(std::string_view message, std::string_view level) const noexcept;
    
    std::shared_ptr<std::ofstream> file_ptr_;
    const std::string filename_;
    LoggerLevel default_level_;

    const std::string INFO_P = " [INFO]  ";
    const std::string WARN_P = " [WARN]  ";
    const std::string ERROR_P = " [ERROR] ";
};

#endif // LOGGER_H
