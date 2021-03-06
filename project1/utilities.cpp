#include "utilities.h"
#include "constants.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

namespace Util {
    // Converts north star ticks to cm based off NS_TICKS constant
    float nsToCM(float ticks) {
        return ticks / NS_TICKS;
    }

    // Converts wheel encoder ticks to cm based off WE_TICKS constant
    float weToCM(float ticks) {
        return ticks / WE_TICKS;
    }

    // Converts cm to north star ticks based off NS_TICKS constant
    float cmToNS(float cm) {
        return cm * NS_TICKS;
    }

    // Converts cm to wheel encoder ticks based off WE_TICKS constant
    float cmToWE(float cm) {
        return cm * WE_TICKS;
    }

    // Performs a matrix multiplication of two matrices, A and B,
    // storing the result in a third matrix, C
    void mMult(float *mA, int lA, int hA, float *mB, int lB, int hB, float *mC) {
        if (hA != lB) {
            printf("Inner matrix dimensions do not match!\n");
            return;
        }

        for (int i = 0; i < lA; i++) {
            for (int j = 0; j < hB; j++) {
                mC[j + (i*hB)] = 0;
                for(int k = 0; k < lB; k++) {
                    mC[j+(i*hB)] += mA[(i*hB)+k] * mB[(k*hB)+j];
                }
            }
        }
    }

    // Takes an error, possibly negative, and converts it to
    // the appropriate representation used internally by robot
    float normalizeThetaError(float thetaError) {
        while (thetaError <= -PI) {
            thetaError += 2*PI;
        }
        while (thetaError >= PI) {
            thetaError -= 2*PI;
        }
        return thetaError;
    }

    /* Input: A number in range [-inf, inf] (usually [-pi, pi])
       Returns: A number in range [0, 2pi]
    */
    float normalizeTheta(float theta) {
        theta = fmod(theta, 2*PI);
        while (theta < 0) {
            theta += 2*PI;
        }
        return theta;
    }

    // Returns the name, which is an integer, of the robot
    // according to its address
    int nameFrom(std::string address) {
        for (int i = 0; i < NUM_ROBOTS; i++) {
            if (ROBOTS[i] == address) {
                return i;
            }
        }
        for (int i = 0; i < NUM_ROBOTS; i++) {
            if (ROBOT_ADDRESSES[i] == address) {
                return i;
            }
        }
    }
};
