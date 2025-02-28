/*
These functions are all present in the simulator,
this is mainly just a utility for development
(with autocomplete 🤑).
*/

#ifndef ZR_SIMULATOR_API
#define ZR_SIMULATOR_API

class Game
{
public:
    /**
     * Gets the current X position of the robot.
     * @return The X-coordinate of the robot.
     */
    float GetRobotPositionX();

    /**
     * Gets the current Y position of the robot.
     * @return The Y-coordinate of the robot.
     */
    float GetRobotPositionY();

    /**
     * Gets the current elapsed time since the start of the game.
     * @return The elapsed time in seconds.
     */
    float GetTime();

    /**
     * Gets the current battery level of the robot.
     * @return The battery level as a percentage (0 to 100).
     */
    float GetBattery();

    /**
     * Gets the current game score.
     * @return The game score.
     */
    float GetScore();

    /**
     * Gets the Object ID of the object currently being carried by the robot.
     * @return The Object ID, or -1 if no object is being carried.
     */
    int GetObjectBeingCarried();

    /**
     * Gets the current X location of an object.
     * @param obj The Object ID.
     * @return The X-coordinate of the object.
     */
    float GetObjectLocX(int obj);

    /**
     * Gets the current Y location of an object.
     * @param obj The Object ID.
     * @return The Y-coordinate of the object.
     */
    float GetObjectLocY(int obj);

    /**
     * Gets the mass of an object.
     * @param obj The Object ID.
     * @return The mass of the object.
     */
    int GetObjectMass(int obj);

    /**
     * Gets the score awarded for moving an object out of the field.
     * @param obj The Object ID.
     * @return The score awarded for the object.
     */
    int GetObjectScore(int obj);

    /**
     * Gets the penalty incurred for leaving an object inside the field.
     * @param obj The Object ID.
     * @return The penalty incurred for the object.
     */
    int GetObjectPenalty(int obj);

    /**
     * Moves the robot to a particular location on the playing field.
     * @param x The X-coordinate of the destination.
     * @param y The Y-coordinate of the destination.
     * @return True if the movement is valid, False otherwise.
     */
    bool MoveTo(float x, float y);

    /**
     * Moves the robot to the home position (0, 0).
     * @return True if the movement is valid, False otherwise.
     */
    bool MoveToHome();

    /**
     * Attempts to grab an object specified by its Object ID.
     * @param obj The Object ID to grab.
     * @return True if the grab is successful, False otherwise.
     */
    bool GrabObject(int obj);

    /**
     * Drops the object currently being carried by the robot.
     * @return True if successful, False otherwise.
     */
    bool DropObject();

    /**
     * Ends the current simulation and stops the timer.
     */
    void EndGame();

    /**
     * Moves the robot to a particular location on the playing field.
     * @param x The X-coordinate of the destination.
     * @param y The Y-coordinate of the destination.
     * Does not return a value.
     */
    void MoveToVoid(float x, float y);

    /**
     * Attempts to grab an object specified by its Object ID.
     * @param obj The Object ID to grab.
     * Does not return a value.
     */
    void GrabObjectVoid(int obj);

    /**
     * Drops the object currently being carried by the robot.
     * Does not return a value.
     */
    void DropObjectVoid(int obj);

    void SetLayout(int layout);
    void SetStartPosition(int pos);
};

/**
 * This is the normal print function.
 * Be advised that this does not work on Astrobee (in the simulator).
 */
void printf(const char *format, ...);

/**
 * Debug print function. Behaves exactly like printf from
 * `stdio.h`, but only works in the simulator and prints out
 * with a `[DBG]` prefix.
 * The `DEBUG` macro is a wrapper for this function.
 */
void debugPrintf(const char *format, ...);

// Debug print macro.
#define DEBUG(args) debugPrintf args;

// Global game object; this is how Astrobee functions are called...for some reason
Game game;

#endif // ZR_SIMULATOR_API