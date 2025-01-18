#include "../lib/include/logger.h"

#include <thread>
#include <mutex>
#include <condition_variable>

#include <iostream>
#include <chrono>

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
        std::cout << "thread ----> 1\n";
        std::unique_lock<std::mutex> locker(mtx);
        std::cout << "thread ----> 2\n";
        std::this_thread::sleep_for(std::chrono::seconds(1));


        while(!flags.logger_notified) {
            std::cout << "thread ----> 3\n";
            cv.wait(locker);
        }

        std::cout << "thread ----> 4\n";

        logger.logInfo(data.message);

        flags.logger_notified = false;
        flags.main_notified = true;
        cv.notify_one();

        std::cout << "thread ----> 5\n";
    }
}

int main(int argc, char *argv[]) {
    Logger test("test.txt", LoggerLevel::INFO);
    test.logInfo("test!");
    
    ThreadData data;
    Flags flags;

    std::condition_variable cv;
    std::mutex mtx;


    std::thread logger_thread(sendLogs, std::ref(test), std::cref(data), std::ref(flags), std::ref(cv), std::ref(mtx));


    while (true) { 
        std::cout <<"main ---------> 1\n";
        std::unique_lock<std::mutex> locker(mtx);
        std::cout <<"main ---------> 2\n";

        while (!flags.main_notified) {
            std::cout <<"main ---------> 3\n";
            cv.wait(locker);
        }

        std::cout << "Введите уровень сообщения и его само:\n";
        std::cin >> data.message;
        
        flags.logger_notified = true;
        flags.main_notified = false;
        std::cout <<"main ---------> 4\n";

        cv.notify_one();
        
    }


    flags.done = true;
    return 0;
}