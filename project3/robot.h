/**
 * robot.h
 * 
 * @brief 
 *      This class defines the rovio robot, and performs operations that the 
 *      system can do, such as movement and requesting sensor updates.  
 *      It also stores the sensor classes, vehicle position, and camera.
 * 
 * @author
 *      Shawn Hanna
 *      Tom Nason
 *      Joel Griffith
 * 
 **/

#ifndef CS1567_ROBOT_H
#define CS1567_ROBOT_H

#include "map.h"
#include "map_strategy.h"
#include "pose.h"
#include "camera.h"
#include "wheel_encoders.h"
#include "north_star.h"
#include "fir_filter.h"
#include "kalman_filter.h"
#include "PID.h"
#include "utilities.h"
#include "constants.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include <robot_if++.h>
#include <string>

#define GOOD_NS_STRENGTH 13222

#define MAX_CAMERA_BRIGHTNESS (0x7F)
#define CAMERA_FRAMERATE 5

#define NUM_SPEEDS 11

#define DIR_LEFT 0
#define DIR_RIGHT 1

#define CELL_SIZE 65

const float TIME_DISTANCE = 116.0; // cm

// average speed to move forward TIME_DISTANCE at integer robot speeds
const float SPEED_FORWARD[NUM_SPEEDS] = {
    0.0,
    TIME_DISTANCE/3.5,
    TIME_DISTANCE/3.5,
    TIME_DISTANCE/3.5,
    TIME_DISTANCE/3.7,
    TIME_DISTANCE/3.7,
    TIME_DISTANCE/3.9,
    TIME_DISTANCE/4.6,
    TIME_DISTANCE/4.6,
    TIME_DISTANCE/4.8,
    TIME_DISTANCE/4.9
};
/*
 * Old stuff
// average speed for left and right turns at integer robot speeds
const float SPEED_TURN[][NUM_SPEEDS] = {
    {0.0, 0.0},
    {(2*PI)/1.8, -(2*PI)/1.6},
    {(2*PI)/1.9, -(2*PI)/2.0},
    {(2*PI)/2.23, -(2*PI)/2.2},
    {(2*PI)/2.36, -(2*PI)/2.3},
    {(2*PI)/2.87, -(2*PI)/2.8},
    {(2*PI)/2.8, -(2*PI)/2.8},
    {(2*PI)/4.6, -(2*PI)/4.75},
    {(2*PI)/4.75, -(2*PI)/4.75},
    {(2*PI)/5.35, -(2*PI)/5.35},
    {(2*PI)/5.35, -(2*PI)/5.45}
};
*/


// average speed for left and right turns at integer robot speeds
const float SPEED_TURN[][NUM_SPEEDS] = {
    {0.0, 0.0},
    {(2*PI)/2.75, -(2*PI)/2.81},
    {(2*PI)/2.8, -(2*PI)/2.59},
    {(2*PI)/3.75, -(2*PI)/3.75},
    {(2*PI)/3.90, -(2*PI)/3.75},
    {(2*PI)/5.80, -(2*PI)/6.05},
    {(2*PI)/5.60, -(2*PI)/5.45},
    {(2*PI)/4.50, -(2*PI)/4.39},
    {(2*PI)/4.60, -(2*PI)/4.45},
    {(2*PI)/5.00, -(2*PI)/5.02},
    {(2*PI)/10.00, -(2*PI)/8.8}
};

class Robot {
public:
    Robot(std::string address, int id);
    ~Robot();
    void eatShit();
    void move(int direction, int numCells);
    void turn(int direction);
    void turn(int relDirection, float radians);
    void moveToCell(float x, float y);
    void moveTo(float x, float y);
    float moveToUntil(float x, float y, float thetaErrorLimit);
    void turnTo(float theta, float thetaErrorLimit);
    void turnCenter();
    void center();
    void setFailLimit(int limit);
    int getFailLimit();
    void setCameraResolution(int resolution, int quality);
    void prefillData();
    void updatePose(bool useWheelEncoders);
    void moveForward(int speed);
    void turnLeft(int speed);
    void turnRight(int speed);
    void strafeLeft(int speed);
    void strafeRight(int speed);
    void stop();
	void moveHead(int position);
    Pose* getPose();
    RobotInterface* getInterface();
    int getName();
    bool isThereABitchInMyWay();
	int getStrength();
    int getRoom();
    int getBattery();
    void printBeginPhrase();
    void printSuccessPhrase();
    void printFailPhrase();
    void rockOut();
	bool sideCenter(int direction);

    Camera *_camera;
private:
    bool _centerTurn(float centerError);
    bool _centerStrafe(float centerError);
    
    RobotInterface *_robotInterface;
    int _name;

	int _speed;	
	char _turnDirection;
	bool _movingForward;
    int _heading;

    PID* _movePID;
    PID* _turnPID;
    PID* _centerTurnPID;
    PID* _centerStrafePID;

    int _failLimit;

    Pose *_pose;

    //Camera *_camera;
    WheelEncoders *_wheelEncoders;
    NorthStar *_northStar;

    KalmanFilter *_kalmanFilter;

    Map *_map;
    MapStrategy *_mapStrategy;
    
    bool _updateInterface();
};

#endif
