#if !defined MAGA_MEN_COLLIDABLE_INCLUDED
#define MAGA_MEN_COLLIDABLE_INCLUDED


// Engine headers.
#include <Engine/GameComponents/PhysicsObject.hpp>


// Game namespace.
namespace mm
{
    /// <summary> 
    /// A base class for every collidable object in the game. Provides functionality required to make use of the water engines physics system.
    /// </summary>
    class Collidable : public water::PhysicsObject
    {
        public:
            
            #pragma region Constructors and destructor
            
            Collidable()                                    = default;
            Collidable (const Collidable& copy)             = default;
            Collidable& operator= (const Collidable& copy)  = default;

            Collidable (Collidable&& move);
            Collidable& operator= (Collidable&& move);

            // Ensure destructor is virtual.
            virtual ~Collidable() override {}

            #pragma endregion


            #pragma region Game functionality

            /// <summary> Moves the object based on its current velocity. </summary>
            virtual void updatePhysics() override;

            /// <summary> By default collidable objects will use interpolation to create smooth graphics at any frame rate. </summary>
            virtual void render() override;

            #pragma endregion


            #pragma region Movement

            /// <summary> Get the movement speed of the object in units-per-second. </summary>
            float getMovementSpeed() const  { return m_speed; }

            /// <summary> Set the maximum movement speed of the object in units-per-second. This cannot be lower than zero. </summary>
            void setMovementSpeed (const float speed);

            #pragma endregion


            #pragma region Health properties

            /// <summary> Tests if the object is dead or not. </summary>
            bool isDead() const             { return m_currentHP == 0; }

            /// <summary> Get the current HP value of the object. </summary>
            int getCurrentHP() const        { return m_currentHP; }

            /// <summary> Get the maximum HP value of the object. </summary>
            int getMaximumHP() const        { return m_currentHP; }

            /// <summary> Get the power value of the object. </summary>
            int getPower() const            { return m_power; }

            /// <summary> Set the current HP value of the object. </summary>
            /// <param name="hp"> The new current HP of the object, will be clamped between 0 and max. </param>
            void setCurrentHP (const int hp);

            /// <summary> Set the maximum HP value of the object, the current HP value will be clamped accordingly. </summary>
            /// <param name="hp"> The new maximum HP value, this can not be below 1. </param>
            void setMaximumHP (const int hp);

            /// <summary> Set the power of the object. </summary>
            /// <param name="power"> This can be negative, if so then the damage function will heal instead. <param>
            void setPower (const int power) { m_power = power; }

            /// <summary> Causes the object to recover HP, this will be clamped to 0 and max. </summary>
            /// <param name="recovery"> How much HP to be recovered. </param>
            void cure (const int recovery)  { setCurrentHP (m_currentHP + recovery); }

            /// <summary> Causes the object to lose HP, this will be clamped to 0 and max. </summary>
            /// <param name="damage"> How much HP should be lost. </param>
            void damage (const int damage)  { setCurrentHP (m_currentHP - damage); }

            #pragma endregion

        protected:

            #pragma region Implementation data

            float   m_speed     { 0 };  //!< The movement speed of the object.

            int     m_currentHP { 0 };  //!< The current HP value of the collidable object.
            int     m_maxHP     { 0 };  //!< The maximum HP value of the collidable object.
            int     m_power     { 0 };  //!< The maximum amount of inflictable damage.

            #pragma endregion
    };
}


#endif