#include "Player.hpp"


// Engine headers.
#include <Engine/Interfaces/IInput.hpp>
#include <Engine/Interfaces/ITime.hpp>


// Game headers.
#include <MagaMen/Misc/Enums.hpp>


// Game namespace.
namespace mm
{
    #pragma region Constructors

    Player::Player (Player&& move)
    {
        *this = std::move (move);
    }


    Player& Player::operator= (Player&& move)
    {
        if (this != &move)
        {
            Character::operator= (std::move (move));

            m_facingLeft        = move.m_facingLeft;
            move.m_facingLeft   = false;
        }

        return *this;
    }

    #pragma endregion
    

    #pragma region Game functionality

    bool Player::initialise()
    {
        // GameObject data.
        m_position  = { 4.f, 7.f };
        m_velocity  = { 0.f, 0.f };
        m_frame     = { 0, 0 };
        m_blendType = water::BlendType::Transparent;
        m_name      = "Player";
        m_tag       = "Player";

        // PhysicsObject data.
        water::Collider collider { };
        
        collider.setLayer   ((unsigned int) mm::CollisionLayer::Player);
        collider.setTrigger (false);
        collider.setBox     ({ 0.1f, 0.1f, 1.9f, 1.9f});
        m_isStatic = false;

        // Collidable data.
        m_speed     = 7.f;
        m_currentHP = 100;
        m_maxHP     = 100;
        m_power     = 10;

        // Character data.
        m_collideCD = 0.f;
        m_fireCD    = 0.f;

        return true;
    }


    void Player::updatePhysics()
    {
        // The velocity has already had deltaTime applied so don't apply it again.
        m_position += m_velocity;

        // Reset the velocity to support instant stopping.
        m_velocity.setPosition (0, 0);
    }


    void Player::update()
    {
        // Cool any remaining timers.
        cooldown();
        
        // Process input.
        movementInput();
        otherInput();

        // Set the correct animation frame. If firing the X should be 1. The left-looking sprites on the spritesheet are 0 on the Y axis.
        m_frame.x = m_fireCD > 0.f;
        m_frame.y = m_facingLeft;
    }

    #pragma endregion


    #pragma region Internal workings

    void Player::movementInput()
    {
        // Start by adding the controller and keyboard input together.
        Vector2<float> direction { 0, 0 };

        if (input().getActionPressed ((int) Action::Left))
        {
            direction.x -= 1.f;
        }

        if (input().getActionPressed ((int) Action::Right))
        {
            direction.x += 1.f;
        }

        if (input().getActionPressed ((int) Action::Up))
        {
            direction.y -= 1.f;
        }

        if (input().getActionPressed ((int) Action::Down))
        {
            direction.y += 1.f;
        }

        direction.x += input().getActionAxis ((int) Action::Right);
        direction.y += input().getActionAxis ((int) Action::Down);


        // Normalise the direction to blend the controller and keyboard input then increase the velocity.
        m_velocity += direction.normalised() * m_speed * time().getDelta();
        
        // If the input direction is positive then we should face right.
        if (direction.x != 0.f)
        {
            m_facingLeft = direction.x < 0.f;
        }
    }


    void Player::otherInput()
    {
        // Shooting!
        if (m_fireCD == 0.f && input().getActionPressed ((int) Action::Shoot))
        {
            // If we're facing right we need to spawn the object with an offset on the X axis.
            const auto offset   = Vector2<float> { m_facingLeft ? 0.f : 2.f, 0.5f };
            const auto velocity = Vector2<float> { m_facingLeft ? -5.f : 5.f, 0.f };

            fireBullet (m_position + offset, velocity);
            m_fireCD = 0.125f;
            audio().playSound (m_fireSound);
        }
    }

    #pragma endregion
}