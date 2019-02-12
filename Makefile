EXENAME = maze
COMPILER = g++

CELL = cell.h cell.cpp
MAZE = maze.h maze.cpp

main: $(CELL) $(MAZE) main.cpp
	$(COMPILER) $(CELL) $(MAZE) main.cpp -o $(EXENAME)

clean:
	rm -rf $(EXENAME)
	clear
	clear
