#if !defined WATER_INTERFACE_TIME_INCLUDED
#define WATER_INTERFACE_TIME_INCLUDED


// STL headers.
#include <string>


// Engine namespace.
namespace water
{
    /// <summary>
    /// An interface to time systems, these hold delta time values as well as control the scale of time.
    /// Time classes should be used to represent the time values of the current context. In other words if deltaTime() 
    /// is called in the physicsUpdate() then it should reflect the physics delta time, normal update should reflect
    /// the standard update time, etc.
    /// </summary>
    class ITime
    {
        public:

            // Ensure destructor is virtual since this is an interface.
            virtual ~ITime() {}


            #pragma region Time management

            /// <summary> Get the delta time value of the current update loop. </summary>
            /// <returns> The physics update time during physicsUpdate() or the update time during update(). </returns>
            virtual float deltaTime() const = 0;

            /// <summary> Gets the delta time for the most recent physics loop. </summary>
            virtual float physicsTime() const = 0;

            /// <summary> Gets the delta time for the most recent update loop. </summary>
            virtual float updateTime() const = 0;

            /// <summary> Obtains the time in seconds since the game start. </summary>
            virtual float timeSinceStart() const = 0;

            /// <summary> Obtains the time scale currently being applied each frame. </summary>
            virtual float getTimeScale() const = 0;

            /// <summary> This sets the time scale applied to the real world frame times. This will not go below zero. </summary>
            virtual void setTimeScale (const float timeScale) = 0;

            #pragma endregion
    };   
}

#endif