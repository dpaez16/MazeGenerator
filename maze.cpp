#include "maze.h"
#include <iostream>
#include <assert.h>

enum Direction {
	N,
	E,
	S, 
	W
};

Maze::Maze(int length, int width) {
	this->length = length;
	this->width = width;

	this->createEmptyMaze();
}

void Maze::generateMaze() {
	this->createEmptyMaze();

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
			Cell cell = M[rowIdx][colIdx];
			bool eastWall = cell.getEastWall();
			bool southWall = cell.getSouthWall();

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
	for (int rowIdx = 0; rowIdx < length; rowIdx++)
		this->M[rowIdx].resize(width);
}

void Maze::clearMaze() {
	if (this->M.empty())
		return;

	for (int rowIdx = 0; rowIdx < length; rowIdx++)
		this->M[rowIdx].clear();
	this->M.clear();
}
