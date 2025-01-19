CC=g++
CFLAGS=-std=c++17 -Wall -Wextra

all: logger_lib app 

app: main.o
	$(CC) $(CFLAGS) ./build/main.o -L./lib -llogger -o ./build/app.out -Wl,-rpath,../lib/

main.o:
	$(CC) $(CFLAGS) -I./lib/include/ -c ./src/main.cpp -o ./build/main.o

logger_lib: logger.o
	$(CC) $(CFLAGS) -shared ./build/logger.o -o ./lib/liblogger.so

logger.o:
	$(CC) $(CFLAGS) -fPIC -I./lib/include/ -c ./lib/src/logger.cpp -o ./build/logger.o

clean:
	rm -f build/*.o