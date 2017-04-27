craps_field_bet_simulation: questionasker.o sim.o
	g++ -Wall -pedantic -g -o craps_field_bet_simulation questionasker.o sim.o

questionasker.o: questionasker.h questionasker.cpp
	g++ -Wall -pedantic -g -c questionasker.cpp

sim.o: field_bet_simulation.cpp
	g++ -Wall -pedantic -g -c field_bet_simulation.cpp -o sim.o

clean:
	rm -f *.o craps_field_bet_simulation craps_field_bet_simulation.exe
