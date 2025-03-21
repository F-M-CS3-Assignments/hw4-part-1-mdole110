all:
	g++ -std=c++11 -g -Wall biggest-divisible-conglomerate.cpp -o bdc
run: 
	./bdc
try: all run