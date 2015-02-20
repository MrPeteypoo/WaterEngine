#if !defined MAGA_MEN_CHARACTER_INCLUDED
#define MAGA_MEN_CHARACTER_INCLUDED


// Engine headers.
#include <Engine/Interfaces/IAudio.hpp>


// Game headers.
#include <MagaMen/GameObjects/Collidable.hpp>


// Game namespace.
namespace mm
{
    // Forward declarations.
    class MagaMenLevelState;


    /// <summary>
    /// The class which contains the player, everything to do with the player is handled here.
    /// </summary>
    class Character : public Collidable
    {
        public:

            #pragma region Constructors and destructor

            Character()                                     = default;
            Character (const Character& copy)               = default;
            Character& operator= (const Character& copy)    = default;

            Character (Character&& move);
            Character& operator= (Character&& move);

            // Ensure destructor is virtual.
            ~Character() override {}

            #pragma endregion


            #pragma region Collision

            /// <summary> Takes damage any environmental object it hits. </summary>
            virtual void onCollision (PhysicsObject* const collision) override;

            #pragma endregion

        protected:

            #pragma region Firing

            /// <summary> Fires a bullet at the given position with the desired velocity with the current layer. </summary>
            void fireBullet (const Vector2<float>& position, const Vector2<float>& velocity);

            /// <summary> Reduces the cooldown values of the character based on delta time. </summary>
            virtual void cooldown();

            #pragma endregion


            #pragma region Implementation data
            
            static MagaMenLevelState*   m_state;                    //!< A pointer to the state associated with the character.
            static water::SoundID       m_fireSound;                //!< The ID of the sound to play when firing bullets.
            static water::SoundID       m_collideSound;             //!< The ID of the sound to play when characters collide with something.

            float                       m_collideCD     { 0 };      //!< The cooldown left before the character can be hurt from collision again.
            float                       m_fireCD        { 0 };      //!< The cooldown left before the character can fire again.

            #pragma endregion
    };
}


#endif