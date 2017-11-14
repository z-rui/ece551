CXX=g++
CXXFLAGS=-Wall -Wextra -g
LDFLAGS=

all: myshell

myshell: myshell.o proc.o path.o repl.o parse.o
	$(CXX) $(LDFLAGS) -o $@ myshell.o proc.o path.o repl.o parse.o

myshell.o: myshell.cpp myshell.h
	$(CXX) $(CXXFLAGS) -c -o $@ $<

proc.o: proc.cpp myshell.h
	$(CXX) $(CXXFLAGS) -c -o $@ $<

path.o: path.cpp myshell.h path.h
	$(CXX) $(CXXFLAGS) -c -o $@ $<

repl.o: repl.cpp myshell.h parse.h path.h
	$(CXX) $(CXXFLAGS) -c -o $@ $<

parse.o: parse.cpp myshell.h parse.h
	$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	rm -f *.o

.PHONY: all clean
