CC=gcc
CXX=g++
INPUT=brainfuck.cpp
OUTPUT=brainfuck
CPPFLAGS=-std=c++17 -Wall -Wfatal-errors -Wpedantic -Wextra


all:
	$(CXX) $(CPPFLAGS) $(INPUT) -o $(OUTPUT) 

clean:
	rm -rf *.exe
	rm -rf *.out
	rm -rf *.o

