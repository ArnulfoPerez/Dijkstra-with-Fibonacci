CC= g++ -g -O0 -std=c++0x

ssp: ssp.o fibonacci.o Graph.o
	$(CC) -o ssp ssp.o fibonacci.o Graph.o

ssp.o: ssp.cpp fibonacci.cpp fibonacci.h Graph.cpp Graph.h
	$(CC) -c ssp.cpp

Graph.o: Graph.cpp Graph.h
	$(CC) -c Graph.cpp

fibonacci.o: fibonacci.cpp fibonacci.h
	$(CC) -c fibonacci.cpp

clean:
	rm -f *.o 
	rm -f ssp
