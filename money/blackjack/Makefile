blackjack: card.o deck.o table.o player.o blackjack.o
	g++ -Wall -pedantic -g -o blackjack card.o deck.o table.o player.o blackjack.o

card.o: card.h card.cpp
	g++ -Wall -pedantic -g -c card.cpp

deck.o: deck.h deck.cpp
	g++ -Wall -pedantic -g -c deck.cpp

table.o: table.h table.cpp
	g++ -Wall -pedantic -g -c table.cpp

player.o: player.h player.cpp
	g++ -Wall -pedantic -g -c player.cpp

blackjack.o: blackjack.cpp deck.h
	g++ -Wall -pedantic -g -c blackjack.cpp

clean:
	rm -f *.o blackjack blackjack.exe
