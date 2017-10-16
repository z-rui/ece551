CCFLAGS=--std=gnu++98 -pedantic -Wall -Werror -ggdb3
intMatrixTest: IntArray.o IntMatrix.o test.o
	g++ -o intMatrixTest $(CCFLAGS) IntArray.o IntMatrix.o test.o

%.o: %.cpp IntArray.h IntMatrix.h
	g++ -c $(CCFLAGS) $<

clean:
	rm -f *.o  *~ intMatrixTest
