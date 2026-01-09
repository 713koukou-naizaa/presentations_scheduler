CXX = g++
CXXFLAGS = -Wall -std=c++17 -Iinclude
LDFLAGS = -static -static-libgcc -static-libstdc++
SRC = main.cpp src/Scheduler.cpp src/Teacher.cpp src/Room.cpp src/Utils.cpp
OUT = bin/main

all:
	mkdir -p bin
	$(CXX) $(CXXFLAGS) $(SRC) -o $(OUT) $(LDFLAGS)
clean:
	rm -f bin/main