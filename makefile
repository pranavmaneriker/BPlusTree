all: main btree 
	g++ -g mainc btree -o main

main:
	g++ -g -c main.cpp -o mainc
btree: btree.cpp btree.hpp
	g++ -g -c btree.cpp -o btree

clean: 
	rm -f btree mainc main