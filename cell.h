#ifndef CELL_H
#define CELL_H

class Cell {

	public:
		Cell();
		Cell(int rowIdx, int colIdx);

		void markCellAsVisited();
		void unmarkCell();

		int getRowIdx();
		int getColIdx();
		bool getEastWall();
		bool getSouthWall();
		bool isVisited();

		void setEastWall(bool b);
		void setSouthWall(bool b);

	private:
		int rowIdx;
		int colIdx;

		bool eastWall;
		bool southWall;
		bool visited;
};

#endif
