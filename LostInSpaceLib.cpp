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
    const float x = 0.0f;
    const float y = 0.0f;
    // Position() : x(0.0f), y(0.0f) {}
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
        return DistanceTo(GetRobotPosition());
    };
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
    const unsigned int id;
    const unsigned int mass;
    unsigned int width;
    DebrisType type;
    Debris(unsigned int id) : id(id), mass(game.GetObjectMass(id))
    {
        if (id >= 0 && id <= 2)
        {
            type = DebrisType::LARGE;
            width = 15;
        }
        else if (id >= 3 && id <= 13)
        {
            type = DebrisType::SMALL;
            width = 5;
        }
        else if (id == 14)
        {
            type = DebrisType::SPECIAL;
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

Position GetRobotPosition()
{
    return Position(game.GetRobotPositionX(), game.GetRobotPositionY());
}

float GetTime()
{
    return game.GetTime();
}

float GetBattery()
{
    return game.GetBattery();
}

float GetScore()
{
    return game.GetScore();
}

const Debris *GetObjectBeingCarried()
{
    if (game.GetObjectBeingCarried() >= 0)
    {
        // SAFETY (yes im pulling a Rust):
        // We just checked if the value is >= 0
        return &(DebrisList[game.GetObjectBeingCarried()]);
    }
    else
    {
        return nullptr;
    }
}

Position GetObjectLocation(int obj)
{
    return Position(game.GetObjectLocX(obj), game.GetObjectLocY(obj));
}
Position GetObjectLocation(Debris *debris)
{
    return GetObjectLocation(debris->id);
}
Position GetObjectLocation(Debris &debris)
{
    return GetObjectLocation(&debris);
}

int GetObjectMass(int obj)
{
    return game.GetObjectMass(obj);
}
int GetObjectMass(Debris *debris)
{
    return GetObjectMass(debris->id);
}
int GetObjectMass(Debris &debris)
{
    return GetObjectMass(&debris);
}

int GetObjectScore(int obj)
{
    return game.GetObjectScore(obj);
}
int GetObjectScore(Debris *debris)
{
    return GetObjectScore(debris->id);
}
int GetObjectScore(Debris &debris)
{
    return GetObjectScore(&debris);
}

int GetObjectPenalty(int obj)
{
    return game.GetObjectPenalty(obj);
}
int GetObjectPenalty(Debris *debris)
{
    return GetObjectPenalty(debris->id);
}
int GetObjectPenalty(Debris &debris)
{
    return GetObjectPenalty(&debris);
}

bool MoveTo(float x, float y)
{
    return game.MoveTo(x, y);
}
bool MoveTo(Position *pos)
{
    return MoveTo(pos->x, pos->y);
}
bool MoveTo(Position &pos)
{
    return MoveTo(&pos);
}

bool MoveToHome()
{
    return game.MoveToHome();
}

bool GrabObject(int obj)
{
    return game.GrabObject(obj);
}
bool GrabObject(Debris *debris)
{
    return GrabObject(debris->id);
}
bool GrabObject(Debris &debris)
{
    return GrabObject(&debris);
}

bool DropObject()
{
    return game.DropObject();
}

void EndGame()
{
    game.EndGame();
}

void MoveToVoid(float x, float y)
{
    game.MoveToVoid(x, y);
}
void MoveToVoid(Position *pos)
{
    MoveToVoid(pos->x, pos->y);
}
void MoveToVoid(Position &pos)
{
    MoveToVoid(&pos);
}

void GrabObjectVoid(int obj)
{
    game.GrabObjectVoid(obj);
}
void GrabObjectVoid(Debris *debris)
{
    GrabObjectVoid(debris->id);
}
void GrabObjectVoid(Debris &debris)
{
    GrabObjectVoid(&debris);
}

Debris (*GetAllDebris())[15]
{
    return &DebrisList;
}

Debris *GetNearestDebris()
{
    int nearestId = -1;
    double nearestDistance = 100; // No debris can be more than sqrt(2) (?) units away, this handles that pretty well

    for (int i = 0; i < 15; i++)
    {
        Debris curr = DebrisList[i];
        double distance = curr.DistanceFromAstrobee();
        if (distance < nearestDistance)
        {
            nearestId = i;
            nearestDistance = distance;
        }
    }

    // SAFETY: Some debris must exist; therefore at some point nearestId != -1 will hold true permanently
    return &DebrisList[nearestId];
}

bool checkCollision(Position linePos1, Position linePos2)
{

}

Debris DebrisList[15] = {
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
    Debris(14)};