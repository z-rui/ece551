CFLAGS=-Wall -Werror -std=gnu++98 -pedantic -ggdb3
SRCS=$(wildcard *.cpp)
OBJS=$(patsubst %.cpp, %.o, $(SRCS))
PROGRAM=circles

$(PROGRAM): $(OBJS)
	g++ $(CFLAGS) -o $@ $(OBJS)

%.o: %.cpp point.h circle.h
	g++ -c $(CFLAGS) $<

clean:
	rm -f $(OBJS) $(PROGRAM) *~
