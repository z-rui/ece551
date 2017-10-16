CCFLAGS=--std=gnu++98 -pedantic -Wall -Werror -ggdb3
intArrayTest: IntArray.o test.o
	g++ -o intArrayTest $(CCFLAGS) IntArray.o test.o

%.o: %.cpp IntArray.h
	g++ -c $(CCFLAGS) $<

clean:
	rm -f *.o  *~ intArrayTest
