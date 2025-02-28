#ifdef __has_include
#include "math.h"
#if __has_include("Astrobee.h")
#include "Astrobee.h"
#endif
#endif
// Global array to keep track of dropped off debris
bool droppedOffDebris[15];

// Forward declaration of getDebrisLocations

int abs(int value) {
    if (value < 0) {
        return -value;
    }
    return value;
}

float fabs(float value) {
    if (value < 0.0f) {
        return -value;
    }
    return value;
}

float (*Midpoint(float first[2], float second[2]))[2]
{
    float arr[2] = {(first[0] + second[0]) / 2, (first[1] + second[1]) / 2};
    return &arr;
}

int GetDebrisWidth(unsigned int id)
{
    if (id >= 0 && id <= 2)
    {
        return 15;
    }
    else if (id >= 3 && id <= 13)
    {
        return 5;
    }
    else if (id == 14)
    {
        return 5;
    }
    else
    {
        return 0;
    }
}

bool checkLineCollisions(float linePos1[2], float linePos2[2], int skipDebris = -1)
{
    int m = (linePos1[1] - linePos2[1]) / (linePos1[0] - linePos2[0]);
    int b = linePos1[1] - m * linePos1[0];

    // Use the line equation to check for collisions in each Debris.
    for (int i = 0; i < 15; i++)
    {
        if (i == skipDebris) continue;
        float position[2] = {game.GetObjectLocX(i), game.GetObjectLocY(i)};
        unsigned int width = GetDebrisWidth(i);

        int leftBound = position[0] - width / 2;
        int rightBound = position[0] + width / 2;
        int bottomBound = position[1] - width / 2;
        int topBound = position[1] + width / 2;

        int y_left = m * leftBound + b;
        int y_right = m * rightBound + b;
        if ((bottomBound <= y_left && y_left <= topBound) || (bottomBound <= y_right && y_right <= topBound))
        {
            return true;
        }

        int x_bottom = (bottomBound - b) / m;
        int x_top = (topBound - b) / m;
        if ((leftBound <= x_bottom && x_bottom <= rightBound) || (leftBound <= x_top && x_top <= rightBound))
        {
            return true;
        }
    }
}

bool hasCollisions(int debrisId, float x1, float y1, float x2, float y2)
{
    float width = GetDebrisWidth(debrisId);
    if (x1 == x2) {
        x2 += 0.0001;
    }
    float linePos1[2] = {x1, y1};
    float linePos2[2] = {x2, y2};
    float m = (linePos1[1] - linePos2[1]) / (linePos1[0] - linePos2[0]);

    float (*centerPtr)[2] = Midpoint(linePos1, linePos2);
    float center[2] = {(*centerPtr)[0], (*centerPtr)[1]};

    if (m >= 0)
    {
        // Check the lines with slope m passing through upperLeft and lowerRight
        float upperLeft[2] = {center[0] - width, center[1] + width};
        float lowerRight[2] = {center[0] + width, center[1] - width};
        float firstCheck[2] = {upperLeft[0] + 1, upperLeft[1] + m};
        float secondCheck[2] = {lowerRight[0] + 1, lowerRight[1] + m};
        return checkLineCollisions(upperLeft, firstCheck, debrisId) || checkLineCollisions(lowerRight, secondCheck, debrisId);
    }
    else
    {
        // Check the lines with slope m passing through lowerLeft and upperRight
        float lowerLeft[2] = {center[0] - width, center[1] - width};
        float upperRight[2] = {center[0] + width, center[1] + width};
        float firstCheck[2] = {lowerLeft[0] + 1, lowerLeft[1] + m};
        float secondCheck[2] = {upperRight[0] + 1, upperRight[1] + m};
        return checkLineCollisions(lowerLeft, firstCheck, debrisId) || checkLineCollisions(upperRight, secondCheck, debrisId);
    }
}

