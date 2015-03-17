#include "PhysicsObject.hpp"


// Engine namespace.
namespace water
{
    /////////////////////////////////
    // Constructors and destructor //
    /////////////////////////////////

    PhysicsObject::PhysicsObject (PhysicsObject&& move)
    {
        *this = std::move (move);
    }


    PhysicsObject& PhysicsObject::operator= (PhysicsObject&& move)
    {
        if (this != &move)
        {
            GameObject::operator= (std::move (move));

            m_collider  = std::move (move.m_collider);
            m_isStatic  = move.m_isStatic;

            // Reset primitives.
            move.m_isStatic = false;
        }

        return *this;
    }
}
