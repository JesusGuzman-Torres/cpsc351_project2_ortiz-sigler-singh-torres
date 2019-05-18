# C++ compiler and flags
CC = g++
CFLAGS = -std=c++17 -Wall -Wpedantic

# To make multiple executibles
all: simulator

# Build receiver
simulator: main.cpp  ProcessManager.cpp MemoryManager.cpp genericImports.h process.h
	$(CC) $(CFLAGS) -cpp main.cpp -o simulator

# Clean up
clean:
	rm -f core simulator