float* getCurrentLocation() {
    static float resultArr[2] = {game.GetRobotPositionX(), game.GetRobotPositionY()}; // array to hold coords of each obj, index corresponds to obj, x and y

    return resultArr; // Return the pointer to the 2D array
}

float (*getDebrisLocations())[2] {
    static float resultArr[15][2] = {{0.0f}}; // array to hold coords of each obj, index corresponds to obj, x and y
    
    for (int debris = 0; debris < 15; ++debris) { // Outer loop for rows
        for (int coord = 0; coord < 2; ++coord) { // Inner loop for columns
            if (coord == 0) {
                resultArr[debris][coord] = static_cast<float>(game.GetObjectLocX(debris)); // Assign x-coordinate
            } else {
                resultArr[debris][coord] = static_cast<float>(game.GetObjectLocY(debris)); // Assign y-coordinate
            }
        }
    }
    return resultArr; // Return the pointer to the 2D array
}

float getDistance(float* coord1, float* coord2){
    // coord1 = [x1, y1]  coord2 = [x2, y2]
    float x1 = coord1[0];
    float y1 = coord1[1];
    float x2 = coord2[0];
    float y2 = coord2[1];
    return sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
}

int getClosestWall() { // 0 for top wall, 1 for right wall, 2 for bottom wall, 3 for left wall
    float currentLocation[2] = {0.0f}; // [xcoord, ycoord]
    currentLocation[0] = game.GetRobotPositionX();
    currentLocation[1] = game.GetRobotPositionY(); // get the current location of the robot
    
    float distances[4] = {0.0f};
    // boundaries of the playing field are 1 by 1 meter, origin is at the center (0,0)
    // the walls are the borders of the imaging zone, and the imaging zone is centerd at the origin and is 0.7 by 0.7 meters
    // to get dist from robot to wall, draw perpendiculat line fro robot to a wall and record distance in array (distances)

    distances[0] = 0.35 - currentLocation[1]; // distance to top wall
    distances[1] = 0.35 - currentLocation[0]; // distance to right wall
    distances[2] = 0.35 + currentLocation[1]; // distance to bottom wall
    distances[3] = 0.35 + currentLocation[0]; // distance to left wall
    
    float minDistance = 30;
    int minDistanceWall = 0;
    
    for (int wallIndex = 0; wallIndex < 4; ++wallIndex) {             // Outer loop for rows
        if (distances[wallIndex] < minDistance) { // if the distance is less than the current min distance
            minDistance = distances[wallIndex]; // update the min distance
            minDistanceWall = wallIndex;   // update the index of the closest wall
        }
    }
    
    return minDistanceWall; // reutnr the index of the closest wall
}

float getDistanceToWall(int wall, float x, float y) { // get the distance to the closest wall using the getClosestWall func

    
    float distances[4] = {0.0f};

    distances[0] = 0.35 - y; // distance to top wall
    distances[1] = 0.35 - x; // distance to right wall
    distances[2] = 0.35 + y; // distance to bottom wall
    distances[3] = 0.35 + x; // distance to left wall

    
    return distances[wall]; // return the distance to the wall
}

float getDistanceToClosestWall() { // get the distance to the closest wall using the getClosestWall func

    int closestWall = getClosestWall(); // get the index of the closest wall
    float currentLocation[2] = {0.0f}; // [xcoord, ycoord]
    currentLocation[0] = game.GetRobotPositionX();
    currentLocation[1] = game.GetRobotPositionY(); // get the current location of the robot
    
    float distances[4] = {0.0f};
    // boundaries of the playing field are 1 by 1 meter, origin is at the center (0,0)
    // the walls are the borders of the imaging zone, and the imaging zone is centerd at the origin and is 0.7 by 0.7 meters
    // to get dist from robot to wall, draw perpendiculat line fro robot to a wall and record distance in array (distances)

    distances[0] = 0.35 - currentLocation[1]; // distance to top wall
    distances[1] = 0.35 - currentLocation[0]; // distance to right wall
    distances[2] = 0.35 + currentLocation[1]; // distance to bottom wall
    distances[3] = 0.35 + currentLocation[0]; // distance to left wall
    
    return distances[closestWall]; // return the distance to the closest wall
}

