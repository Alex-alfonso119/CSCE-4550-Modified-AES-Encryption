encryption: encryption.o
	g++ -o encryption encryption.o

encryption.o: encryption.cpp
	g++ -c encryption.cpp

.PHONY: clean

clean:
	rm encryption *.o output*.txt
