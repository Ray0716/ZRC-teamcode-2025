#include <map> 
#include <list> 
using namespace std; 
// Global array to keep track of dropped off debris
bool droppedOffDebris[15];

float* getCurrentLocation() {
    static float resultArr[2] = {game.GetRobotPositionX(), game.GetRobotPositionY()}; // array to hold coords of each obj, index cooresponds to obj, x and y

    return resultArr; // Return the pointer to the 2D array
}

float (*getDebrisLocations())[2] {
    static float resultArr[15][2] = {{0.0f}}; // array to hold coords of each obj, index cooresponds to obj, x and y
    
    for (int debris = 0; debris < 15; ++debris) {             // Outer loop for rows
        for (int coord = 0; coord < 2; ++coord) {         // Inner loop for columns
            if(coord == 0){
                resultArr[debris][coord] = static_cast<float>(game.GetObjectLocX(debris)); // Assign values
            } else {
                resultArr[debris][coord] = static_cast<float>(game.GetObjectLocY(debris));
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
    
    return sqrt((x2-x1)*(x2-x1) + (y2-y1)*(y2-y1));
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

    for (int debrisIndex = 3; debrisIndex < 14; ++debrisIndex) {
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

float* getPathPoint(float* destinationPoint) {
    float currentLocation[2] = {0.0f}; // [xcoord, ycoord]
    currentLocation[0] = game.GetRobotPositionX();
    currentLocation[1] = game.GetRobotPositionY(); // get the current location of the robot

    int closestWall = getClosestWall(); // get the index of the closest wall

    float* dPrime = new float[2]; // [xcoord, ycoord]
    // dprime is the point of the reflected destination point

    if (closestWall == 0) {
        dPrime[0] = destinationPoint[0]; // x-coordinate of the reflected destination point is the same because the wall is horizontal
        dPrime[1] = destinationPoint[1] + 2 * (0.35 - destinationPoint[1]); // y-coordinate of the reflected destination point is calculated by getting the distance from the destination point to the wall and adding twice that distance to the destination point
    } else if (closestWall == 1) {
        dPrime[0] = destinationPoint[0] + 2 * (0.35 - destinationPoint[0]);
        dPrime[1] = destinationPoint[1];
    } else if (closestWall == 2) {
        dPrime[0] = destinationPoint[0];
        dPrime[1] = destinationPoint[1] - 2 * (0.35 + destinationPoint[1]);
    } else if (closestWall == 3) {
        dPrime[0] = destinationPoint[0] - 2 * (0.35 + destinationPoint[0]);
        dPrime[1] = destinationPoint[1];
    }

    float* pathPoint = new float[2]; // [xcoord, ycoord]

    float slopeOfLine = (dPrime[1] - currentLocation[1]) / (dPrime[0] - currentLocation[0]); // y2 - y1 / x2 - x1 of the line from the robot to the reflected destination point

    // Calculate intersection with wall
    if (closestWall == 0) {
        float deltaY = 0.35 - currentLocation[1]; // distance to top wall
        float detlaX = deltaY / slopeOfLine; // distance to top wall divided by the slope of the line
        pathPoint[0] = currentLocation[0] + detlaX; // x-coordinate of intersection
        pathPoint[1] = 0.35; // y-coordinate of intersection
    } else if (closestWall == 1)
    {
        float deltaX = 0.35 - currentLocation[0]; // distance to right wall
        float detlaY = deltaX * slopeOfLine; // distance to right wall multiplied by the slope of the line

        pathPoint[0] = 0.35; // x-coordinate of intersection
        pathPoint[1] = currentLocation[1] + detlaY; // y-coordinate of intersection
    } else if (closestWall == 2) {

        float deltaY = 0.35 + currentLocation[1]; // distance to bottom wall
        float detlaX = deltaY / slopeOfLine; // distance to bottom wall divided by the slope of the line

        pathPoint[0] = currentLocation[0] - detlaX; // x-coordinate of intersection
        pathPoint[1] = -0.35; // y-coordinate of intersection
    } else if (closestWall == 3) {
        float deltaX = 0.35 + currentLocation[0]; // distance to left wall
        float detlaY = deltaX * slopeOfLine; // distance to left wall multiplied by the slope of the line

        pathPoint[0] = -0.35; // x-coordinate of intersection
        pathPoint[1] = currentLocation[1] - detlaY; // y-coordinate of intersection
    }
    

    DEBUG(("pathpotnierntoe %f %f", pathPoint[0], pathPoint[1]));
    return pathPoint; // return the intersection point
}

bool allSmallDebrisOut() { // the small debris are indexes 3 - 14, check if all the small debris are outside of the 
    return true;
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
}

void loop(){
    //This function is called once per second.  Use it to control the satellite.
    
    while(game.GetTime() <= 320) {
    static float debrisSearchRadius = 0.3;
    DEBUG(("hello world"));
    
    float (*debrisArray)[2] = getDebrisLocations();
    
    bool debrisInRadius = false;
    for (int debrisIndex = 3; debrisIndex < 14; ++debrisIndex) {             // Outer loop for rows
        float distance = getDistance(getCurrentLocation(), debrisArray[debrisIndex]);
        if (distance < debrisSearchRadius) {
            debrisInRadius = true;
            break;
        }
    }

    int closestDebris = getClosestDebris();
    if (closestDebris == -1) {
        return; // No debris found
    }
    float* closestDebrisLocation = debrisArray[closestDebris];

    int secondClosestDebris = getSecondClosestDebris();
    if (secondClosestDebris == -1) {
        return; // No second closest debris found
    }
    float* secondClosestDebrisLocation = debrisArray[secondClosestDebris];

    game.MoveTo(closestDebrisLocation[0], closestDebrisLocation[1]);
    game.GrabObject(closestDebris);
    DEBUG(("obj grab"));
    game.GetObjectBeingCarried();
    
    DEBUG(("sedn %f %f", secondClosestDebrisLocation[0], secondClosestDebrisLocation[1]));
    float* bouncePoint = getPathPoint(secondClosestDebrisLocation);
    game.MoveTo(bouncePoint[0], bouncePoint[1]);
    DEBUG(("movedtobounceoputn"));
    game.DropObject();

    droppedOffDebris[closestDebris] = true; // Mark the debris as dropped off

    game.MoveTo(secondClosestDebrisLocation[0], secondClosestDebrisLocation[1]);
    DEBUG(("one cyclke complete"));}
    
}

// checks if there is a collision in any one of the cardinal directions (returns -1 if no collision)
int inCollisionRange(int debrisIndex1, int debrisIndex2) {
    float (*debrisArray)[2] = getDebrisLocations(); 
    float *debrisLocation1 = debrisArray[debrisIndex1];
    float *debrisLocation2 = debrisArray[debrisIndex2]; 

    float x1 = debrisLocation1[0];
    float y1 = debrisLocation1[1];
    float x2 = debrisLocation2[0];
    float y2 = debrisLocation2[1];

    float offset = (debrisIndex1 <= 2) ? (0.15 / 2.0) : (0.05 / 2.0); // sets offset based on whether the debris is large or small

    if (x2 <= x1 + offset && x2 >= x1 - offset) {
        return (y2 <= y1) ? 0 : 2; // north or south cardinal direction
    } else if (y2 <= y1 + offset && y2 >= y1 - offset) {
        return (x2 <= x1) ? 3 : 1; // west or east cardinal direction
    }

    return -1; // no collision
}

/*
map<int, list<int>[4]> getKeyDebris() {
    map<int, list<int>[4]> neighbors;
    float (*debrisArray)[2] = getDebrisLocations();

    for (int direction = 0; direction < 4; ++direction) {
        for (int debrisIndex = 0; debrisIndex < 15; ++debrisIndex) {
            if (debrisIndex <= 2 || debrisIndex == 14) {
                continue;
            }
 
            switch (direction) {
                case 0: 
                    if (debrisArray[debrisIndex][1] > 0.15) {
                        neighbors[debrisIndex][direction].push_back(debrisIndex);
                    }
                    break;
            }
        }
    }
    return neighbors; 
} */

// getKeyDebris(debris index) returns a 2d array, each sub array is the indecies of the key derbris of one debris in each cardinal direction. 0th index subarray is north key debris, 1 is east, 2 is south, 3 is west. key debris is defined as debris that is in the area such that if we were to move the debris in a cardinal direction, it would collide with the debris. the subarray is empty if there is no key debris in that direction. to get the area that is the collision zone we can simply loop through north east south and west zones, then loop through all of the debris locations and see if they are in the zones if they are, add the debris index to the corresponding subarray that corresponds to which zone it is in. to calculate the zone dimensions we need to know the size of the debris. debris indexes 0-2 inclusive are all large debris with a lendgh and width of 15cm (0.15 meter, we use meters), and all other debris are 5 by 5 cm (0.05 meters). so for example, if we had a large debris, the north coolision zone would be defined as two vertical lines, each 0.075m away from the center of the debris. this would create a zone extending from the debris upwards with a width of the debris (0.15m). 
// note, this modified the above comment: because we cannot use vectors in the simulation IDE, we need to use normal arrays. let's use another approahc: in our final 2d array, each subarray for each cardial direction zone will be an array with legnth 10. it will start out with all -1's. if at the end they are still all -1, then that means there are no key debris in that zone, howveer if there are, we repalce the element with the index of it. for example, if this was the first subnarray: [0, 1, 2, -1, -1, -1 ... -1] means that debris numbers 0, 1, 2 are in the north zone. now, if [-1, -1, -1 ... -1] was the last subarray, this means there are no debsi in the west zone. now this approach allows us to write the function as int** getkey... 



