Tutorial: main.o
	g++ -g -Wall -pedantic main.o -o Tutorial `sdl2-config --cflags --libs`

main.o: main.cpp
	g++ -g -Wall -c -pedantic main.cpp `sdl2-config --cflags --libs`

clean:
	rm *.o Tutorial
