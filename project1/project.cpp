#include "robot.h"
#include <iostream>
#include <time.h>
#include <sys/time.h>
#include <string>
#include <math.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>

#define NUMBASES 7

int zero_x;
int zero_y;
float zero_theta;
int starting_room_ID;

int main(int argc, char *argv[]) {
	if(argc<2){
		printf("ERROR: need argument for robot name\n");
		exit(-1);
	}
    //Base locations within the global coordinate system
	Pose * bases[NUMBASES];
	bases[0] = new Pose(0, 0, 0);
	bases[1] = new Pose(341, 0, 0);
	bases[2] = new Pose(240, 186, 0);
	bases[3] = new Pose(320, 186, 0);
	bases[4] = new Pose(402, 303, 0);
	bases[5] = new Pose(402, 353, 0);
	bases[6] = new Pose(69, 419, 0);

	Robot *robot = new Robot(argv[1], 0);

	for (int i = 0; i < 15; i++) {
		printf("prefilling data\n");
		robot->update();
	}

    for (int i = 0; i < NUMBASES; i++) {
        robot->moveTo(bases[i]->getX(),
                      bases[i]->getY());
    }

	delete(robot);
    // FIXME: Should this be delete[] ?
	for(int i = 0; i < NUMBASES; i++) {
		delete bases[i];
	}

	return 0;
}
