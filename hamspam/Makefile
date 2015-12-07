hamspam: main.o sortinghat.o
	g++ -Wall -pedantic -g -o hamspam main.o sortinghat.o

main.o: main.cpp sortinghat.h
	g++ -Wall -pedantic -g -c main.cpp

sortinghat.o: sortinghat.h sortinghat.cpp
	g++ -Wall -pedantic -g -c sortinghat.cpp

clean:
	rm -f *.o hamspam hamspam.exe trained.data
