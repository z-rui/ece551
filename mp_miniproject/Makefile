CXX=g++
CXXFLAGS=-Wall -Wextra -Werror -std=c++03 -pedantic
CXXFLAGS+=-O2
LDFLAGS=

all: myshell parse_test var_test path_test TESTING.txt

myshell: myshell.o proc.o path.o repl.o parse.o var.o hash.o
	$(CXX) $(LDFLAGS) -o $@ myshell.o proc.o path.o repl.o parse.o var.o hash.o
	cp myshell myShell	# who likes capital letters?

parse_test: parse_test.o parse.o var.o hash.o
	$(CXX) $(LDFLAGS) -o $@ parse_test.o parse.o var.o hash.o

var_test: var_test.o var.o hash.o
	$(CXX) $(LDFLAGS) -o $@ var_test.o var.o hash.o

path_test: path_test.o path.o
	$(CXX) $(LDFLAGS) -o $@ path_test.o path.o

TESTING.txt: maketesting.sh
	./$< > $@

depend:
	$(CXX) -MM *.cpp > make.depends

clean:
	rm -f *.o

include make.depends

.PHONY: all clean depend
