matrix: main.o matrix.o
	g++ -Wall -pedantic -g -o matrix_multiply main.o matrix.o

main.o: main.cpp matrix.h
	g++ -Wall -pedantic -g -c main.cpp

matrix.o: matrix.h matrix.cpp
	g++ -Wall -pedantic -g -c matrix.cpp

clean:
	rm -f *.o matrix matrix.exe