int getClosestDebris() {
    float currentLocation[2] = {0.0f}; // [xcoord, ycoord]
    currentLocation[0] = game.GetRobotPositionX();
    currentLocation[1] = game.GetRobotPositionY(); // get the current location of the robot
    DEBUG(("curreloc %f %f", currentLocation[0], currentLocation[1]));

    float (*debrisArray)[2] = getDebrisLocations(); // Pointer to the 2D array of debris locations
    if (debrisArray == NULL) { // Use NULL instead of nullptr
        printf("Error: debrisArray is null\n");
        return -1; // Return an error code if debrisArray is null
    }

    float minDistance = 30.0f;
    int minDistanceDebris = -1;

    for (int debrisIndex = 0; debrisIndex < 14; ++debrisIndex) {
        if (droppedOffDebris[debrisIndex]) {
            continue; // Skip debris that have been dropped off
        }
        float distance = getDistance(currentLocation, debrisArray[debrisIndex]);
        DEBUG(("Debris %d: Distance = %f\n", debrisIndex, distance)); // Debug print

        if (distance < minDistance) { // If the distance is less than the current min distance
            minDistance = distance; // Update the min distance
            minDistanceDebris = debrisIndex; // Update the index of the closest debris
        }
    }

    if (minDistanceDebris == -1) {
        DEBUG(("Error: No debris found within the minimum distance\n"));
    } else {
        DEBUG(("Closest debris index: %d, Distance: %f\n", minDistanceDebris, minDistance)); // Debug print
    }

    return minDistanceDebris; // Return the index of the closest debris
}

int getSecondClosestDebris() {
    float currentLocation[2] = {0.0f}; // [xcoord, ycoord]
    currentLocation[0] = game.GetRobotPositionX();
    currentLocation[1] = game.GetRobotPositionY(); // get the current location of the robot

    float (*debrisArray)[2] = getDebrisLocations(); // Pointer to the 2D array of debris locations
    if (debrisArray == NULL) { // Use NULL instead of nullptr
        printf("Error: debrisArray is null\n");
        return -1; // Return an error code if debrisArray is null
    }

    float minDistance = 30.0f;
    float secondMinDistance = 30.0f;
    int minDistanceDebris = -1;
    int secondMinDistanceDebris = -1;

    for (int debrisIndex = 3; debrisIndex < 14; ++debrisIndex) {
        if (droppedOffDebris[debrisIndex]) {
            continue; // Skip debris that have been dropped off
        }
        float distance = getDistance(currentLocation, debrisArray[debrisIndex]);
        printf("Debris %d: Distance = %f\n", debrisIndex, distance); // Debug print

        if (distance < minDistance) { // If the distance is less than the current min distance
            secondMinDistance = minDistance; // Update the second min distance
            secondMinDistanceDebris = minDistanceDebris; // Update the index of the second closest debris

            minDistance = distance; // Update the min distance
            minDistanceDebris = debrisIndex; // Update the index of the closest debris
        } else if (distance < secondMinDistance) { // If the distance is less than the second min distance
            secondMinDistance = distance; // Update the second min distance
            secondMinDistanceDebris = debrisIndex; // Update the index of the second closest debris
        }
    }

    if (secondMinDistanceDebris == -1) {
        printf("Error: No second closest debris found within the minimum distance\n");
    } else {
        printf("Second closest debris index: %d, Distance: %f\n", secondMinDistanceDebris, secondMinDistance); // Debug print
    }

    return secondMinDistanceDebris; // Return the index of the second closest debris
}

