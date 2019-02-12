#ifndef MAZE_H
#define MAZE_H

#include <vector>
#include <iostream>
#include <assert.h>
#include <stack>
#include <algorithm>
#include <ctime>
#include <tuple>
#include <stdio.h>
#include <png++/png.hpp>
#include "cell.h"

using namespace std;

enum Direction {
	N, E, S, W
};

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
		vector<tuple<Cell *, Cell *, Direction>> getNeighbors(Cell *& c);
		bool coordinateInsideMaze(int rowIdx, int colIdx);
};

#endif
