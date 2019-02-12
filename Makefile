EXENAME = maze
COMPILER = g++
OBJS = main.o maze.o cell.o
LIBPNG_OBJ = libpng-config --cflags
LIBPNG_FINAL = libpng-config --ldflags --libs

CELL = cell.h cell.cpp
MAZE = maze.h maze.cpp

build: $(OBJS)
	$(COMPILER) -o $(EXENAME) $(OBJS) `$(LIBPNG_FINAL)`

main: main.cpp maze.h cell.h
	$(COMPILER) -c main.cpp maze.h cell.h `$(LIBPNG_OBJ)`

maze: $(MAZE)
	$(COMPILER) -c $(MAZE) `$(LIBPNG_OBJ)`

cell: $(CELL)
	$(COMPILER) -c $(CELL)

clean:
	rm -rf $(EXENAME) *.o *.png
	clear
	clear
