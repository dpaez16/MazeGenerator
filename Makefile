main: cell.h cell.cpp maze.h maze.cpp main.cpp
	g++ cell.h cell.cpp maze.h maze.cpp main.cpp -o maze

clean:
	rm -rf maze
	clear
	clear
