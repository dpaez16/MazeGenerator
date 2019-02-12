#ifndef MAZE_H
#define MAZE_H

#include <vector>
#include "cell.h"

using namespace std;

class Maze {
	public:
		Maze(int length, int width);
		void generateMaze();
		void solveMaze();	
		void printMaze();

	private:
		int length;
		int width;
		vector<vector<Cell>> M;

		void clearMaze();
		void createEmptyMaze();
};

#endif
