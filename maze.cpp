#include "maze.h"

Maze::Maze(int length, int width) {
	this->length = length;
	this->width = width;

	this->createEmptyMaze();
}

Maze::~Maze() {
	this->clearMaze();
}

void markProperWall(Cell *& previousCell, Cell *& currentCell, Direction dir) {
	if (previousCell == nullptr)
		return;

	if (dir == N)
		currentCell->setSouthWall(false);
	else if (dir == E)
		previousCell->setEastWall(false);
	else if (dir == S)
		previousCell->setSouthWall(false);
	else
		currentCell->setEastWall(false);
}

void Maze::generateMaze() {
	this->createEmptyMaze();
	srand(time(0));

	png::image<png::rgb_pixel> img(this->length, this->width);
	//char fileName[50];
	//sprintf(fileName, "maze_%d_%d.png", this->length, this->width);
	img.write("maze_unsolved.png");

	stack<tuple<Cell *, Cell *, Direction>> s;
	s.push(make_tuple(nullptr, this->M[0][0], E));

	while(!s.empty()) {
		tuple<Cell *, Cell *, Direction> t = s.top();
		
		Cell * previousCell = get<0>(t);
		Cell * currentCell = get<1>(t);
		Direction dir = get<2>(t);

		currentCell->markCellAsVisited();
		s.pop();

		markProperWall(previousCell, currentCell, dir);

		vector<tuple<Cell *, Cell *, Direction>> neighbors = this->getNeighbors(currentCell);
		random_shuffle(begin(neighbors), end(neighbors));

		for (tuple<Cell *, Cell *, Direction> neighbor : neighbors)
			s.push(neighbor);
	}
}

void Maze::solveMaze() {
	assert(this->M.empty() == false);
	
	return;
}

void Maze::printMaze() {
	assert(this->M.empty() == false);

	for (int rowIdx = 0; rowIdx < this->length; rowIdx++) {
		for (int colIdx = 0; colIdx < this->width; colIdx++) {
			Cell * cell = M[rowIdx][colIdx];
			bool eastWall = cell->getEastWall();
			bool southWall = cell->getSouthWall();

			if (eastWall && southWall)
				cout << "_|";
			else if (eastWall && !southWall)
				cout << " |";
			else if (!eastWall && southWall)
				cout << "_ ";
			else
				cout << "  ";
		}
		cout << endl;
	}
}

// Helper functions

void Maze::createEmptyMaze() {
	this->clearMaze();

	this->M.resize(length);
	for (int rowIdx = 0; rowIdx < length; rowIdx++) {
		this->M[rowIdx].resize(width);
		for (int colIdx = 0; colIdx < width; colIdx++) {
			this->M[rowIdx][colIdx] = new Cell(rowIdx, colIdx);
		}
	}
}

void Maze::clearMaze() {
	if (this->M.empty())
		return;

	for (int rowIdx = 0; rowIdx < this->length; rowIdx++) {
		for (int colIdx = 0; colIdx < this->width; colIdx++) {
			delete this->M[rowIdx][colIdx];
		}
		this->M[rowIdx].clear();
	}

	this->M.clear();
}

bool Maze::coordinateInsideMaze(int rowIdx, int colIdx) {
	return (0 <= rowIdx) && (rowIdx < this->length) && (0 <= colIdx) && (colIdx < this->width);
}

vector<tuple<Cell *, Cell *, Direction>> Maze::getNeighbors(Cell *& c) {
	vector<tuple<Cell *, Cell *, Direction>> neighbors;
	int rowIdx = c->getRowIdx();
	int colIdx = c->getColIdx();

	if (this->coordinateInsideMaze(rowIdx - 1, colIdx)) {
		Cell * neighbor = this->M[rowIdx - 1][colIdx];
		if (!neighbor->isVisited())
			neighbors.push_back(make_tuple(c, neighbor, N));
	}
	
	if (this->coordinateInsideMaze(rowIdx, colIdx + 1)) {
		Cell * neighbor = this->M[rowIdx][colIdx + 1];
		if (!neighbor->isVisited())
			neighbors.push_back(make_tuple(c, neighbor, E));
	}

	if (this->coordinateInsideMaze(rowIdx + 1, colIdx)) {
		Cell * neighbor = this->M[rowIdx + 1][colIdx];
		if (!neighbor->isVisited())
			neighbors.push_back(make_tuple(c, neighbor, S));
	}
	
	if (this->coordinateInsideMaze(rowIdx, colIdx - 1)) {
		Cell * neighbor = this->M[rowIdx][colIdx - 1];
		if (!neighbor->isVisited())
			neighbors.push_back(make_tuple(c, neighbor, W));
	}
	

	return neighbors;
}
