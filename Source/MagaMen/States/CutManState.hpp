#if !defined MAGA_MEN_CUT_MAN_STAGE_INCLUDED
#define MAGA_MEN_CUT_MAN_STAGE_INCLUDED


// Game headers.
#include <MagaMen/States/MagaMenLevelState.hpp>
#include <MagaMen/GameObjects/StaticObject.hpp>
#include <MagaMen/GameObjects/Player.hpp>


// Game namespace.
namespace mm
{
    /// <summary>
    /// The level where the player has to defeat Cut Man. This is where the majority of gameplay is.
    /// </summary>
    class CutManState final : public MagaMenLevelState
    {
        public:

            #pragma region Constructors and destructor

            /// <summary> A simple constructor which specifies the .XML file to load the states data from. </summary>
            /// <param name="loadFrom"> An .XML file containing information the state needs to load from. </param>
            CutManState (const std::string& loadFrom) : MagaMenLevelState (loadFrom) { }

            CutManState (CutManState&& move);
            CutManState& operator= (CutManState&& move);
            ~CutManState() override final { }
            
            CutManState (const CutManState& copy)               = delete;
            CutManState& operator= (const CutManState& copy)    = delete;

            #pragma endregion


            #pragma region Game flow

            /// <summary> Loads all of the graphics required by the title screen. Also clears previous data. </summary>
            /// <returns> Whether the loading was successful or not. </returns>
            bool onAdd() override final;

            /// <summary> Removes all graphical and audio data from the game. </summary>
            /// <returns> If something went wrong and an error should be flagged. </returns>
            bool onRemove() override final;

            /// <summary> Plays the background music and resets the state. </summary>
            void onEntry() override final;

            /// <summary> Stops the music from playing. </summary>
            void onExit() override final;

            /// <summary> Updates all collidable objects. </summary>
            void updatePhysics() override final;

            /// <summary> Checks which objects are still alive and scrolls the background/ </summary>
            void update() override final;

            /// <summary> Renders every object in the scene. </summary>
            void render() override final;

            #pragma endregion


            #pragma region Level management

            /// <summary> Informs the level that the player has died. </summary>
            /*void playerDead() override final;

            /// <summary> Increment the players score by the value given. </summary>
            /// <param name="value"> The amount to increase the players score by. </param>
            void increaseScore (const int value) override final;

            /// <summary> Spawns a bullet in the level which is returned by the function. </summary>
            /// <returns> A usable bullet. </returns>
            Bullet& spawnBullet() override final;

            /// <summary> Spawns an explosion in the level which is returned by the function. </summary>
            /// <returns> Explosions are non-collidable graphical effects, usually this happens when an object is destroyed. </returns>
            Explosion& spawnExplosion() override final;*/

            #pragma endregion

        private:

            #pragma region Implementation data

            // Allow the construction of the state externally.
            friend class StateBuilder;

            water::TextureID    m_background    { 0 };      //!< The scrolling background texture.
            Vector2<float>      m_bgPosition    { 0, 0 };   //!< The current position of the scrolling background.

            bool                m_paused        { false };  //!< A flag which indicates whether the game is paused or not.
            int                 m_score         { 0 };      //!< Keeps track of the score of the player.
            
            StaticObject        m_lives         { };        //!< A GUI object displaying the current lives of the player.
            Player              m_player        { };        //!< The player.

            #pragma endregion
    };
}


#endif