int getClosestPrioritizedDebris() { // gets the closest debris out of 0, 1, 2 and 14
    if (!droppedOffDebris[14] && !isOutsideImagingZone(14)) {
        return 14; // Prioritize debris 14 first if it hasn't been dropped off and is inside the imaging zone
    }

    float currentLocation[2] = {0.0f}; // [xcoord, ycoord]
    currentLocation[0] = game.GetRobotPositionX();
    currentLocation[1] = game.GetRobotPositionY(); // get the current location of the robot
    DEBUG(("curreloc %f %f", currentLocation[0], currentLocation[1]));

    float (*debrisArray)[2] = getDebrisLocations(); // Pointer to the 2D array of debris locations
    if (debrisArray == NULL) { // Use NULL instead of nullptr
        printf("Error: debrisArray is null\n");
        return -1; // Return an error code if debrisArray is null
    }

    float minDistance = 30.0f;
    int minDistanceDebris = -1;

    for (int debrisIndex = 0; debrisIndex < 3; ++debrisIndex) {
        if (droppedOffDebris[debrisIndex] || isOutsideImagingZone(debrisIndex)) {
            continue; // Skip debris that have been dropped off or are outside the imaging zone
        }
        float distance = getDistance(currentLocation, debrisArray[debrisIndex]);
        DEBUG(("Debris %d: Distance = %f\n", debrisIndex, distance)); // Debug print

        if (distance < minDistance) { // If the distance is less than the current min distance
            minDistance = distance; // Update the min distance
            minDistanceDebris = debrisIndex; // Update the index of the closest debris
        }
    }

    if (minDistanceDebris == -1) {
        DEBUG(("Error: No debris found within the minimum distance\n"));
    } else {
        DEBUG(("Closest debris index: %d, Distance: %f\n", minDistanceDebris, minDistance)); // Debug print
    }

    return minDistanceDebris; // Return the index of the closest debris
}


int (*getKeyDebris(int debrisIndex))[15] {
    static int keyDebris[4][15]; // array to hold key debris indices for each direction

    // Initialize the keyDebris array with -1
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 15; ++j) {
            keyDebris[i][j] = -1;
        }
    }

    float (*debrisArray)[2] = getDebrisLocations();
    float x1 = debrisArray[debrisIndex][0];
    float y1 = debrisArray[debrisIndex][1];
    float offset = (debrisIndex <= 2) ? 0.075f : 0.025f; // half the width of the debris

    int northIndex = 0, eastIndex = 0, southIndex = 0, westIndex = 0;

    for (int i = 0; i < 15; ++i) {
        if (i == debrisIndex) continue;

        float x2 = debrisArray[i][0];
        float y2 = debrisArray[i][1];
        float otherOffset = (i <= 2) ? 0.075f : 0.025f;

        // North zone
        if (x2 + otherOffset >= x1 - offset && x2 - otherOffset <= x1 + offset && y2 - otherOffset > y1 + offset) {
            keyDebris[0][northIndex++] = i;
        }
        // East zone
        if (y2 + otherOffset >= y1 - offset && y2 - otherOffset <= y1 + offset && x2 - otherOffset > x1 + offset) {
            keyDebris[1][eastIndex++] = i;
        }
        // South zone
        if (x2 + otherOffset >= x1 - offset && x2 - otherOffset <= x1 + offset && y2 + otherOffset < y1 - offset) {
            keyDebris[2][southIndex++] = i;
        }
        // West zone
        if (y2 + otherOffset >= y1 - offset && y2 - otherOffset <= y1 + offset && x2 + otherOffset < x1 - offset) {
            keyDebris[3][westIndex++] = i;
        }
    }

    return keyDebris;
}

bool isOutsideImagingZone(int debrisIndex) {

    float (*debrisArray)[2] = getDebrisLocations();
    float x = debrisArray[debrisIndex][0];
    float y = debrisArray[debrisIndex][1];

    return (fabs(x) > 0.35 || fabs(y) > 0.35);

}

