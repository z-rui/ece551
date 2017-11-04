SOURCES=bitstring.cpp  buildTree.cpp  compress.cpp  node.cpp  readFreq.cpp
OBJS=$(patsubst %.cpp, %.o, $(SOURCES))
CPPFLAGS=-ggdb3 -Wall -Werror -pedantic -std=gnu++98

compress: $(OBJS)
	g++ $(CPPFLAGS) -o compress $(OBJS)
%.o: %.cpp bitstring.h  node.h  readFreq.h
	g++ $(CPPFLAGS) -c $<

clean:
	rm compress *~ *.o
