topdowntower: card.o deck.o player.o table.o topdowntower.o
	g++ -Wall -pedantic -g -o topdowntower card.o deck.o player.o table.o topdowntower.o

card.o: card.h card.cpp
	g++ -Wall -pedantic -g -c card.cpp

deck.o: deck.h deck.cpp
	g++ -Wall -pedantic -g -c deck.cpp

player.o: player.h player.cpp
	g++ -Wall -pedantic -g -c player.cpp

table.o: table.h table.cpp
	g++ -Wall -pedantic -g -c table.cpp

topdowntower.o: topdowntower.cpp
	g++ -Wall -pedantic -g -c topdowntower.cpp

clean:
	rm -f *.o topdowntower topdowntower.exe