void removeDebris(int debrisIndex, int direction) {
//get the current location of the robot

    // get the coordinates of the debris we want to remove
    float debrisLocation[2] = {game.GetObjectLocX(debrisIndex), game.GetObjectLocY(debrisIndex)};    

    float* destinationPoint = new float[2]; // [xcoord, ycoord]

    if (direction == 0) {
        destinationPoint[0] = debrisLocation[0];
        destinationPoint[1] = 0.36;
    } else if (direction == 1) {
        destinationPoint[0] = 0.36;
        destinationPoint[1] = debrisLocation[1];
    } else if (direction == 2) {
        destinationPoint[0] = debrisLocation[0];
        destinationPoint[1] = -0.36;
    } else if (direction == 3) {
        destinationPoint[0] = -0.36;
        destinationPoint[1] = debrisLocation[1];
    }
    DEBUG(("attempted to move to %f %f", destinationPoint[0], destinationPoint[1]));
    game.MoveTo(destinationPoint[0], destinationPoint[1]); // move astrobee to the destination point

}

void removeDebrisFar(int debrisIndex, int direction) {
//get the current location of the robot

    // get the coordinates of the debris we want to remove
    float debrisLocation[2] = {game.GetObjectLocX(debrisIndex), game.GetObjectLocY(debrisIndex)};    

    float* destinationPoint = new float[2]; // [xcoord, ycoord]

    if (direction == 0) {
        destinationPoint[0] = debrisLocation[0];
        destinationPoint[1] = 0.49;
    } else if (direction == 1) {
        destinationPoint[0] = 0.49;
        destinationPoint[1] = debrisLocation[1];
    } else if (direction == 2) {
        destinationPoint[0] = debrisLocation[0];
        destinationPoint[1] = -0.49;
    } else if (direction == 3) {
        destinationPoint[0] = -0.49;
        destinationPoint[1] = debrisLocation[1];
    }
    DEBUG(("attempted to move to %f %f", destinationPoint[0], destinationPoint[1]));
    game.MoveTo(destinationPoint[0], destinationPoint[1]); // move astrobee to the destination point

}

float* getDebrisDestinationCoords(int debrisIndex, int direction) {
    // get the coordinates of the debris we want to remove
    float debrisLocation[2] = {game.GetObjectLocX(debrisIndex), game.GetObjectLocY(debrisIndex)};    

    static float destinationPoint[2]; // [xcoord, ycoord]

    if (direction == 0) {
        destinationPoint[0] = debrisLocation[0];
        destinationPoint[1] = 0.36;
    } else if (direction == 1) {
        destinationPoint[0] = 0.36;
        destinationPoint[1] = debrisLocation[1];
    } else if (direction == 2) {
        destinationPoint[0] = debrisLocation[0];
        destinationPoint[1] = -0.36;
    } else if (direction == 3) {
        destinationPoint[0] = -0.36;
        destinationPoint[1] = debrisLocation[1];
    }

    return destinationPoint; // return the destination point coordinates
}

float* getDebrisDestinationCoordsWithOffset(int debrisIndex, int direction, float xoffset, float yOffset) {
    float (*debrisArray)[2] = getDebrisLocations();

    // get the coordinates of the debris we want to remove
    float debrisLocation[2] = {debrisArray[debrisIndex][0], debrisArray[debrisIndex][1]};    

    static float destinationPoint[2]; // [xcoord, ycoord]

    if (direction == 0) {
        destinationPoint[0] = debrisLocation[0];
        destinationPoint[1] = 0.36;
    } else if (direction == 1) {
        destinationPoint[0] = 0.36;
        destinationPoint[1] = debrisLocation[1];
    } else if (direction == 2) {
        destinationPoint[0] = debrisLocation[0];
        destinationPoint[1] = -0.36;
    } else if (direction == 3) {
        destinationPoint[0] = -0.36;
        destinationPoint[1] = debrisLocation[1];
    }

    destinationPoint[0] += xoffset;
    destinationPoint[1] += yOffset;

    return destinationPoint; // return the destination point coordinates with offset
}

