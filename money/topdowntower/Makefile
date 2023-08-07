topdowntower: card.o deck.o player.o bigint.o table.o main.o
	g++ -Wall -pedantic -g -o topdowntower card.o deck.o player.o bigint.o table.o main.o

card.o: card.h card.cpp
	g++ -Wall -pedantic -g -c card.cpp

deck.o: deck.h deck.cpp
	g++ -Wall -pedantic -g -c deck.cpp

player.o: player.h player.cpp
	g++ -Wall -pedantic -g -c player.cpp

bigint.o: bigint.h bigint.cpp
	g++ -Wall -pedantic -g -c bigint.cpp

table.o: table.h table.cpp bigint.cpp
	g++ -Wall -pedantic -g -c table.cpp

main.o: main.cpp
	g++ -Wall -pedantic -g -c main.cpp

clean:
	rm -f *.o topdowntower topdowntower.exe
