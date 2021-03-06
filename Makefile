EXENAME=maze
CC=g++
#CC_FLAGS=-g
CC_FLAGS=-O2 -Wall
OBJS=main.o maze.o cell.o
LIBPNG_OBJ=libpng-config --cflags
LIBPNG_FINAL=libpng-config --ldflags --libs

CELL = cell.h cell.cpp
MAZE = maze.h maze.cpp

build: $(OBJS)
	$(CC) $(CC_FLAGS) -o $(EXENAME) $(OBJS) `$(LIBPNG_FINAL)`

main.o: main.cpp maze.h cell.h
	$(CC) $(CC_FLAGS) -c main.cpp maze.h cell.h `$(LIBPNG_OBJ)`

maze.o: $(MAZE)
	$(CC) $(CC_FLAGS) -c $(MAZE) `$(LIBPNG_OBJ)`

cell.o: $(CELL)
	$(CC) $(CC_FLAGS) -c $(CELL)

clean:
	rm -rf $(EXENAME) *.o *.gch *.png
	clear
	clear
