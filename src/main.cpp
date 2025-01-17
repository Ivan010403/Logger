#include <string>
#include <fstream>
#include <memory>
#include <iostream>

enum class LoggerLevel: unsigned char {INFO, WARN, ERROR};



class Logger {
public:
    Logger(const std::string filename, LoggerLevel level) : 
        filename_(filename), 
        default_level_(level) 
    {
        logger_file_ = std::make_shared<std::ofstream> (filename, std::ios_base::app);
    }

    Logger(const Logger& src) : 
        logger_file_(src.logger_file_),
        filename_(src.filename_),
        default_level_(src.default_level_) {}

    ~Logger() = default;

    Logger() = delete;

    Logger(Logger&& src) = delete;

    Logger& operator=(const Logger& src) = delete;
    
    Logger& operator=(Logger&& src) = delete;

protected:

private:
    // не забыть про выравнивание!
    std::shared_ptr<std::ofstream> logger_file_;
    const std::string filename_;
    LoggerLevel default_level_;
};

int main(int argc, char *argv[]) {


    return 0;
}