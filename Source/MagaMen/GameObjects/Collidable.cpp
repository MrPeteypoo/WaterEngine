#include "Collidable.hpp"


// Engine headers.
#include <Engine/Interfaces/ITime.hpp>
#include <Engine/Utility/Maths.hpp>


// Game namespace.
namespace mm
{
    #pragma region Constructors and destructor

    Collidable::Collidable (Collidable&& move)
    {
        *this = std::move (move);
    }


    Collidable& Collidable::operator= (Collidable&& move)
    {
        if (this != &move)
        {
            PhysicsObject::operator= (std::move (move));

            m_currentHP = move.m_currentHP;
            m_maxHP     = move.m_maxHP;
            m_power     = move.m_power;

            // Reset primitives.
            move.m_currentHP = 0;
            move.m_maxHP     = 0;
            move.m_power     = 0;
        }

        return *this;
    }

    #pragma endregion


    #pragma region Game functionality

    void Collidable::render()
    {
        // Interpolate between the current and next frame based on the timestep between physics updates.
        const auto smoothPosition = Vector2<float>::lerp (m_position, m_position + m_velocity, time().getPhysicsStep());
        renderer().drawToScreen (smoothPosition, m_baseTexture, m_frame, m_blendType);
    }

    #pragma endregion


    #pragma region Health properties

    void Collidable::setCurrentHP (const int hp)
    {
        m_currentHP = util::clamp (hp, 0, m_maxHP);
    }


    void Collidable::setMaximumHP (const int hp)
    {
        // Ensure max HP doesn't go below zero.
        m_maxHP = util::max (hp, 0);

        // Reset the current HP value.
        setCurrentHP (m_currentHP);
    }

    #pragma endregion
}