/*
These functions are all present in the simulator,
this is mainly just a utility for development
(with autocomplete 🤑).

Also the compiler will yell at you if you try to
compile this in VS Code lol
*/

#ifndef ZR_SIMULATOR_API
#define ZR_SIMULATOR_API

#define DUMMY_FLOAT { return 0.0f; }
#define DUMMY_BOOL { return false; }
#define DUMMY_INT { return -1; }
#define DUMMY_VOID {}

// Functions wrapper (see `game` constant)
class Game
{
public:
    /**
     * Gets the current X position of the robot.
     * @return The X-coordinate of the robot.
     */
    float GetRobotPositionX() DUMMY_FLOAT;

    /**
     * Gets the current Y position of the robot.
     * @return The Y-coordinate of the robot.
     */
    float GetRobotPositionY() DUMMY_FLOAT;

    /**
     * Gets the current elapsed time since the start of the game.
     * @return The elapsed time in seconds.
     */
    float GetTime() DUMMY_FLOAT;

    /**
     * Gets the current battery level of the robot.
     * @return The battery level as a percentage (0 to 100).
     */
    float GetBattery() DUMMY_FLOAT;

    /**
     * Gets the current game score.
     * @return The game score.
     */
    float GetScore() DUMMY_FLOAT;

    /**
     * Gets the Object ID of the object currently being carried by the robot.
     * @return The Object ID, or -1 if no object is being carried.
     */
    int GetObjectBeingCarried() DUMMY_INT;

    /**
     * Gets the current X location of an object.
     * @param obj The Object ID.
     * @return The X-coordinate of the object.
     */
    float GetObjectLocX(int obj) DUMMY_FLOAT;

    /**
     * Gets the current Y location of an object.
     * @param obj The Object ID.
     * @return The Y-coordinate of the object.
     */
    float GetObjectLocY(int obj) DUMMY_FLOAT;

    /**
     * Moves the robot to a particular location on the playing field.
     * @param x The X-coordinate of the destination.
     * @param y The Y-coordinate of the destination.
     * @return True if the movement is valid, False otherwise.
     */
    bool MoveTo(float x, float y) DUMMY_BOOL;

    /**
     * Moves the robot to the home position (0, 0).
     * @return True if the movement is valid, False otherwise.
     */
    bool MoveToHome() DUMMY_BOOL;

    /**
     * Attempts to grab an object specified by its Object ID.
     * @param obj The Object ID to grab.
     * @return True if the grab is successful, False otherwise.
     */
    bool GrabObject(int obj) DUMMY_BOOL;

    /**
     * Drops the object currently being carried by the robot.
     * @return True if successful, False otherwise.
     */
    bool DropObject() DUMMY_BOOL;

    /**
     * Ends the current simulation and stops the timer.
     */
    void EndGame() DUMMY_VOID;

    /**
     * Moves the robot to a particular location on the playing field.
     * @param x The X-coordinate of the destination.
     * @param y The Y-coordinate of the destination.
     * Does not return a value.
     */
    void MoveToVoid(float x, float y) DUMMY_VOID;

    /**
     * Attempts to grab an object specified by its Object ID.
     * @param obj The Object ID to grab.
     * Does not return a value.
     */
    void GrabObjectVoid(int obj) DUMMY_VOID;

    /**
     * Drops the object currently being carried by the robot.
     * Does not return a value.
     */
    void DropObjectVoid(int obj) DUMMY_VOID;
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
