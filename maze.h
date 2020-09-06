#ifndef MAZE_H
#define MAZE_H

#include <vector>
#include <assert.h>
#include <stack>
#include <unordered_map>
#include <unordered_set>
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
		Cell *** M = NULL;

		void clearMaze();
		void createEmptyMaze();
		vector<tuple<Cell *, Cell *, Direction>> getNeighbors(Cell *& c);
		bool coordinateInsideMaze(int rowIdx, int colIdx);
		void solveMazeHelper(int startRowIdx, int startColIdx, image<rgb_pixel> & img);
		vector<pair<int, int>> getPixelNeighbors(pair<int, int> & curr, image<rgb_pixel> & img);

		struct pair_hash {
			template <class T1, class T2>
			size_t operator()(pair<T1, T2> const & pair) const {
				size_t h1 = hash<T1>()(pair.first);
				size_t h2 = hash<T1>()(pair.second);

				return h1 ^ h2;
			}
		};
};

#endif
