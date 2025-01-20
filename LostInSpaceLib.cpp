// These are wrappers for Astrobee wee

#include "math.h"
#include "LostInSpaceLib.h"

double Position::DistanceTo(Position *other) const
{
    return sqrt(pow(this->x - other->x, 2) + pow(this->y - other->y, 2));
}
double Position::DistanceTo(Position &other) const
{
    return DistanceTo(&other);
}

Position Debris::GetLocation()
{
    return GetObjectLocation(this);
}
Position Debris::GetPosition()
{
    return GetLocation();
}
double Debris::DistanceFromAstrobee()
{
    return GetPosition().DistanceTo(GetRobotPosition());
}

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