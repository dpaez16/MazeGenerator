#include <stdlib.h>
#include <stdio.h>
#include "maze.h"

void printUsage() {
	fprintf(stderr, "Usage: ./maze length\n");
	exit(1);
}

void checkArgs(int argc, char ** argv, int & length) {
	if (argc != 2)
		printUsage();
	
	if (sscanf(argv[1], "%d", &length) == 0)
		printUsage();

	assert(length > 0);
}

int main(int argc, char ** argv) {
	int length;
	checkArgs(argc, argv, length);

	Maze maze(length);
	
	maze.generateMaze();
	maze.solveMaze();

	return 0;
}
