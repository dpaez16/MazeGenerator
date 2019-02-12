#include "maze.h"
#include <iostream>
#include <assert.h>
#include <stack>

enum Direction {
	N, E, S, W
};

Maze::Maze(int length, int width) {
	this->length = length;
	this->width = width;

	this->createEmptyMaze();
}

Maze::~Maze() {
	this->clearMaze();
}

void Maze::generateMaze() {
	this->createEmptyMaze();

	std::stack<Cell *> s;
	s.push(this->M[0][0]);

	while(!s.empty()) {
		Cell * currentCell = s.top();
		s.pop();

		currentCell->setEastWall(false);

		vector<Cell *> neighbors = this->getNeighbors(currentCell);
		for (Cell * neighbor : neighbors) {
			neighbor->markCellAsVisited();
			s.push(neighbor);
		}
	}

	return;
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
				std::cout << "_|";
			else if (eastWall && !southWall)
				std::cout << " |";
			else if (!eastWall && southWall)
				std::cout << "_ ";
			else
				std::cout << "  ";
		}
		std::cout << endl;
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
	return (0 <= rowIdx) && (rowIdx < this->length) && (0 <= colIdx) && (colIdx <= this->width);
}

bool canGoToNeighbor(Cell *& neighbor, Direction dir) {
	assert((dir == N) || (dir == W));

	if (dir == N)
		return !neighbor->getSouthWall() && !neighbor->isVisited();
	else // dir == W
		return !neighbor->getEastWall() && !neighbor->isVisited();
}

vector<Cell *> Maze::getNeighbors(Cell *& c) {
	vector<Cell *> neighbors;
	int rowIdx = c->getRowIdx();
	int colIdx = c->getColIdx();

	if (this->coordinateInsideMaze(rowIdx - 1, colIdx)) {
		Cell * neighbor = this->M[rowIdx][colIdx];
		if (canGoToNeighbor(neighbor, N))
			neighbors.push_back(neighbor);
	}

	if (this->coordinateInsideMaze(rowIdx + 1, colIdx)) {
		if (!c->getSouthWall() && !c->isVisited())
			neighbors.push_back(this->M[rowIdx + 1][colIdx]);
	}
	
	if (this->coordinateInsideMaze(rowIdx, colIdx - 1)) {
		if (!c->getEastWall() && !c->isVisited())
			neighbors.push_back(this->M[rowIdx][colIdx - 1]);
	}
	
	if (this->coordinateInsideMaze(rowIdx, colIdx + 1)) {
		Cell * neighbor = this->M[rowIdx][colIdx + 1];
		if (canGoToNeighbor(neighbor, W))
			neighbors.push_back(neighbor);
	}

	return neighbors;
}
