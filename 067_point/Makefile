CFLAGS=-Wall -Werror -std=gnu++98 -pedantic -ggdb3
SRCS=$(wildcard *.cpp)
OBJS=$(patsubst %.cpp, %.o, $(SRCS))
PROGRAM=points

$(PROGRAM): $(OBJS)
	g++ $(CFLAGS) -o $@ $(OBJS)

%.o: %.cpp point.h
	g++ -c $(CFLAGS) $<

clean:
	rm -f $(OBJS) $(PROGRAM) *~
