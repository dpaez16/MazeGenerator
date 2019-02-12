#include "cell.h"

Cell::Cell() {
	this->eastWall = true;
	this->southWall = true;
	this->visited = false;
}

void Cell::markCellAsVisited() {
	this->visited = true;
}

void Cell::unmarkCell() {
	this->visited = false;
}


bool Cell::getEastWall() {
	return this->eastWall;
}

bool Cell::getSouthWall() {
	return this->southWall;
}

void Cell::setEastWall(bool b) {
	this->eastWall = b;
}

void Cell::setSouthWall(bool b) {
	this->southWall = b;
}
