// These are wrappers for Astrobee wee

#ifdef __has_include
#include "math.h"
#if __has_include("Astrobee.h")
#include "Astrobee.h"
#endif
#endif

#define unreachable throw "unreachable code point reached"

/*
class Position
{
public:
    float x;
    float y;
    Position(float x, float y) : x(x), y(y) {}
    double DistanceTo(Position *other) const
    {
        return sqrt(pow(this->x - other->x, 2) + pow(this->y - other->y, 2));
    };
    double DistanceTo(Position &other) const
    {
        return DistanceTo(&other);
    };
    double DistanceFromAstrobee() const
    {
        Position robot = GetRobotPosition();
        return DistanceTo(robot);
    };
    Position Midpoint(Position *other) const
    {
        return Position((this->x + other->x) / 2, (this->y + other->y) / 2);
    }
    Position Midpoint(Position &other) const
    {
        return Midpoint(&other);
    }
};

enum DebrisType
{
    LARGE,
    SMALL,
    SPECIAL // ooo ahh special very special
};

class Debris
{
public:
    unsigned int id;
    unsigned int mass;
    unsigned int width;
    enum DebrisType type;
    Debris(unsigned int id) : id(id), mass(game.GetObjectMass(id))
    {
        if (id >= 0 && id <= 2)
        {
            type = LARGE;
            width = 15;
        }
        else if (id >= 3 && id <= 13)
        {
            type = SMALL;
            width = 5;
        }
        else if (id == 14)
        {
            type = SPECIAL;
            width = 5;
        }
        else
        {
            // This should literally never happen. If it does, someone screwed up lol.
            unreachable;
        }
    }
    Position GetLocation()
    {
        return GetObjectLocation(this);
    };
    // Alias for `GetLocation`
    Position GetPosition()
    {
        return GetLocation();
    };
    // I love the distance formula
    // You love the distance formula
    // We all love the distance formula
    double DistanceFromAstrobee()
    {
        return GetPosition().DistanceFromAstrobee();
    };
};
*/

float (*Midpoint(float first[2], float second[2]))[2]
{
    float arr[2] = {(first[0] + second[0]) / 2, (first[1] + second[1]) / 2};
    return &arr;
}

double DistanceFromAstrobee(float x, float y) {
    float robot[2] = {game.GetRobotPositionX(), game.GetRobotPositionY()};
    return sqrt(pow(x - robot[0], 2) + pow(y - robot[1], 2));
}

static const unsigned int GetNearestDebris()
{
    int nearestId = -1;
    double nearestDistance = 100; // No debris can be more than sqrt(2) (?) units away, this handles that pretty well
    for (int i = 0; i < 15; i++)
    {
        double distance = DistanceFromAstrobee(game.GetObjectLocX(i), game.GetObjectLocY(i));
        if (distance < nearestDistance)
        {
            nearestId = i;
            nearestDistance = distance;
        }
    }

    // SAFETY: Some debris must exist; therefore at some point nearestId != -1 will hold true permanently
    return nearestId;
}

static const unsigned int GetDebrisWidth(unsigned int id)
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
        // This should literally never happen. If it does, someone screwed up lol.
        unreachable;
    }
}

static bool checkLineCollisions(float linePos1[2], float linePos2[2])
{
    int m = (linePos1[1] - linePos2[1]) / (linePos1[0] - linePos2[0]);
    int b = linePos1[1] - m * linePos1[0];

    // Use the line equation to check for collisions in each Debris.
    for (int i = 0; i < 15; i++)
    {
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

static bool hasCollisions(float linePos1[2], float linePos2[2])
{
    int m = (linePos1[1] - linePos2[1]) / (linePos1[0] - linePos2[0]);

    float (*centerPtr)[2] = Midpoint(linePos1, linePos2);
    float center[2] = {(*centerPtr)[0], (*centerPtr)[1]};

    if (m >= 0)
    {
        // Check the lines with slope m passing through upperLeft and lowerRight
        float upperLeft[2] = {center[0] - 3.2f, center[1] + 3.2f};
        float lowerRight[2] = {center[0] + 3.2f, center[1] - 3.2f};
        float firstCheck[2] = {upperLeft[0] + 1, upperLeft[1] + m};
        float secondCheck[2] = {lowerRight[0] + 1, lowerRight[1] + m};
        return checkLineCollisions(upperLeft, firstCheck) || checkLineCollisions(lowerRight, secondCheck);
    }
    else
    {
        // Check the lines with slope m passing through lowerLeft and upperRight
        float lowerLeft[2] = {center[0] - 3.2f, center[1] - 3.2f};
        float upperRight[2] = {center[0] + 3.2f, center[1] + 3.2f};
        float firstCheck[2] = {lowerLeft[0] + 1, lowerLeft[1] + m};
        float secondCheck[2] = {upperRight[0] + 1, upperRight[1] + m};
        return checkLineCollisions(lowerLeft, firstCheck) || checkLineCollisions(upperRight, secondCheck);
    }
}
