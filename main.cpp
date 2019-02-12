#include "maze.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <iostream>

void printUsage() {
	fprintf(stderr, "Usage: ./maze length height\n");
	exit(1);
}

void checkArgs(int argc, char ** argv, int & length, int & width) {
	if (argc != 3)
		printUsage();
	
	if (sscanf(argv[1], "%d", &length) == 0)
		printUsage();
	if (sscanf(argv[2], "%d", &width) == 0)
		printUsage();

	assert(length != 0 && width != 0);
}

int main(int argc, char ** argv) {
	int length, width;
	checkArgs(argc, argv, length, width);

	Maze maze(length, width);
	maze.printMaze();

	return 0;
}
