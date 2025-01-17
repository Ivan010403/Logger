#include <string>
#include <fstream>
#include <memory>
#include <ctime>

#include <iostream>

enum class LoggerLevel: unsigned char {INFO, WARN, ERROR};

class Logger {
public:
    Logger(const std::string filename, LoggerLevel default_level) : 
        filename_(filename), 
        default_level_(default_level) 
    {
        file_ptr_ = std::make_shared<std::ofstream> (filename, std::ios_base::app);
    }

    Logger(const Logger& src) : 
        file_ptr_(src.file_ptr_),
        filename_(src.filename_),
        default_level_(src.default_level_) {}

    ~Logger() = default;

    // There is no need for using these constructors and operators
    Logger() = delete;
    Logger(Logger&& src) = delete;
    Logger& operator=(const Logger& src) = delete;
    Logger& operator=(Logger&& src) = delete;

    void changeLevel(LoggerLevel new_level) noexcept {
        default_level_ = new_level;
    }

    void logInfo(std::string_view message) noexcept {
        if (default_level_ <= LoggerLevel::INFO) {
            createLog(message, INFO_P);
        }
    }

    void logWarn(std::string_view message) noexcept {
        if (default_level_ <= LoggerLevel::WARN) {
            createLog(message, WARN_P);
        }
    }

    void logError(std::string_view message) noexcept { // maybe noexcept(true)?
        if (default_level_ <= LoggerLevel::ERROR) {
            createLog(message, ERROR_P);
        }
    }

private:
    void createLog(std::string_view message, std::string_view level) noexcept {
        time_t now = time(0);
        tm* timeinfo = localtime(&now);

        char timestamp[20];
        strftime(timestamp, sizeof(timestamp), "%d.%m.%Y %H:%M:%S", timeinfo);

        // не будет ли быстрее сделать новую строку?
        *file_ptr_ << timestamp << level << message << '\n';
    }
    // не забыть про выравнивание!
    std::shared_ptr<std::ofstream> file_ptr_;
    const std::string filename_;
    LoggerLevel default_level_;

    const std::string INFO_P = " [INFO] ";
    const std::string WARN_P = " [WARN] ";
    const std::string ERROR_P = " [ERROR] ";
};

int main(int argc, char *argv[]) {
    Logger test("test.txt", LoggerLevel::INFO);

    test.logInfo("info log");
    test.logWarn("warn log");
    test.logError("error!");



    return 0;
}