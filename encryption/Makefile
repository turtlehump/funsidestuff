encrypter: main.o encrypter.o
	g++ -Wall -pedantic -g -o encrypter main.o encrypter.o

main.o: main.cpp encrypter.h
	g++ -Wall -pedantic -g -c main.cpp

encrypter.o: encrypter.h encrypter.cpp
	g++ -Wall -pedantic -g -c encrypter.cpp

clean:
	rm -f *.o encrypter encrypter.exe
