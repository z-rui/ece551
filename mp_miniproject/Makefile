CXX=g++
CXXFLAGS=-Wall -Wextra -g

all: myshell

myshell: myshell.o proc.o
	$(CXX) -o $@ myshell.o proc.o

myshell.o: myshell.cpp myshell.h
	$(CXX) $(CXXFLAGS) -c -o $@ $<

proc.o: proc.cpp myshell.h
	$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	rm -f *.o

.PHONY: all clean
