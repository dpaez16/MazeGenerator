#include "cell.h"

Cell::Cell() {

}

Cell::Cell(int rowIdx, int colIdx) {
	this->eastWall = true;
	this->southWall = true;
	this->visited = false;
	this->rowIdx = rowIdx;
	this->colIdx = colIdx;
}

void Cell::markCellAsVisited() {
	this->visited = true;
}

void Cell::unmarkCell() {
	this->visited = false;
}

int Cell::getRowIdx() {
	return this->rowIdx;
}

int Cell::getColIdx() {
	return this->colIdx;
}

bool Cell::getEastWall() {
	return this->eastWall;
}

bool Cell::getSouthWall() {
	return this->southWall;
}

bool Cell::isVisited() {
	return this->visited;
}

void Cell::setEastWall(bool b) {
	this->eastWall = b;
}

void Cell::setSouthWall(bool b) {
	this->southWall = b;
}
