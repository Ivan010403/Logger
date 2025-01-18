#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>

#include "../lib/include/logger.h"

struct ThreadData {    
    LoggerLevel log_level;
    std::string message;
};

struct Flags {
    bool done = false;
    bool logger_notified = false;
    bool main_notified = true;
};

void sendLogs(Logger& logger, const ThreadData& data, Flags& flags, std::condition_variable& cv, std::mutex& mtx) noexcept {
    while (!flags.done) {
        std::unique_lock<std::mutex> locker(mtx);

        while(!flags.logger_notified) {
            cv.wait(locker);
        }

        if (flags.done) {
            return;
        }

        if      (data.log_level == LoggerLevel::INFO)   logger.logInfo(data.message);
        else if (data.log_level == LoggerLevel::WARN)   logger.logWarn(data.message);
        else if (data.log_level == LoggerLevel::ERROR)  logger.logError(data.message);


        flags.logger_notified = false;
        flags.main_notified = true;
        cv.notify_one();
    }
}

// что если нет такого файла? или он был удален!


// forward declarations of simple functions for validation data from users
void validateData(std::string& filename, int& level);
void validateData(int& level);


int main(int argc, char *argv[]) {    
    ThreadData data;
    Flags flags;

    std::condition_variable cv;
    std::mutex mtx;
    
    std::string filename;
    int level;

    validateData(filename, level);
    Logger logger(filename, static_cast<LoggerLevel>(level));
    
    std::thread logger_thread(sendLogs, std::ref(logger), std::cref(data), std::ref(flags), std::ref(cv), std::ref(mtx));


    int choice;
    while (true) {
        std::cout << "\nIf you want:\n"
                     "- Send message - write \"0\"\n"
                     "- Change default level of logging - write \"1\"\n"
                     "- Exit the program - write \"2\"\n";
        std::cin >> choice; 
        validateData(choice);

        switch (choice)
        {
        case 0:
            while (true) {
                std::unique_lock<std::mutex> locker(mtx);
                
                while (!flags.main_notified) {
                    cv.wait(locker);
                }

                std::cout << "\nEnter the message ('q' for break):\n";
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                getline(std::cin, data.message);
                
                if (data.message[0] == 'q') {
                    break;
                }

                std::cout << "Enter the level of message (0-INFO, 1-WARN, 2-ERROR):\n";
                std::cin >> level;
                validateData(level);
                data.log_level = static_cast<LoggerLevel>(level);

                
                flags.logger_notified = true;
                flags.main_notified = false;
                cv.notify_one();
            }
            break;
        
        case 1:
            std::cout << "Enter a new default level (0-INFO, 1-WARN, 2-ERROR):\n";
            std::cin >> level;
            validateData(level);

            logger.changeLevel(static_cast<LoggerLevel>(level));

            break;

        case 2:
            flags.done = true;
            flags.logger_notified = true;
            cv.notify_one();

            logger_thread.join();

            return 0;
        } 
    }
}


// validation functions

void validateData(std::string& filename, int& level) {
    std::cout << "Enter file name and default level of logging (0-INFO, 1-WARN, 2-ERROR)\n"
                 "For example, \"test.txt 1\"\n";

    std::cin >> filename >> level;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    while (filename.length() == 0 or level < 0 or level > 2 or std::cin.fail()) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        std::cout << "Input data is incorrect. Try again\n";
        std::cin >> filename >> level;
    }  
}

void validateData(int& level) {
    while (level < 0 or level > 2 or std::cin.fail()) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        std::cout << "Value is incorrect. Try again\n";
        std::cin >> level;
    }
}