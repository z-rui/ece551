CXX=g++
CXXFLAGS=-Wall -Wextra -g

all: myshell

myshell: myshell.o proc.o path.o
	$(CXX) -o $@ myshell.o proc.o path.o

myshell.o: myshell.cpp myshell.h
	$(CXX) $(CXXFLAGS) -c -o $@ $<

proc.o: proc.cpp myshell.h
	$(CXX) $(CXXFLAGS) -c -o $@ $<

path.o: path.cpp myshell.h
	$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	rm -f *.o

.PHONY: all clean
