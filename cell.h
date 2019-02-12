#ifndef CELL_H
#define CELL_H

class Cell {

	public:
		Cell();

		void markCellAsVisited();
		void unmarkCell();

		bool getEastWall();
		bool getSouthWall();

		void setEastWall(bool b);
		void setSouthWall(bool b);

	private:
		bool eastWall;
		bool southWall;
		bool visited;
};

#endif
