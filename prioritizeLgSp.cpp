
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

int howManyMins(int* array) {

    // get the min of the array and return how many instances of this minimum integer are in the array
    int min = 1000000;
    int count = 0;
    for (int i = 0; i < 15; ++i) {
        if (array[i] < min) {
            min = array[i];
            count = 1;
        } else if (array[i] == min) {
            count++;
        }
    }
    return count;
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

float getDistanceToWall() { // get the distance to the closest wall using the getClosestWall func

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

    for (int debrisIndex = 0; debrisIndex < 15; ++debrisIndex) {
        if (droppedOffDebris[debrisIndex] || (debrisIndex > 2 && debrisIndex != 14) || isOutsideImagingZone(debrisIndex)) {
            continue; // Skip debris that have been dropped off, are not 0, 1, 2 or 14, or are outside the imaging zone
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

    game.MoveTo(destinationPoint[0], destinationPoint[1]); // move astrobee to the destination point

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

    game.MoveTo(destinationPoint[0] + xoffset, destinationPoint[1] + yOffset); // move astrobee to the destination point

}

//Declare any variables shared between functions here
void init(){
    //This function is called once when your code is first loaded.
    game.SetLayout(1);
    game.SetStartPosition(1);
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

    while (!isOutsideImagingZone(0) || !isOutsideImagingZone(1) || !isOutsideImagingZone(2) || !isOutsideImagingZone(14)) {
        // Add your logic here to handle the debris
        // For example, you can move the robot to pick up and drop off debris
        // Update the droppedOffDebris array as needed

        // get the closest debris to the robot out of 0, 1, 2 and 14

        int closestPrioritizedDebris = getClosestPrioritizedDebris();
        // get the direction in which this debris has the least key debris, and return an array of the indexes of the key debris

        int (*keyDebris)[15] = getKeyDebris(closestPrioritizedDebris);
        int minKeyDebrisCount = 15;
        int minKeyDebrisDirection = -1;

        // get location of closest prioritized debris
        float* closestPrioritizedDebrisLocation = getDebrisLocations()[closestPrioritizedDebris];

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
            }
        }

        if (minKeyDebrisDirection != -1) {
            printf("Direction with least key debris: %d\n", minKeyDebrisDirection);
            printf("Key debris in that direction: ");
            for (int i = 0; i < 15 && keyDebris[minKeyDebrisDirection][i] != -1; ++i) {
            printf("%d ", keyDebris[minKeyDebrisDirection][i]);
            }
            printf("\n");
        }

        if (minKeyDebrisCount == 0) {
            // there exists at least one direction where there are no key debris, if this is the case, let astrobee directly remove the debris.
            game.MoveTo(closestPrioritizedDebrisLocation[0], closestPrioritizedDebrisLocation[1]);
            game.GrabObject(closestPrioritizedDebris);
            removeDebris(closestPrioritizedDebris, minKeyDebrisDirection);
            game.DropObject();
        } else if (howManyMins(numberOfKeyDebris) == 1) {
            // If there is only one direction with the least number of key debris
            // rm key debris first
            // offset equals half of 5 cm if key debris is small, and half of 15 cm if key debris is large
            
            for (int i = 0; i < 15 && keyDebris[minKeyDebrisDirection][i] != -1; ++i) {
                // keyDebris[minKeyDebrisDirection][i] is the index of kety debris
                float offset = (keyDebris[minKeyDebrisDirection][i] <= 2) ? 0.075f : 0.025f;
                offset += (closestPrioritizedDebris <= 2) ? 0.075f : 0.025f; // Add half the width of the closestPrioritizedDebris
                // deinfe x and y offsets, y offset will be 0 if the debris is in the north or south direction, and x offset will be 0 if the debris is in the east or west direction
                float yOffset = (minKeyDebrisDirection == 0 || minKeyDebrisDirection == 2) ? 0.0f : offset;
                float xOffset = (minKeyDebrisDirection == 1 || minKeyDebrisDirection == 3) ? 0.0f : offset;

                if (minKeyDebrisCount == 1)
                { // if there is only one key debris in the direction
                    // move to the key debris
                    float* keyDebrisLocation = getDebrisLocations()[keyDebris[minKeyDebrisDirection][i]];
                    game.MoveTo(keyDebrisLocation[0], keyDebrisLocation[1]);
                    game.GrabObject(keyDebris[minKeyDebrisDirection][i]);
                    removeDebrisWithOffset(keyDebris[minKeyDebrisDirection][i], minKeyDebrisDirection, xOffset, yOffset);
                    game.DropObject();
                    droppedOffDebris[keyDebris[minKeyDebrisDirection][i]] = true;

                    // after removing the single key debris, move back to the debris and remove it
                    game.MoveTo(closestPrioritizedDebrisLocation[0], closestPrioritizedDebrisLocation[1]);
                    game.GrabObject(closestPrioritizedDebris);
                    removeDebris(closestPrioritizedDebris, minKeyDebrisDirection);
                    game.DropObject();
                }

                // if there are two key debris, do same thing however negate the offset that is not zero (if xOffset is 0 and yOffset is some value, move one of them to the positive of that value and the other to the negative of that value)
                else if (minKeyDebrisCount == 2) {
                    for (int z = 0; z < 2; ++z) {
                        float* keyDebrisLocation = getDebrisLocations()[keyDebris[minKeyDebrisDirection][i]];
                        game.MoveTo(keyDebrisLocation[0], keyDebrisLocation[1]);
                        game.GrabObject(keyDebris[minKeyDebrisDirection][i]);
                        float offset = (keyDebris[minKeyDebrisDirection][i] <= 2) ? 0.025f : 0.075f;
                offset += (closestPrioritizedDebris <= 2) ? 0.075f : 0.025f; // Add half the width of the closestPrioritizedDebris
                        float yOffset = (minKeyDebrisDirection == 0 || minKeyDebrisDirection == 2) ? 0.0f : (z == 0 ? offset : -offset);
                        float xOffset = (minKeyDebrisDirection == 1 || minKeyDebrisDirection == 3) ? 0.0f : (z == 0 ? offset : -offset);
                        removeDebrisWithOffset(keyDebris[minKeyDebrisDirection][i], minKeyDebrisDirection, xOffset, yOffset);
                        game.DropObject();
                        droppedOffDebris[keyDebris[minKeyDebrisDirection][i]] = true;
                    }

                    // after removing the two key debris, move back to the debris and remove it
                    game.MoveTo(closestPrioritizedDebrisLocation[0], closestPrioritizedDebrisLocation[1]);
                    game.GrabObject(closestPrioritizedDebris);
                    removeDebris(closestPrioritizedDebris, minKeyDebrisDirection);
                    game.DropObject();
                    droppedOffDebris[closestPrioritizedDebris] = true;
                }


            }

        
        } else {
            // If there is a tie between the number of key debris in each direction
            // Find the direction with the least distance to the wall
            float minDistanceToWall = 30.0f;
            int bestDirection = -1;

            for (int direction = 0; direction < 4; ++direction) {
                if (numberOfKeyDebris[direction] == minKeyDebrisCount) {
                    float distanceToWall = getDistanceToWall();
                    if (distanceToWall < minDistanceToWall) {
                        minDistanceToWall = distanceToWall;
                        bestDirection = direction;
                    }
                }
            }

            if (bestDirection != -1) {
                // we have found the best direction

                for (int i = 0; i < 15 && keyDebris[bestDirection][i] != -1; ++i) {
                    // keyDebris[bestDirection][i] is the index of kety debris
                    float offset = (keyDebris[minKeyDebrisDirection][i] <= 2) ? 0.025f : 0.075f;
                offset += (closestPrioritizedDebris <= 2) ? 0.075f : 0.025f; // Add half the width of the closestPrioritizedDebris
                    // deinfe x and y offsets, y offset will be 0 if the debris is in the north or south direction, and x offset will be 0 if the debris is in the east or west direction
                    float yOffset = (bestDirection == 0 || bestDirection == 2) ? 0.0f : offset;
                    float xOffset = (bestDirection == 1 || bestDirection == 3) ? 0.0f : offset;

                    if (minKeyDebrisCount == 1)
                    { // if there is only one key debris in the direction
                        // move to the key debris
                        float* keyDebrisLocation = getDebrisLocations()[keyDebris[bestDirection][i]];
                        game.MoveTo(keyDebrisLocation[0], keyDebrisLocation[1]);
                        game.GrabObject(keyDebris[minKeyDebrisDirection][i]);
                        removeDebrisWithOffset(keyDebris[bestDirection][i], bestDirection, xOffset, yOffset);
                        game.DropObject();
                        droppedOffDebris[keyDebris[bestDirection][i]] = true;

                        // after removing the single key debris, move back to the debris and remove it
                        game.MoveTo(closestPrioritizedDebrisLocation[0], closestPrioritizedDebrisLocation[1]);
                        game.GrabObject(closestPrioritizedDebris);
                        removeDebris(closestPrioritizedDebris, bestDirection);
                        game.DropObject();
                    }

                    // if there are two key debris, do same thing however negate the offset that is not zero (if xOffset is 0 and yOffset is some value, move one of them to the positive of that value and the other to the negative of that value)
                    else if (minKeyDebrisCount == 2) {
                        for (int z = 0; z < 2; ++z) {
                            float* keyDebrisLocation = getDebrisLocations()[keyDebris[bestDirection][i]];
                            game.MoveTo(keyDebrisLocation[0], keyDebrisLocation[1]);
                            game.GrabObject(keyDebris[minKeyDebrisDirection][i]);
                            float offset = (keyDebris[minKeyDebrisDirection][i] <= 2) ? 0.025f : 0.075f;
                offset += (closestPrioritizedDebris <= 2) ? 0.075f : 0.025f; // Add half the width of the closestPrioritizedDebris
                            float yOffset = (bestDirection == 0 || bestDirection == 2) ? 0.0f : (z == 0 ? offset : -offset);
                            float xOffset = (bestDirection == 1 || bestDirection == 3) ? 0.0f : (z == 0 ? offset : -offset);
                            removeDebrisWithOffset(keyDebris[bestDirection][i], bestDirection, xOffset, yOffset);
                            game.DropObject();
                            droppedOffDebris[keyDebris[bestDirection][i]] = true;
                        }

                        // after removing the two key debris, move back to the debris and remove it
                        game.MoveTo(closestPrioritizedDebrisLocation[0], closestPrioritizedDebrisLocation[1]);
                        game.GrabObject(closestPrioritizedDebris);
                        removeDebris(closestPrioritizedDebris, bestDirection);
                        game.DropObject();
                        droppedOffDebris[closestPrioritizedDebris] = true;
                    }
                }


            }


            }

             // if there are no more debris in the imaging zone, break out of the loop
        if (closestPrioritizedDebris == -1) {
            break;
        }



       
    }

}
    
