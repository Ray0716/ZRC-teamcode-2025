// These are wrappers for Astrobee wee

#ifdef __has_include
#include "math.h"
#if __has_include("Astrobee.h")
#include "Astrobee.h"
#endif
#endif

#define unreachable throw "unreachable code point reached"

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

static Position GetRobotPosition()
{
    return Position(game.GetRobotPositionX(), game.GetRobotPositionY());
}

static float GetTime()
{
    return game.GetTime();
}

static float GetBattery()
{
    return game.GetBattery();
}

static float GetScore()
{
    return game.GetScore();
}

const Debris GetObjectBeingCarried()
{
    if (game.GetObjectBeingCarried() >= 0)
    {
        // SAFETY (yes im pulling a Rust):
        // We just checked if the value is >= 0
        return game.GetObjectBeingCarried();
    }
    else
    {
        return NULL;
    }
}

static Position GetObjectLocation(int obj)
{
    return Position(game.GetObjectLocX(obj), game.GetObjectLocY(obj));
}
static Position GetObjectLocation(Debris *debris)
{
    return GetObjectLocation(debris->id);
}
static Position GetObjectLocation(Debris &debris)
{
    return GetObjectLocation(&debris);
}

static int GetObjectMass(int obj)
{
    return game.GetObjectMass(obj);
}
static int GetObjectMass(Debris *debris)
{
    return GetObjectMass(debris->id);
}
static int GetObjectMass(Debris &debris)
{
    return GetObjectMass(&debris);
}

static int GetObjectScore(int obj)
{
    return game.GetObjectScore(obj);
}
static int GetObjectScore(Debris *debris)
{
    return GetObjectScore(debris->id);
}
static int GetObjectScore(Debris &debris)
{
    return GetObjectScore(&debris);
}

static int GetObjectPenalty(int obj)
{
    return game.GetObjectPenalty(obj);
}
static int GetObjectPenalty(Debris *debris)
{
    return GetObjectPenalty(debris->id);
}
static int GetObjectPenalty(Debris &debris)
{
    return GetObjectPenalty(&debris);
}

static bool MoveTo(float x, float y)
{
    return game.MoveTo(x, y);
}
static bool MoveTo(Position *pos)
{
    return MoveTo(pos->x, pos->y);
}
static bool MoveTo(Position &pos)
{
    return MoveTo(&pos);
}

static bool MoveToHome()
{
    return game.MoveToHome();
}

static bool GrabObject(int obj)
{
    return game.GrabObject(obj);
}
static bool GrabObject(Debris *debris)
{
    return GrabObject(debris->id);
}
static bool GrabObject(Debris &debris)
{
    return GrabObject(&debris);
}

static bool DropObject()
{
    return game.DropObject();
}

static void EndGame()
{
    game.EndGame();
}

static void MoveToVoid(float x, float y)
{
    game.MoveToVoid(x, y);
}
static void MoveToVoid(Position *pos)
{
    MoveToVoid(pos->x, pos->y);
}
static void MoveToVoid(Position &pos)
{
    MoveToVoid(&pos);
}

static void GrabObjectVoid(int obj)
{
    game.GrabObjectVoid(obj);
}
static void GrabObjectVoid(Debris *debris)
{
    GrabObjectVoid(debris->id);
}
static void GrabObjectVoid(Debris &debris)
{
    GrabObjectVoid(&debris);
}

static Debris (*GetAllDebris())[15]
{
    static Debris DebrisList[15] = {
        Debris(0), 
        Debris(1), 
        Debris(2), 
        Debris(3), 
        Debris(4),
        Debris(5), 
        Debris(6), 
        Debris(7), 
        Debris(8), 
        Debris(9),
        Debris(10),
        Debris(11),
        Debris(12),
        Debris(13),
        Debris(14)
    };
    return &DebrisList;
}

static const Debris GetNearestDebris()
{
    int nearestId = -1;
    double nearestDistance = 100; // No debris can be more than sqrt(2) (?) units away, this handles that pretty well
    for (int i = 0; i < 15; i++)
    {
        double distance = GetObjectLocation(i).DistanceFromAstrobee();
        if (distance < nearestDistance)
        {
            nearestId = i;
            nearestDistance = distance;
        }
    }

    // SAFETY: Some debris must exist; therefore at some point nearestId != -1 will hold true permanently
    return Debris(nearestId);
}

static bool checkLineCollisions(Position &linePos1, Position &linePos2)
{
    int m = (linePos1.y - linePos2.y) / (linePos1.x - linePos2.x);
    int b = linePos1.y - m * linePos1.x;

    // allocate memory to avoid hotloops of memory allocation + releasing
    Debris debris(0);

    // Use the line equation to check for collisions in each Debris.
    for (int i = 0; i < 15; i++)
    {
        debris = Debris(i);
        Position position = debris.GetLocation();
        int leftBound = position.x - debris.width / 2;
        int rightBound = position.x + debris.width / 2;
        int bottomBound = position.y - debris.width / 2;
        int topBound = position.y + debris.width / 2;

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

static bool hasCollisions(Position &linePos1, Position &linePos2)
{
    int m = (linePos1.y - linePos2.y) / (linePos1.x - linePos2.x);

    Position center = linePos1.Midpoint(linePos2);

    if (m >= 0)
    {
        // Check the lines with slope m passing through upperLeft and lowerRight
        Position upperLeft = Position(center.x - 3.2, center.y + 3.2);
        Position lowerRight = Position(center.x + 3.2, center.y - 3.2);
        Position firstCheck = Position(upperLeft.x + 1, upperLeft.y + m);
        Position secondCheck = Position(lowerRight.x + 1, lowerRight.y + m);
        return checkLineCollisions(upperLeft, firstCheck) || checkLineCollisions(lowerRight, secondCheck);
    }
    else
    {
        // Check the lines with slope m passing through lowerLeft and upperRight
        Position lowerLeft = Position(center.x - 3.2, center.y - 3.2);
        Position upperRight = Position(center.x + 3.2, center.y + 3.2);
        Position firstCheck = Position(lowerLeft.x + 1, lowerLeft.y + m);
        Position secondCheck = Position(upperRight.x + 1, upperRight.y + m);
        return checkLineCollisions(lowerLeft, firstCheck) || checkLineCollisions(upperRight, secondCheck);
    }
}
