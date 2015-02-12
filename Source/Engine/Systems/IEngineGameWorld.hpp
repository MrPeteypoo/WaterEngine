#if !defined WATER_INTERFACE_WORLD_ENGINE_INCLUDED
#define WATER_INTERFACE_WORLD_ENGINE_INCLUDED


// STL headers.
#include <vector>


// Engine headers
#include <Interfaces/IGameWorld.hpp>


// Engine namespace.
namespace water
{
    // Forward declarations.
    class PhysicsObject;


    /// <summary>
    /// An interface for the engine to perform system-critical functions the world system.
    /// </summary>
    class IEngineGameWorld : public IGameWorld
    {
        public:

            // Ensure destructor is virtual since this is an interface.
            virtual ~IEngineGameWorld() {}


            #pragma region System management

            /// <summary> Perform a physics update on the active state. </summary>
            virtual void updatePhysics() = 0;

            /// <summary> Perform an update call on the active state. </summary>
            virtual void update() = 0;

            /// <summary> Render the active state. </summary>
            virtual void render() = 0;

            /// <summary> Allow the game world to proceed with any queued tasks at the end of a frame. </summary>
            virtual void processQueue() = 0;

            #pragma endregion


            #pragma region Getters

            /// <summary> Tests if the stack is empty. If so then the engine knows that it can exit. </summary>
            /// <returns> Whether the stack is empty. </returns>
            virtual bool isStackEmpty() const = 0;

            /// <summary> Obtain the collection of physics objects in the active state. </summary>
            /// <returns> The vector of objects. </returns>
            virtual const std::vector<PhysicsObject*>& getPhysicsObjects() const = 0;

            #pragma endregion
    };
}

#endif