void removeDebrisWithOffset(int debrisIndex, int direction, float xoffset, float yOffset) {
    float (*debrisArray)[2] = getDebrisLocations();
//get the current location of the robot

    // get the coordinates of the debris we want to remove
    float debrisLocation[2] = {debrisArray[debrisIndex][0], debrisArray[debrisIndex][1]};    

    float* destinationPoint = new float[2]; // [xcoord, ycoord]

    if (direction == 0) {
        destinationPoint[0] = debrisLocation[0];
        destinationPoint[1] = 0.36;
    } else if (direction == 1) {
        destinationPoint[0] = 0.36;
        destinationPoint[1] = debrisLocation[1];
    } else if (direction == 2) {
        destinationPoint[0] = debrisLocation[0];
        destinationPoint[1] = -0.36;
    } else if (direction == 3) {
        destinationPoint[0] = -0.36;
        destinationPoint[1] = debrisLocation[1];
    }

    DEBUG(("attempted to move to %f %f", destinationPoint[0] + xoffset, destinationPoint[1] + yOffset));
    game.MoveTo(destinationPoint[0] + xoffset, destinationPoint[1] + yOffset); // move astrobee to the destination point
}

void removeDebrisWithOffsetFar(int debrisIndex, int direction, float xoffset, float yOffset) {
    float (*debrisArray)[2] = getDebrisLocations();
//get the current location of the robot

    // get the coordinates of the debris we want to remove
    float debrisLocation[2] = {debrisArray[debrisIndex][0], debrisArray[debrisIndex][1]};    

    float* destinationPoint = new float[2]; // [xcoord, ycoord]

    if (direction == 0) {
        destinationPoint[0] = debrisLocation[0];
        destinationPoint[1] = 0.49;
    } else if (direction == 1) {
        destinationPoint[0] = 0.49;
        destinationPoint[1] = debrisLocation[1];
    } else if (direction == 2) {
        destinationPoint[0] = debrisLocation[0];
        destinationPoint[1] = -0.49;
    } else if (direction == 3) {
        destinationPoint[0] = -0.49;
        destinationPoint[1] = debrisLocation[1];
    }

    DEBUG(("attempted to move to %f %f", destinationPoint[0] + xoffset, destinationPoint[1] + yOffset));
    game.MoveTo(destinationPoint[0] + xoffset, destinationPoint[1] + yOffset); // move astrobee to the destination point
}

