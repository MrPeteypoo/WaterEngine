#if !defined WATER_INTERFACE_TIME_ENGINE_INCLUDED
#define WATER_INTERFACE_TIME_ENGINE_INCLUDED


// Engine headers.
#include <ITime.hpp>


// Engine namespace.
namespace water
{
    /// <summary>
    /// An interface for the engine to perform system-critical functions to ITime systems.
    /// </summary>
    class ITimeEngine : public ITime
    {
        public:

            // Ensure destructor is virtual since this is an interface.
            virtual ~ITimeEngine() {}


            #pragma region System management
            
            /// <summary> Initialise the time system, preparing it for usage. </summary>
            /// <param name="physicsFPS"> The FPS which physics updates should be capped at. </param>
            /// <param name="standardFPS"> The FPS other systems should update at. 0 means unlimited. </param>
            /// <returns> Whether the initialisation was successful. </returns>
            virtual bool initialise (const size_t physicsFPS, const size_t standardFPS) = 0;

            /// <summary> Causes physics update to become the active context and updates the physics delta time. </summary>
            virtual void physicsUpdate() = 0;

            /// <summary> Causes update to become the active context and updates the standard delta time. </summary>
            virtual void update() = 0;

            #pragma endregion
    };   
}

#endif