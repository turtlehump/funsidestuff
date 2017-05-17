blackjack: questionasker.o card.o deck.o hand.o dealer.o player.o table.o blackjack.o
	g++ -Wall -pedantic -g -o blackjack questionasker.o card.o deck.o hand.o dealer.o player.o table.o blackjack.o

questionasker.o: questionasker.h questionasker.cpp
	g++ -Wall -pedantic -g -c questionasker.cpp

card.o: card.h card.cpp
	g++ -Wall -pedantic -g -c card.cpp

deck.o: deck.h deck.cpp
	g++ -Wall -pedantic -g -c deck.cpp

hand.o: hand.h hand.cpp
	g++ -Wall -pedantic -g -c hand.cpp

dealer.o: dealer.h dealer.cpp
	g++ -Wall -pedantic -g -c dealer.cpp

player.o: player.h player.cpp
	g++ -Wall -pedantic -g -c player.cpp

table.o: table.h table.cpp
	g++ -Wall -pedantic -g -c table.cpp

blackjack.o: blackjack.cpp
	g++ -Wall -pedantic -g -c blackjack.cpp

clean:
	rm -f *.o blackjack blackjack.exe
