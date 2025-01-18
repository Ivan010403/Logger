#include "../include/logger.h"

Logger::Logger(const std::string filename, LoggerLevel default_level) : 
        filename_(filename), 
        default_level_(default_level) 
    {
        file_ptr_ = std::make_shared<std::ofstream> (filename, std::ios_base::app);
    }

Logger::Logger(const Logger& src) : 
        file_ptr_(src.file_ptr_),
        filename_(src.filename_),
        default_level_(src.default_level_) {}

void Logger::changeLevel(LoggerLevel new_level) noexcept {
    default_level_ = new_level;
}

void Logger::logInfo(std::string_view message) noexcept {
    if (default_level_ <= LoggerLevel::INFO) {
        createLog(message, INFO_P);
    }
}

void Logger::logWarn(std::string_view message) noexcept {
    if (default_level_ <= LoggerLevel::WARN) {
        createLog(message, WARN_P);
    }
}

void Logger::logError(std::string_view message) noexcept { // maybe noexcept(true)?
    if (default_level_ <= LoggerLevel::ERROR) {
        createLog(message, ERROR_P);
    }
}

void Logger::createLog(std::string_view message, std::string_view level) noexcept {
    time_t now = time(0);
    tm* timeinfo = localtime(&now);

    char timestamp[20];
    strftime(timestamp, sizeof(timestamp), "%d.%m.%Y %H:%M:%S", timeinfo);

    *file_ptr_ << timestamp << level << message << '\n';
    (*file_ptr_).flush();
}