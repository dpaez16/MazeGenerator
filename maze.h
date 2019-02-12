#ifndef MAZE_H
#define MAZE_H

#include <vector>
#include <assert.h>
#include <stack>
#include <queue>
#include <algorithm>
#include <ctime>
#include <tuple>
#include <stdio.h>
#include <png++/png.hpp>
#include "cell.h"

using namespace std;
using namespace png;

enum Direction {
	N, E, S, W
};

class Maze {
	public:
		Maze(int length);
		void generateMaze();
		void solveMaze();	
		~Maze();

	private:
		int length;
		vector<vector<Cell *>> M;

		void clearMaze();
		void createEmptyMaze();
		vector<tuple<Cell *, Cell *, Direction>> getNeighbors(Cell *& c);
		bool coordinateInsideMaze(int rowIdx, int colIdx);
};

#endif
