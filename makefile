all: main.cpp btree 
	g++ -g -c main.cpp -o mainc
	g++ -g mainc btree -o main

btree: btree.cpp btree.hpp
	g++ -g -c btree.cpp -o btree
