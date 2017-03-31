question_asker: questionasker.o main.o
	g++ -Wall -pedantic -g -o question_asker questionasker.o main.o

questionasker.o: questionasker.h questionasker.cpp
	g++ -Wall -pedantic -g -c questionasker.cpp

main.o: main.cpp
	g++ -Wall -pedantic -g -c main.cpp

clean:
	rm -f *.o question_asker question_asker.exe
