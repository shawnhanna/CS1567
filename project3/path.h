#ifndef CS1567_PATH_H
#define CS1567_PATH_H

#include "cell.h"
#include "logger.h"
#include <vector>

class Path {
public:
	Path();
	Path(Cell *cell);
	Path(Path *path);
	~Path();
	void push(Cell *cell);
	void pop();
	int length();
	int getHeading(int upTo);
	float getValue();
	Cell* getCell(int i);
	Cell* getFirstCell();
	Cell* getLastCell();
private:
	std::vector<Cell*> _cells;
};

#endif