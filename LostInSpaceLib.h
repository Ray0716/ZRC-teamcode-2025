// These are wrappers for Astrobee wee

#ifdef __has_include
#if __has_include("Astrobee.h")
#include "Astrobee.h"
#endif
#endif

#define unreachable throw "unreachable code point reached";

class Position
{
public:
    const float x = 0.0f;
    const float y = 0.0f;
    // Position() : x(0.0f), y(0.0f) {}
    Position(float x, float y) : x(x), y(y) {}
    double DistanceTo(Position *other) const;
    double DistanceTo(Position &other) const;
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
            unreachable
        }
    }
    Position GetLocation();
    // Alias for `GetLocation`
    Position GetPosition();
    // I love the distance formula
    // You love the distance formula
    // We all love the distance formula
    double DistanceFromAstrobee();
};

Position GetRobotPosition();

float GetTime();
float GetBattery();
float GetScore();

const Debris *GetObjectBeingCarried();

Position GetObjectLocation(int obj);
Position GetObjectLocation(Debris *debris);
Position GetObjectLocation(Debris &debris);

int GetObjectMass(int obj);
int GetObjectMass(Debris *debris);
int GetObjectMass(Debris &debris);

int GetObjectScore(int obj);
int GetObjectScore(Debris *debris);
int GetObjectScore(Debris &debris);

int GetObjectPenalty(int obj);
int GetObjectPenalty(Debris *debris);
int GetObjectPenalty(Debris &debris);

bool MoveTo(float x, float y);
bool MoveTo(Position *pos);
bool MoveTo(Position &pos);

bool MoveToHome();

bool GrabObject(int obj);
bool GrabObject(Debris *debris);
bool GrabObject(Debris &debris);

bool DropObject();

void EndGame();

void MoveToVoid(float x, float y);
void MoveToVoid(Position *pos);
void MoveToVoid(Position &pos);

void GrabObjectVoid(int obj);
void GrabObjectVoid(Debris *debris);
void GrabObjectVoid(Debris &debris);

Debris (*GetAllDebris())[15];

Debris *GetNearestDebris();

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