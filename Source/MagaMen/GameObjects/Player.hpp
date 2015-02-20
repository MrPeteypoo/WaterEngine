#if !defined MAGA_MEN_PLAYER_INCLUDED
#define MAGA_MEN_PLAYER_INCLUDED


// Game headers.
#include <MagaMen/GameObjects/Character.hpp>


// Game namespace.
namespace mm
{
    /// <summary>
    /// The class which contains the player, everything to do with the player is handled here.
    /// </summary>
    class Player final : public Character
    {
        public:

            #pragma region Constructors and destructor

            Player()                                = default;
            Player (const Player& copy)             = default;
            Player& operator= (const Player& copy)  = default;

            Player (Player&& move);
            Player& operator= (Player&& move);

            // Ensure destructor is virtual.
            ~Player() override final {}

            #pragma endregion


            #pragma region Game functionality

            /// <summary> Prepares the players collision data and such. </summary>
            bool initialise() override final;

            /// <summary> Moves the player based on its velocity which gets reset afterwards. </summary>
            void updatePhysics() override final;

            /// <summary> Checks for input and moves the player accordingly. </summary>
            void update() override final;

            /// <summary> Spawns an explosion at the point where the player died. </summary>
            void onDeath() override final;

            #pragma endregion


            #pragma region Getters

            /// <summary> Tests whether the player has lost all of their lives and has no remaining HP. </summary>
            bool isGameOver() const { return m_lives == 0 && m_currentHP == 0; }

            /// <summary> Obtains the number of lives the player currently has. </summary>
            int getLives() const    { return m_lives; }

            #pragma endregion


        private:

            #pragma region Internal workings

            /// <summary> Checks keyboard and controller input and calculates the desired direction of movement. </summary>
            void movementInput();

            /// <summary> Fires bullets if both possible and desired from the input. </summary>
            void otherInput();

            #pragma endregion

            bool    m_facingLeft    { false };  //!< Indicates whether the player is currently facing left or right.
            int     m_lives         { 5 };      //!< The number of lives the player has.
    };
}


#endif