CC=g++
CFLAGS=-std=c++17 -Wall -Wextra
LDFLAGS=
SOURCES=main.cpp hello.cpp factorial.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=app

all: logger_lib app

app: main.o
	$(CC) $(CFLAGS) ./build/main.o liblogger.so -o app.out

main.o:
	$(CC) $(CFLAGS) -I./lib/include/ -c ./src/main.cpp -o ./build/main.o

logger_lib: logger.o
	$(CC) $(CFLAGS) -shared ./build/logger.o -o ./lib/liblogger.so

logger.o:
	$(CC) $(CFLAGS) -fPIC -I./lib/include/ -c ./lib/src/logger.cpp -o ./build/logger.o