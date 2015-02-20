#if !defined WATER_PHYSICS_OBJECT_INCLUDED
#define WATER_PHYSICS_OBJECT_INCLUDED


// Engine headers.
#include <GameComponents/Collider.hpp>
#include <GameComponents/GameObject.hpp>


// Engine namespace.
namespace water
{
    /// <summary>
    /// An abstract class for physics objects. Any object which the physics system manages must inherit from this class.
    /// PhysicsObject's can be added to a GameState's collection of managed objects which will be given to the physics
    /// system every physicUpdate(), this will apply collision detection as well as other features of the system.
    /// </summary>
    class PhysicsObject : public GameObject
    {
        public:

            #pragma region Constructors and destructor
            
            PhysicsObject()                                         = default;
            PhysicsObject (const PhysicsObject& copy)               = default;
            PhysicsObject& operator= (const PhysicsObject& copy)    = default;

            PhysicsObject (PhysicsObject&& move);
            PhysicsObject& operator= (PhysicsObject&& move);

            // Ensure destructor is virtual.
            virtual ~PhysicsObject() override {}

            #pragma endregion


            #pragma region Collision

            /// <summary> 
            /// This function is called every time two collision objects intersect. This will be called AFTER the objects have been moved
            /// by the physics system and will be called on both objects.
            /// </summary>
            /// <param name="collision"> The object being collided with. </param>
            virtual void onCollision (PhysicsObject* const collision) = 0;

            /// <summary>
            /// The function called on trigger objects when either another trigger object or a collision object intersects the trigger zone.
            /// This will be called every frame until the object leaves.
            /// </summary>
            /// <param name="collision"> The object intersecting the trigger zone. </param>
            virtual void onTrigger (PhysicsObject* const collision) = 0;

            #pragma endregion


            #pragma region Getters and setters

            /// <summary> Indicates whether the PhysicsObject is static or not. </summary>
            bool isStatic() const                   { return m_isStatic; }

            /// <summary> 
            /// Obtain a reference to the collider of the physics object. This contains information relating to how the physics
            /// object should be handled by the engine.
            /// </summary>
            /// <returns> A reference to the collider. </returns>
            const Collider& getCollider() const     { return m_collider; }

            /// <summary> Sets whether the PhysicsObject is static. If they're static they will not be moved by the physics system. </summary>
            /// <param name="isStatic"> Whether it should be static. </param>
            void setStatic (const bool isStatic)    { m_isStatic = isStatic; }

            #pragma endregion

        private:

            #pragma region Implementation data
            
            Collider    m_collider  { };        //!< The collision information of the PhysicsObject.
            bool        m_isStatic  { true };   //!< Determines whether collision should cause this object to move or not.

            #pragma endregion
    };
}


#endif