void removeKeyDebris(int debrisIndex) {
    // this function will remove the key debris in the most optimal direction of a debris, then it will fly back to the debris 
    // get the direction with least key debris, if there is a direction with no key debris, remove the debris directly
    // if there is a tie between directions, take the direction that is closest to the wall
    // debrisindex is the debris which we ant to remove all key debris from

    int (*keyDebris)[15] = getKeyDebris(debrisIndex);
    int minKeyDebrisCount = 15;
    int minKeyDebrisDirection = -1;
    bool directionTie = false;
    int tieKeyDebrisDirection = -1;
    

    // get location of closest prioritized debris
    float* debrisLocation = getDebrisLocations()[debrisIndex];
    

    // find the direction(s) with the least key debris
    int numberOfKeyDebris[4];

    for (int direction = 0; direction < 4; ++direction) {
        int keyDebrisCount = 0;
        for (int i = 0; i < 15 && keyDebris[direction][i] != -1; ++i) {
            keyDebrisCount++;
        } 
        numberOfKeyDebris[direction] = keyDebrisCount;
        if (keyDebrisCount < minKeyDebrisCount) {
            minKeyDebrisCount = keyDebrisCount;
            minKeyDebrisDirection = direction;
        } else if (keyDebrisCount == minKeyDebrisCount) 
            directionTie = true; // there's at least one tie
            tieKeyDebrisDirection = direction; 
            
    }
    int closestWallDistance = getDistanceToWall(minKeyDebrisDirection, debrisLocation[0], debrisLocation[1]);
    // first let's determine which direction to remvoe key debris from
    // if no tie, the direction is already stored in minKeyDebrisDirection
    if (directionTie) {
        // if there is a tie between directions, take the direction that is closest to the wall
        
        for (int direction = 0; direction < 4; ++direction)
        { // iterate through and get all the directions that are tied, skip over the ones that have more key debris
            if (numberOfKeyDebris[direction] != minKeyDebrisCount) {
                // skip if this direction has more key debris than the minimum
                continue;
            }
            float distanceToWall = getDistanceToWall(direction, debrisLocation[0], debrisLocation[1]);
            DEBUG(("DISTNAFETOWALL %f", distanceToWall));
            if (distanceToWall < closestWallDistance) {
                closestWallDistance = distanceToWall;
                minKeyDebrisDirection = direction;
            }
        }
    }
    // let's handle the cases where there are 0-2 kley debris
    // i do not think there will be any case where the minimum number of key debris will be 3 or more

    if (minKeyDebrisCount == 0) {
        // move the debris directly out since there are no key debris
        DEBUG(("%f", game.GetBattery()));
        game.MoveTo(debrisLocation[0], debrisLocation[1]);
        DEBUG(("%f", game.GetBattery()));
        game.GrabObject(debrisIndex);
        removeDebris(debrisIndex, minKeyDebrisDirection);
        DEBUG(("%f", game.GetBattery()));
        game.DropObject();
        DEBUG(("%f", game.GetBattery()));
        // end
    }

    if (minKeyDebrisCount == 1) { // if ther is one key debris, take it and move it out
        int keyDebrisIndex = keyDebris[minKeyDebrisDirection][0];
        float* keyDebrisLocation = getDebrisLocations()[keyDebrisIndex];
        DEBUG(("%f", game.GetBattery()));
        game.MoveTo(keyDebrisLocation[0], keyDebrisLocation[1]);
        game.GrabObject(keyDebrisIndex);

        // when we rm key debris, it need to be offset from the path that our actual debris will take, now we calc that offset
        float offset = (keyDebrisIndex <= 2) ? 0.075f : 0.025f;
        offset += (debrisIndex <= 2) ? 0.075f : 0.025f;
        // y offset will be 0 if we are moving in the north or south direction, and x offset will be 0 if we are moving in the east or west direction
        float yOffset = (minKeyDebrisDirection == 0 || minKeyDebrisDirection == 2) ? 0.0f : offset; 
        float xOffset = (minKeyDebrisDirection == 1 || minKeyDebrisDirection == 3) ? 0.0f : offset;

        removeDebrisFar(keyDebrisIndex, minKeyDebrisDirection); 
        
        // this moves the astrobee to the loc outside the imaging zone that is calculated with the removebdeirs wih offset fiunction
        game.DropObject();

        // move the debris directly out since there are no key debris
        DEBUG(("%f", game.GetBattery()));
        game.MoveTo(debrisLocation[0], debrisLocation[1]);
        game.GrabObject(debrisIndex);
        DEBUG(("MOVEOD off debris %d\n", debrisIndex));
        DEBUG(("%f", game.GetBattery()));
        removeDebris(debrisIndex, minKeyDebrisDirection);
        
        game.DropObject();
        DEBUG(("DSROPPPEDOWUERY off debris %d\n", debrisIndex));
        // end
    }    

    if (minKeyDebrisCount == 2) { // if ther is one key debris, take it and move it out
        int firstKeyDebrisIndex = keyDebris[minKeyDebrisDirection][0];
        float* firstKeyDebrisLocation = getDebrisLocations()[firstKeyDebrisIndex];
        int secondKeyDebrisIndex = keyDebris[minKeyDebrisDirection][1];
        float* secondKeyDebrisLocation = getDebrisLocations()[secondKeyDebrisIndex];
        DEBUG(("%f", game.GetBattery()));
        game.MoveTo(firstKeyDebrisLocation[0], firstKeyDebrisLocation[1]);
        DEBUG(("%f", game.GetBattery()));
        game.GrabObject(firstKeyDebrisIndex);

        // when we rm key debris, it need to be offset from the path that our actual debris will take, now we calc that offset
        float offset = (firstKeyDebrisIndex <= 2) ? 0.05f : 0.0f;
        //offset += (debrisIndex <= 2) ? 0.075f : 0.025f;
        // y offset will be 0 if we are moving in the north or south direction, and x offset will be 0 if we are moving in the east or west direction
        float yOffset = (minKeyDebrisDirection == 0 || minKeyDebrisDirection == 2) ? 0.0f : offset; 
        float xOffset = (minKeyDebrisDirection == 1 || minKeyDebrisDirection == 3) ? 0.0f : offset;
        DEBUG(("%f", game.GetBattery()));
        removeDebrisWithOffsetFar(firstKeyDebrisIndex, minKeyDebrisDirection, xOffset, yOffset); // this moves the astrobee to the loc outside the imaging zone that is calculated with the removebdeirs wih offset fiunction
        DEBUG(("%f", game.GetBattery()));
        // move the debris directly out since there are no key debris
        game.MoveTo(secondKeyDebrisLocation[0], secondKeyDebrisLocation[1]);
        DEBUG(("%f", game.GetBattery()));
        game.GrabObject(secondKeyDebrisIndex);

        removeDebrisWithOffsetFar(secondKeyDebrisIndex, minKeyDebrisDirection, -xOffset, -yOffset); 

        game.MoveTo(debrisLocation[0], debrisLocation[1]);
        game.GrabObject(debrisIndex);
        removeDebris(debrisIndex, minKeyDebrisDirection);
        game.DropObject();
        // end
    }
    //rm key debris calcs the correct dir, removes key debris if neccesary, then removes our actuial debris and stays there
    DEBUG(("%f", game.GetBattery()));
    
}

