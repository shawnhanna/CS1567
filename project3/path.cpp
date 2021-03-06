#include "path.h"
#include "map.h"

float NS_PENALTY[MAP_WIDTH][MAP_HEIGHT] = {
	// col 1 (starting from left of room)
	{0.60,
	 0.55,
	 0.50,
	 0.50,
	 0.50},
	// col 2
	{0.35,
	 0.30,
	 0.25,
	 0.25,
	 0.25},
	// col 3
	{0.10,
	 0.05,
	 0.01,
	 0.00,
	 0.00},
	// col 4
	{0.10,
	 0.05,
	 0.01,
	 0.00,
	 0.00},
	// col 5
	{0.10,
	 0.05,
	 0.01,
	 0.00,
	 0.00},
	// col 6
	{0.15,
	 0.10,
	 0.01,
	 0.00,
	 0.00},
	// col 7
	{0.20,
	 0.15,
	 0.01,
	 0.00,
	 0.00}
};

Path::Path() 
: _cells() {	
}

Path::Path(Cell *cell) 
: _cells() {
	push(cell);
}

Path::Path(Path *path) 
: _cells() {
	for (int i = 0; i < path->length(); i++) {
		push(path->getCell(i));
	}
}

Path::~Path() {}

void Path::push(Cell *cell) {
	_cells.push_back(cell);
}

void Path::pop() {
	_cells.pop_back();
}

int Path::length() {
	return _cells.size();
}

int Path::getHeading(int upTo) {
	int heading = -1;

	if (upTo > 1) {
		Cell *curCell = getCell(upTo-1);
		Cell *prevCell = getCell(upTo-2);

		if (curCell->x != prevCell->x) {
			if (curCell->x > prevCell->x) {
				heading = DIR_WEST;
			}
			else {
				heading = DIR_EAST;
			}
		}
		else if (curCell->y != prevCell->y) {
			if (curCell->y > prevCell->y) {
				heading = DIR_NORTH;
			}
			else {
				heading = DIR_SOUTH;
			}
		}
	}

	return heading;
}

/** 
 * The value of a path is calculated as follows:
 * For each cell in the path, a NS penalty is applied, 
 * which knocks the points for that cell down by a 
 * percentage. Then, the amount of times the robot
 * must change directions is summed and subtracted
 * from the final path value.
**/
float Path::getValue() {
	float value = 0.0;

	bool used[MAP_WIDTH][MAP_HEIGHT] = {
		{false, false, false, false, false},
		{false, false, false, false, false},
		{false, false, false, false, false},
		{false, false, false, false, false},
		{false, false, false, false, false},
		{false, false, false, false, false},
		{false, false, false, false, false}
	};

	int opponentX=-1, opponentY=-1;

	LOG.write(LOG_LOW, "path", "\n\nPATH:\n");
	int directionChanges = 0;
	for (int i = 0; i < length(); i++) {
		Cell *nextCell = getCell(i);

		int nextX = nextCell->x;
		int nextY = nextCell->y;

		LOG.write(LOG_LOW, "path", "%d, %d", nextX,nextY);

		if (!used[nextX][nextY]) {
			int points = nextCell->getPoints();
			value += (float)points * (1-NS_PENALTY[nextX][nextY]);
			used[nextX][nextY] = true;
		}

		if (i > 1) {
			Cell *curCell = getCell(i-1);
			int curX = curCell->x;
			int curY = curCell->y;

			int heading = getHeading(i);
			switch (heading) {
			case DIR_NORTH:
				if (nextY < curY || nextX != curX) {
					directionChanges++;
				}
				break;
			case DIR_SOUTH:
				if (nextY > curY || nextX != curX) {
					directionChanges++;
				}
				break;
			case DIR_EAST:
				if (nextX > curX || nextY != curY) {
					directionChanges++;
				}
				break;
			case DIR_WEST:
				if (nextX < curX || nextY != curY) {
					directionChanges++;
				}
				break;
			}
			//apply penalty based on the location of the other robot
			// to the cell iff they are fairly close
			/*
			int diff = abs(curX-)+abs(curY-opponentY);
			if(diff != 0 && diff <= 3)
				value -= (1-(1/(2*diff)))*nextCell->getPoints();
			*/
		}
	}

	value -= (float)directionChanges;

	LOG.printfScreen(LOG_LOW, "path", "Path Value = %f\n", value);
	return value;
}

Cell* Path::getCell(int i) {
	if (length() > i) {
		return _cells[i];
	}
	return NULL;
}

Cell* Path::getFirstCell() {
	return getCell(1);
}

Cell* Path::getLastCell() {
	return getCell(length()-1);
}
