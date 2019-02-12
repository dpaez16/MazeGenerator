#include "maze.h"

Maze::Maze(int length) {
	this->length = length;

	this->createEmptyMaze();
}

Maze::~Maze() {
	this->clearMaze();
}

void markProperWall(Cell *& previousCell, Cell *& currentCell, Direction dir, image<rgb_pixel> & img) {
	if (previousCell == nullptr)
		return;

	int rowIdx = previousCell->getRowIdx();
	int colIdx = previousCell->getColIdx();

	if (dir == N) {
		img[2*rowIdx][2*colIdx + 1] = rgb_pixel(255, 255, 255);
		currentCell->setSouthWall(false);
	} else if (dir == E) {
		img[2*rowIdx + 1][2*colIdx + 2] = rgb_pixel(255, 255, 255);
		previousCell->setEastWall(false);
	} else if (dir == S) {
		img[2*rowIdx + 2][2*colIdx + 1] = rgb_pixel(255, 255, 255);
		previousCell->setSouthWall(false);
	} else {
		img[2*rowIdx + 1][2*colIdx] = rgb_pixel(255, 255, 255);
		currentCell->setEastWall(false);
	}
}

void emptyPNG(image<rgb_pixel> & img) {
	for (size_t rowIdx = 0; rowIdx < img.get_height(); rowIdx++) {
		for (size_t colIdx = 0; colIdx < img.get_width(); colIdx++) {
			if (rowIdx % 2 == 0 || colIdx % 2 == 0)
				img[rowIdx][colIdx] = rgb_pixel(0, 0, 0);
			else
				img[rowIdx][colIdx] = rgb_pixel(255, 255, 255);
		}
	}
	img[1][0] = rgb_pixel(255, 255, 255);
	img[img.get_height() - 2][img.get_width() - 1] = rgb_pixel(255, 255, 255);
}

void clearPNG(image<rgb_pixel> & img) {
	for (size_t rowIdx = 0; rowIdx < img.get_height(); rowIdx++) {
		for (size_t colIdx = 0; colIdx < img.get_width(); colIdx++) {
			if (rowIdx % 2 == 0 || colIdx % 2 == 0)
				img[rowIdx][colIdx] = rgb_pixel(0, 0, 0);
			else
				img[rowIdx][colIdx] = rgb_pixel(255, 255, 255);
		}
	}
	img[1][0] = rgb_pixel(255, 255, 255);
	img[img.get_height() - 2][img.get_width() - 1] = rgb_pixel(255, 255, 255);
}

void Maze::generateMaze() {
	this->createEmptyMaze();
	srand(time(0));
	
	image<rgb_pixel> unsolvedImage(2*this->length + 1, 2*this->length + 1);
	clearPNG(unsolvedImage);

	stack<tuple<Cell *, Cell *, Direction>> s;
	s.push(make_tuple(nullptr, this->M[0][0], E));

	while(!s.empty()) {
		tuple<Cell *, Cell *, Direction> t = s.top();
		s.pop();
		
		Cell * previousCell = get<0>(t);
		Cell * currentCell = get<1>(t);
		Direction dir = get<2>(t);

		if (currentCell->isVisited())
			continue;

		currentCell->markCellAsVisited();
		markProperWall(previousCell, currentCell, dir, unsolvedImage);

		vector<tuple<Cell *, Cell *, Direction>> neighbors = this->getNeighbors(currentCell);
		random_shuffle(begin(neighbors), end(neighbors));

		for (tuple<Cell *, Cell *, Direction> neighbor : neighbors)
			s.push(neighbor);
	}
	unsolvedImage.write("unsolvedMaze.png");
}

void Maze::solveMaze() {
	assert(this->M.empty() == false);
	
	return;
}

// Helper functions

void Maze::createEmptyMaze() {
	this->clearMaze();

	this->M.resize(this->length);
	for (int rowIdx = 0; rowIdx < this->length; rowIdx++) {
		this->M[rowIdx].resize(this->length);
		for (int colIdx = 0; colIdx < this->length; colIdx++) {
			this->M[rowIdx][colIdx] = new Cell(rowIdx, colIdx);
		}
	}
}

void Maze::clearMaze() {
	if (this->M.empty())
		return;

	for (int rowIdx = 0; rowIdx < this->length; rowIdx++) {
		for (int colIdx = 0; colIdx < this->length; colIdx++) {
			delete this->M[rowIdx][colIdx];
		}
		this->M[rowIdx].clear();
	}

	this->M.clear();	
}

bool Maze::coordinateInsideMaze(int rowIdx, int colIdx) {
	return (0 <= rowIdx) && (rowIdx < this->length) && (0 <= colIdx) && (colIdx < this->length);
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
		neighbors.push_back(make_tuple(c, neighbor, E));
	}

	if (this->coordinateInsideMaze(rowIdx + 1, colIdx)) {
		Cell * neighbor = this->M[rowIdx + 1][colIdx];
		neighbors.push_back(make_tuple(c, neighbor, S));
	}
	
	if (this->coordinateInsideMaze(rowIdx, colIdx - 1)) {
		Cell * neighbor = this->M[rowIdx][colIdx - 1];
		neighbors.push_back(make_tuple(c, neighbor, W));
	}
	

	return neighbors;
}
