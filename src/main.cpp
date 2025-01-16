#include <string>

enum class LoggerLevel: unsigned char {INFO, WARN, ERROR};

class Logger {
public:
    Logger() = delete;

    Logger(const std::string filename, LoggerLevel level) : filename_(filename), default_level_(level) {}

    Logger(const Logger& src) = default;

    Logger(Logger&& src) = default;

    ~Logger() = default;

    // There are no needs for this operator
    Logger& operator=(const Logger& src) = delete;
    
    // There are no needs for this operator
    Logger& operator=(Logger&& src) = delete;

protected:

private:
    // не забыть про выравнивание!
    const std::string filename_;
    LoggerLevel default_level_;
};

int main(int argc, char *argv[]) {

    std::string test2= "hello world";

    Logger test(test2, LoggerLevel::INFO);

    return 0;
}