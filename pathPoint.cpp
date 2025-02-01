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

        pathPoint[0] = currentLocation[0] + detlaX; // x-coordinate of intersection
        pathPoint[1] = -0.35; // y-coordinate of intersection
    } else if (closestWall == 3) {
        float deltaX = 0.35 + currentLocation[0]; // distance to left wall
        float detlaY = deltaX * slopeOfLine; // distance to left wall multiplied by the slope of the line

        pathPoint[0] = -0.35; // x-coordinate of intersection
        pathPoint[1] = currentLocation[1] + detlaY; // y-coordinate of intersection
    }
    

    DEBUG(("pathpotnierntoe %f %f", pathPoint[0], pathPoint[1]));
    return pathPoint; // return the intersection point
}
