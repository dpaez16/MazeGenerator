#include "maze.h"
#include <iostream>
#include <unordered_set>
#include <unordered_map>

Maze::Maze(int length) {
	this->length = length;
	this->createEmptyMaze();
}

Maze::~Maze() {
	this->clearMaze();
}

void markProperWall(Cell *& previousCell, Cell *& currentCell, Direction dir, image<rgb_pixel> & img) {
	if (previousCell == NULL)
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
	//srand(time(0));
	srand(400);
	
	image<rgb_pixel> unsolvedImage(2*this->length + 1, 2*this->length + 1);
	clearPNG(unsolvedImage);

	stack<tuple<Cell *, Cell *, Direction>> s;
	s.push(make_tuple((Cell *)NULL, this->M[0][0], E));

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

bool pixelIsWhite(rgb_pixel & pixel) {
	return ((int)pixel.red == 255) && ((int)pixel.green == 255) && ((int)pixel.blue == 255);
}

bool coordinateInsidePicture(int rowIdx, int colIdx, image<rgb_pixel> & img) {
	return (0 <= rowIdx) && (rowIdx < img.get_height()) && (0 <= colIdx) && (colIdx < img.get_width());
}

vector<pair<int, int>> Maze::getPixelNeighbors(int rowIdx, int colIdx, image<rgb_pixel> & img) {
	vector<pair<int, int>> neighbors;
	neighbors.reserve(4);

	if (coordinateInsidePicture(rowIdx - 1, colIdx, img)) {
		if (pixelIsWhite(img[rowIdx - 1][colIdx]))
			neighbors.push_back(pair<int, int>(rowIdx - 1, colIdx));
	}
	
	if (coordinateInsidePicture(rowIdx, colIdx + 1, img)) {
		if (pixelIsWhite(img[rowIdx][colIdx + 1]))
			neighbors.push_back(pair<int, int>(rowIdx, colIdx + 1));
	}
	
	if (coordinateInsidePicture(rowIdx + 1, colIdx, img)) {
		if (pixelIsWhite(img[rowIdx + 1][colIdx]))
			neighbors.push_back(pair<int, int>(rowIdx + 1, colIdx));
	}

	if (coordinateInsidePicture(rowIdx, colIdx - 1, img)) {
		if (pixelIsWhite(img[rowIdx][colIdx - 1]))
			neighbors.push_back(pair<int, int>(rowIdx, colIdx - 1));
	}
	
	return neighbors;
}

bool ** Maze::getVisitedMatrix() {
	bool ** visitedMatrix;

	visitedMatrix = new bool * [2*this->length + 1];
	for (int rowIdx = 0; rowIdx < (2*this->length + 1); rowIdx++) {
		visitedMatrix[rowIdx] = new bool [2*this->length + 1];
		for (int colIdx = 0; colIdx < (2*this->length + 1); colIdx++) {
			visitedMatrix[rowIdx][colIdx] = false;
		}
	}

	return visitedMatrix;
}

struct pair_hash {
	template <class T1, class T2>
	size_t operator()(pair<T1, T2> const & pair) const {
		size_t h1 = hash<T1>()(pair.first);
		size_t h2 = hash<T1>()(pair.second);
		
		return h1 ^ h2;
	}
};

void Maze::solveMazeHelper(int startRowIdx, int startColIdx, image<rgb_pixel> & img, bool **& visited) {
	unordered_map<pair<int, int>, unsigned, pair_hash> dist;
	unordered_map<pair<int, int>, pair<int, int>, pair_hash> prev;
	unordered_set<pair<int, int>, pair_hash> q;

	for (int rowIdx = 0; rowIdx < (2*this->length + 1); rowIdx++) {
		for (int colIdx = 0; colIdx < (2*this->length + 1); colIdx++) {
			pair<int, int> curr(rowIdx, colIdx);
			dist[curr] = -1; //infinity
			prev[curr] = pair<int, int>(-1, -1); // undefined
			q.insert(curr);
		}
	}
	
	dist[pair<int, int>(startRowIdx, startColIdx)] = 0;

	while (!q.empty()) {
		unsigned min_dist = -1;
		pair<int, int> u;

		for (pair<int, int> v : q) {
			if (min_dist > dist[v]) {
				cout << dist[v] << endl;
				min_dist = dist[v];
				u = v;
			}
		}

		q.erase(u);
		cout << u.first << ' ' << u.second << ' ' << min_dist << endl;
		int rowIdx = u.first;
		int colIdx = u.second;

		vector<pair<int, int>> neighbors = this->getPixelNeighbors(rowIdx, colIdx, img);

		for (pair<int, int> v : neighbors) {
			unsigned alt = dist[u];
			if (dist[u] != -1) alt++;

			if (alt < dist[v]) {
				dist[v] = alt;
				prev[v] = u;
			}
		}
	
	}

	pair<int, int> u(img.get_height() - 2, img.get_width() - 1); // target
	if (prev[u] != pair<int, int>(-1, -1) || u == pair<int, int>(startRowIdx, startColIdx)) {
		while (prev[u] != pair<int, int>(-1, -1)) {
			int rowIdx = u.first;
			int colIdx = u.second;

			img[rowIdx][colIdx] = rgb_pixel(255, 0, 0);
			u = prev[u];
		}
	}
}

void Maze::solveMaze() {
	assert(this->M != NULL);
	
	image<rgb_pixel> solvedImage("unsolvedMaze.png");
	bool ** visited = getVisitedMatrix();
	(void)solveMazeHelper(1, 0, solvedImage, visited);

	for (int rowIdx = 0; rowIdx < (2*this->length + 1); rowIdx++) {
		delete [] visited[rowIdx];
	}

	delete [] visited;

	solvedImage.write("solvedMaze.png");
}

// Helper functions

void Maze::createEmptyMaze() {
	if (this->M != NULL)
		this->clearMaze();

	this->M = new Cell ** [this->length];
	for (int rowIdx = 0; rowIdx < this->length; rowIdx++) {
		this->M[rowIdx] = new Cell * [this->length];
		for (int colIdx = 0; colIdx < this->length; colIdx++) {
			this->M[rowIdx][colIdx] = new Cell(rowIdx, colIdx);
		}
	}
}

void Maze::clearMaze() {
	if (this->M == NULL)
		return;

	for (int rowIdx = 0; rowIdx < this->length; rowIdx++) {
		for (int colIdx = 0; colIdx < this->length; colIdx++) {
			delete this->M[rowIdx][colIdx];
		}
		delete [] this->M[rowIdx];
	}

	delete [] this->M;
	this->M = NULL;
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
