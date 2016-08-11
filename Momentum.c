#include <stdio.h>
#include <stdlib.h>
#include "Momentum.h"

/* This file provides functions to handle momentum and gravity calculations */

int getVelocity(int objectVel, int objectMass, int collisionVel, int collisionMass){

	if (collisionMass == 0){ // 0 mass would be a wall
		return objectVel *= -1; // reverse velocity direction
	}

	return objectVel;

    //TODO: elastic collisions later

}
