SOURCES=$(wildcard *.cpp)
OBJS=$(patsubst %.cpp, %.o, $(SOURCES))
CPPFLAGS=-ggdb3 -Wall -Werror -pedantic -std=gnu++98

testMap: $(OBJS)
	g++ $(CPPFLAGS) -o testMap $(OBJS)
%.o: %.cpp bitstring.h  node.h  readFreq.h
	g++ $(CPPFLAGS) -c $<

clean:
	rm testMap *~ *.o
