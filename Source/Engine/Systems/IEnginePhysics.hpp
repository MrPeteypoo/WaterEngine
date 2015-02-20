#if !defined WATER_INTERFACE_PHYSICS_ENGINE_INCLUDED
#define WATER_INTERFACE_PHYSICS_ENGINE_INCLUDED


// STL headers.
#include <vector>


// Engine headers.
#include <Interfaces/IPhysics.hpp>


// Engine namespace.
namespace water
{
    // Forward declarations.
    class PhysicsObject;


    /// <summary>
    /// An interface for the engine to provide physics systems with collidable objects to check.
    /// </summary>
    class IEnginePhysics : public IPhysics
    {
        public:

            // Ensure destructor is virtual since this is an interface.
            virtual ~IEnginePhysics () override {}


            #pragma region System management
    
            /// <summary> Initialise the system, preparing it for checking collisions. </summary>
            virtual void initialise() = 0;

            /// <summary> Checks for collisions in all given PhysicsObject's. </summary>
            /// <param name="objects"> The objects to check collision for. </param>
            virtual void detectCollisions (const std::vector<PhysicsObject*>& objects) = 0;

            #pragma endregion
    };   
}

#endif