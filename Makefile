CC=g++
CFLAGS=-std=c++17 -Wall -Wextra

all: build_dir logger_lib app 

# creation build folder
build_dir:
	@if ! [ -d ./build ]; then\
        mkdir build;\
    fi 

# creation dynamic library
logger_lib: logger.o
	$(CC) $(CFLAGS) -shared ./build/logger.o -o ./lib/liblogger.so

logger.o:
	$(CC) $(CFLAGS) -fPIC -I./lib/include/ -c ./lib/src/logger.cpp -o ./build/logger.o

# creation application using library
app: main.o
	$(CC) $(CFLAGS) ./build/main.o -L./lib -llogger -o ./build/app -Wl,-rpath,../lib/

main.o:
	$(CC) $(CFLAGS) -I./lib/include/ -c ./src/main.cpp -o ./build/main.o

clean:
	rm -f build/*.o