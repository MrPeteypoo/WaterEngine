#if !defined WATER_INTERFACE_TIME_ENGINE_INCLUDED
#define WATER_INTERFACE_TIME_ENGINE_INCLUDED


// Engine headers.
#include <Interfaces/ITime.hpp>


// Engine namespace.
namespace water
{
    /// <summary>
    /// An interface for the engine to perform system-critical functions to ITime systems. Logging systems
    /// will not throw exceptions on initialisation, instead they will use a boolean flag to inform the application
    /// that something has gone wrong, therefore, it should be closed.
    /// </summary>
    class IEngineTime : public ITime
    {
        public:

            // Ensure destructor is virtual since this is an interface.
            virtual ~IEngineTime() override {}


            ///////////////////////
            // System management //
            ///////////////////////

            /// <summary> Initialise the time system, preparing it for usage. </summary>
            /// <param name="physicsFPS"> The FPS which physics updates should be capped at. </param>
            /// <param name="updateFPS"> The FPS other systems should update at. 0 means unlimited. </param>
            /// <param name="minFPS"> The minimum FPS before slowdown occurs. This effectively caps the max tick rate of the engine. </param>
            /// <returns> Whether the initialisation was successful. </returns>
            virtual void initialise (const unsigned int physicsFPS, const unsigned int updateFPS, const unsigned int minFPS) = 0;

            /// <summary> Causes physics update to become the active context and updates the physics delta time. </summary>
            /// <returns> Whether an update should be performed on physics systems this frame. </returns>
            virtual bool updatePhysics() = 0;

            /// <summary> Causes update to become the active context and updates the standard delta time. </summary>
            /// <returns> Whether an update should be performed on game objects. </returns>
            virtual bool update() = 0;

            /// <summary> Causes the time system to perform end-of-frame actions such as decrementing accumulators. </summary>
            virtual void endFrame() = 0;

            /// <summary> Force the time class to reset the start time, this is useful to avoid initialisation effecting time values. </summary>
            virtual void resetTime() = 0;
    };
}

#endif
