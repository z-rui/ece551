CXX=g++
CXXFLAGS=-Wall -Wextra -g -std=c++98 -ansi -pedantic
LDFLAGS=

myshell: myshell.o proc.o path.o repl.o parse.o var.o hash.o
	$(CXX) $(LDFLAGS) -o $@ myshell.o proc.o path.o repl.o parse.o var.o hash.o

parse_test: parse_test.o parse.o var.o hash.o
	$(CXX) $(LDFLAGS) -o $@ parse_test.o parse.o var.o hash.o

all: myshell parse_test

depend: make.depends

make.depends:
	$(CXX) -MM *.cpp > $@

clean:
	rm -f *.o

include make.depends

.PHONY: all clean depend
