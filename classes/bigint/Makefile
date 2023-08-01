bigint: bigint.o main.o
	g++ -Wall -pedantic -g -o bigint bigint.o main.o

bigint.o: bigint.h bigint.cpp
	g++ -Wall -pedantic -g -c bigint.cpp

main.o: main.cpp
	g++ -Wall -pedantic -g -c main.cpp

clean:
	rm -f *.o bigint bigint.exe
