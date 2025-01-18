#include "../lib/include/logger.h"

#include <thread>
#include <mutex>
#include <condition_variable>

#include <iostream>
#include <chrono>

struct ThreadData {
    bool done = false;
    bool notified = false;
    
    LoggerLevel log_level;
    std::string message;
};

void sendLogs(const Logger& logger, ThreadData& data, std::condition_variable& cv, std::mutex& mtx) noexcept {
    while (!data.done) {
        std::unique_lock<std::mutex> locker(mtx);
        data.notified = false;


        while(!data.notified) // от ложных пробуждений
            cv.wait(locker);
    }
}

int main(int argc, char *argv[]) {
    Logger test("test.txt", LoggerLevel::INFO);
    
    ThreadData data;

    std::condition_variable cv;
    std::mutex mtx;


    std::thread logger_thread(sendLogs, std::cref(test), std::ref(data), std::ref(cv), std::ref(mtx));


    while (true) {
        std::cout << "Введите уровень сообщения и его само:\n";
        std::cin >> data.message;
        std::unique_lock<std::mutex> locker(mtx);
        
        data.notified = true;
        cv.notify_one();
    }
    // test.logInfo("info log");
    // test.logWarn("warn log");
    // test.logError("error!");


    data.done = true;
    return 0;
}