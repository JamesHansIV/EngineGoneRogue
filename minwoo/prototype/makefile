CC=gcc
CXX=g++
RM=rm -f
CPPFLAGS=-I../include
LDFLAGS=
LDLIBS=-L../lib -l SDL2-2.0.0 -l SDL2_image-2.0.0 -l SDL2_ttf-2.0.0 -l SDL2_mixer-2.0.0
SRCS=main.cpp
OBJS=main.o

game: $(OBJS)
	$(CXX) $(CPPFLAGS) $(LDFLAGS) -o game $(OBJS) $(LDLIBS)

main.o: main.cpp globals.h
	$(CXX) $(CPPFLAGS) -c main.cpp

clean:
	$(RM) $(OBJS)
