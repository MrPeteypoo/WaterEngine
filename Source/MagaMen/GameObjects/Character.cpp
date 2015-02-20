#include "Character.hpp"


// Engine headers.
#include <Engine/Interfaces/ITime.hpp>


// Game headers.
#include <MagaMen/States/MagaMenLevelState.hpp>


// Game namespace.
namespace mm
{
    #pragma region Static data

    MagaMenLevelState*  Character::m_state = nullptr;
    water::SoundID      Character::m_collideSound = 0;
    water::SoundID      Character::m_fireSound = 0;
    water::SoundID      Character::m_deathSound = 0;

    #pragma endregion


    #pragma region Constructors

    Character::Character (Character&& move)
    {
        *this = std::move (move);
    }


    Character& Character::operator= (Character&& move)
    {
        if (this != &move)
        {
            Collidable::operator= (std::move (move));

            m_collideCD = move.m_collideCD;
            m_fireCD    = move.m_fireCD;

            move.m_collideCD = 0.f;
            move.m_fireCD    = 0.f;
        }

        return *this;
    }

    #pragma endregion


    #pragma region Firing

    void Character::fireBullet (const Vector2<float>& position, const Vector2<float>& velocity)
    {
        // Spawn a bullet and fill it with the required information!
        //auto& bullet = m_state->spawnBullet();

        // Prepare the bullet before letting the game handle it.
        //bullet.m_collider.setLayer (m_collider.getLayer());
        //bullet.setPosition (position);
        //bullet.setVelocity (velocity);
        //bullet.setPower (m_power);
    }


    void Character::cooldown()
    {
        const auto dt = time().getDelta();

        m_collideCD = util::max (m_collideCD - dt, 0.f);
        m_fireCD    = util::max (m_fireCD - dt, 0.f);
    }

    #pragma endregion


    #pragma region Other

    void Character::onCollision (PhysicsObject* const collision)
    {
        // If it is the player then cause them to take damage.
        if (collision->getTag() == "Player" || collision->getTag() == "Enemy")
        {
            auto const object = (Character*) collision;
            
            // Check if we can hurt them.
            if (!object->m_collideCD == 0.f)
            {
                object->damage (m_power);
                object->m_collideCD = 0.125f;

                // Play a sound to indicate that we've collided with them.
                audio().playSound (m_collideSound);
            }
        }
    }

    
    void Character::onDeath()
    {
        audio().playSound (m_deathSound);
    }

    #pragma endregion
}