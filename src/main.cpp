#include "../lib/include/logger.h"



int main(int argc, char *argv[]) {
    Logger test("test.txt", LoggerLevel::INFO);

    test.logInfo("info log");
    test.logWarn("warn log");
    test.logError("error!");

    return 0;
}