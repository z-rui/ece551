CCFLAGS=--std=gnu++98 -pedantic -Wall -Werror -ggdb3
matrixTest:  test.o
	g++ -o matrixTest $(CCFLAGS) test.o

%.o: %.cpp Matrix.h
	g++ -c $(CCFLAGS) $<

clean:
	rm -f *.o  *~ matrixTest
