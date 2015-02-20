#if !defined MAGA_MEN_LEVEL_STATE_INCLUDED
#define MAGA_MEN_LEVEL_STATE_INCLUDED


// Game headers.
#include <MagaMen/States/MagaMenState.hpp>


// Game namespace.
namespace mm
{
    class Bullet;
    class Explosion;


    /// <summary>
    /// The base class of all level states in the game, provides common functionality required for level management and the objects that exist inside it.
    /// </summary>
    class MagaMenLevelState : public MagaMenState
    {
        public:

            #pragma region Constructors and destructor

            /// <summary> A simple constructor which specifies the .XML file to load the states data from. </summary>
            /// <param name="loadFrom"> An .XML file containing information the state needs to load from. </param>
            MagaMenLevelState (const std::string& loadFrom) : MagaMenState (loadFrom) { }

            MagaMenLevelState()                                             = default;

            MagaMenLevelState (MagaMenLevelState&& move);
            MagaMenLevelState& operator= (MagaMenLevelState&& move);
            ~MagaMenLevelState() override { }
            
            MagaMenLevelState (const MagaMenLevelState& copy)               = delete;
            MagaMenLevelState& operator= (const MagaMenLevelState& copy)    = delete;

            #pragma endregion


            #pragma region Level management

            /// <summary> Informs the level that the player has died. </summary>
            /*virtual void playerDead() = 0;

            /// <summary> Increase the current score of the player. </summary>
            virtual void increaseScore (const int value) = 0;

            /// <summary> Spawns a bullet in the level which is returned by the function. </summary>
            /// <returns> A usable bullet. </returns>
            virtual Bullet& spawnBullet() = 0;

            /// <summary> Spawns an explosion in the level which is returned by the function. </summary>
            /// <returns> Explosions are non-collidable graphical effects, usually this happens when an object is destroyed. </returns>
            virtual Explosion& spawnExplosion() = 0;*/

            #pragma endregion

        private:

    };
}


#endif