all: main

main:
	g++ -c -Wall testParser.cpp

run:
	g++ testParser.cpp -o p1

lextest:
	g++ testLex.cpp -o lexTest

clean:
	rm -f *.o
	rm -f *.out
	rm -f p1