//Declare any variables shared between functions here
void init(){
    //This function is called once when your code is first loaded.
    game.SetLayout(3);
    game.SetStartPosition(3);
    //IMPORTANT: make sure to set any variables that need an initial value.
    //Do not assume variables will be set to 0 automatically

    // Initialize the droppedOffDebris array
    for (int i = 0; i < 15; ++i) {
        droppedOffDebris[i] = false;
    }

    // Print getKeyDebris for all debris 
    for (int debrisIndex = 0; debrisIndex < 15; ++debrisIndex) {
        int (*keyDebris)[15] = getKeyDebris(debrisIndex);
        printf("Debris %d:\n", debrisIndex);
        printf("  North: ");
        for (int i = 0; i < 15 && keyDebris[0][i] != -1; ++i) {
            printf("%d ", keyDebris[0][i]);
        }
        printf("\n  East: ");
        for (int i = 0; i < 15 && keyDebris[1][i] != -1; ++i) {
            printf("%d ", keyDebris[1][i]);
        }
        printf("\n  South: ");
        for (int i = 0; i < 15 && keyDebris[2][i] != -1; ++i) {
            printf("%d ", keyDebris[2][i]);
        }
        printf("\n  West: ");
        for (int i = 0; i < 15 && keyDebris[3][i] != -1; ++i) {
            printf("%d ", keyDebris[3][i]);
        }
        printf("\n");
    }
}

void loop(){
    // while debris numbers 0, 1, 2, and 14 are still inside the imaging zone

    while (game.GetTime() <= 180 && game.GetBattery() >= 37.00) {
        // Add your logic here to handle the debris
        // For example, you can move the robot to pick up and drop off debris
        // Update the droppedOffDebris array as needed

        // get the closest debris to the robot out of 0, 1, 2 and 14

        int closestPrioritizedDebris = getClosestPrioritizedDebris();
        DEBUG(("closestPrioritizedDebris %d", closestPrioritizedDebris));
        removeKeyDebris(closestPrioritizedDebris); 
       
    }
    game.MoveToHome(); // this may not be best for every config
    game.EndGame();

}
    
