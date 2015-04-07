all: maintar btreetar
	g++ -g mainc btree -o main

maintar: main.cpp
	g++ -g -c main.cpp -o mainc
btreetar: btree.cpp btree.hpp
	g++ -g -c btree.cpp -o btree

clean: 
	rm -f btree mainc main