CXX=g++
CXXFLAGS=-Wall -Wextra -g -std=c++98 -ansi -pedantic
LDFLAGS=

all: myshell

myshell: myshell.o proc.o path.o repl.o parse.o var.o hash.o
	$(CXX) $(LDFLAGS) -o $@ myshell.o proc.o path.o repl.o parse.o var.o hash.o

depend: make.depends

make.depends:
	$(CXX) -MM *.cpp > $@

clean:
	rm -f *.o

include make.depends

.PHONY: all clean depend
