#if !defined MAGA_MEN_LEVEL_STATE_INCLUDED
#define MAGA_MEN_LEVEL_STATE_INCLUDED


// Game headers.
#include <MagaMen/States/MagaMenState.hpp>


// Game namespace.
namespace mm
{
    class Bullet;

    /// <summary>
    /// The base class of all level states in the game, provides common functionality required for level management and the objects that exist inside it.
    /// </summary>
    class MagaMenLevelState final : public MagaMenState
    {
        public:

            #pragma region Constructors and destructor

            /// <summary> A simple constructor which specifies the .XML file to load the states data from. </summary>
            /// <param name="loadFrom"> An .XML file containing information the state needs to load from. </param>
            MagaMenLevelState (const std::string& loadFrom) : MagaMenState (loadFrom) { }

            MagaMenLevelState (MagaMenLevelState&& move);
            MagaMenLevelState& operator= (MagaMenLevelState&& move);
            ~MagaMenLevelState() override final { }
            
            MagaMenLevelState (const MagaMenLevelState& copy)               = delete;
            MagaMenLevelState& operator= (const MagaMenLevelState& copy)    = delete;

            #pragma endregion


            #pragma region Level management

            //void kill

            /// <summary> Spawns a bullet in the level which is returned by the function. </summary>
            /// <returns> A usable bullet. </returns>
            Bullet& spawnBullet();

            #pragma endregion

        private:

            #pragma region Implementation data

            // Allow the construction of the state externally.
            friend class CutManStateBuilder;

            //std::vector<

            #pragma endregion
    };
}


#endif