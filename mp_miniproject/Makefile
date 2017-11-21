CXX=g++
CXXFLAGS=-Wall -Wextra -g -std=c++98 -ansi -pedantic
LDFLAGS=

all: myshell

myshell: myshell.o proc.o path.o repl.o parse.o var.o
	$(CXX) $(LDFLAGS) -o $@ myshell.o proc.o path.o repl.o parse.o var.o

myshell.o: myshell.cpp repl.cpp myshell.h path.h parse.h var.h debug.h
	$(CXX) $(CXXFLAGS) -c -o $@ $<

proc.o: proc.cpp myshell.h
	$(CXX) $(CXXFLAGS) -c -o $@ $<

path.o: path.cpp path.h
	$(CXX) $(CXXFLAGS) -c -o $@ $<

repl.o: repl.cpp myshell.h parse.h path.h var.h
	$(CXX) $(CXXFLAGS) -c -o $@ $<

parse.o: parse.cpp myshell.h parse.h
	$(CXX) $(CXXFLAGS) -c -o $@ $<

var.o: var.cpp var.h
	$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	rm -f *.o

.PHONY: all clean
