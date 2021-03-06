#include "maze.h"

#define WHITE rgb_pixel(255, 255, 255)
#define BLACK rgb_pixel(0, 0, 0)
#define RED rgb_pixel(255, 0, 0)

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
		img[2*rowIdx][2*colIdx + 1] = WHITE;
		currentCell->setSouthWall(false);
	} else if (dir == E) {
		img[2*rowIdx + 1][2*colIdx + 2] = WHITE;
		previousCell->setEastWall(false);
	} else if (dir == S) {
		img[2*rowIdx + 2][2*colIdx + 1] = WHITE;
		previousCell->setSouthWall(false);
	} else {
		img[2*rowIdx + 1][2*colIdx] = WHITE;
		currentCell->setEastWall(false);
	}
}

void clearPNG(image<rgb_pixel> & img) {
	for (size_t rowIdx = 0; rowIdx < img.get_height(); rowIdx++) {
		for (size_t colIdx = 0; colIdx < img.get_width(); colIdx++) {
			if (rowIdx % 2 == 0 || colIdx % 2 == 0)
				img[rowIdx][colIdx] = BLACK;
			else
				img[rowIdx][colIdx] = WHITE;
		}
	}

	img[1][0] = WHITE;					// start
	img[img.get_height() - 2][img.get_width() - 1] = WHITE;	// end
}

void Maze::generateMaze() {
	this->createEmptyMaze();
	srand(time(0));
	
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
	return (pixel.red == 255) && (pixel.green == 255) && (pixel.blue == 255);
}

bool coordinateInsidePicture(unsigned rowIdx, unsigned colIdx, image<rgb_pixel> & img) {
	return (0 <= rowIdx) && (rowIdx < img.get_height()) && (0 <= colIdx) && (colIdx < img.get_width());
}

vector<pair<int, int>> Maze::getPixelNeighbors(pair<int, int> & curr, image<rgb_pixel> & img) {
	int rowIdx = curr.first;
	int colIdx = curr.second;
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

void Maze::solveMazeHelper(int startRowIdx, int startColIdx, image<rgb_pixel> & img) {
	stack<pair<pair<int, int>, pair<int, int>>> stk;
	unordered_set<pair<int, int>, pair_hash> s;
	unordered_map<pair<int, int>, pair<int, int>, pair_hash> edge_map;

	const pair<int, int> UNDEFINED(-1, -1);
	const pair<int, int> START(startRowIdx, startColIdx);
	const pair<int, int> END(img.get_height() - 2, img.get_width() - 1);

	stk.push({UNDEFINED, START});

	while (stk.top().second != END) {
		auto top = stk.top();
		stk.pop();
		pair<int, int> parent = top.first;
		pair<int, int> curr = top.second;

		if (s.find(curr) != s.end())
			continue;

		s.insert(curr);
		edge_map[curr] = parent;
		vector<pair<int, int>> neighbors = getPixelNeighbors(curr, img);

		for (auto neighbor : neighbors) {
			stk.push({curr, neighbor});
		}
	}

	edge_map[END] = stk.top().first;
	pair<int, int> curr = END;

	while (curr != UNDEFINED) {
		img[curr.first][curr.second] = RED;
		curr = edge_map[curr];
	}
}

void Maze::solveMaze() {
	assert(this->M != NULL);
	
	image<rgb_pixel> solvedImage("unsolvedMaze.png");
	solveMazeHelper(1, 0, solvedImage);

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
