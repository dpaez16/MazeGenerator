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
		~Maze();

	private:
		int length;
		int width;
		vector<vector<Cell *>> M;

		void clearMaze();
		void createEmptyMaze();
		vector<Cell *> getNeighbors(Cell *& c);
		bool coordinateInsideMaze(int rowIdx, int colIdx);
};

#endif
