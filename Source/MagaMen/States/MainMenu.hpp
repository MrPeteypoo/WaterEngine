#if !defined MAGA_MEN_MAIN_MENU_INCLUDED
#define MAGA_MEN_MAIN_MENU_INCLUDED


// Engine headers.
#include <Engine/GameComponents/GameState.hpp>
#include <Engine/Interfaces/IAudio.hpp>


// Game headers.
#include <MagaMen/States/StateIDs.hpp>


// Game namespace.
namespace mm
{
    /// <summary>
    /// The main menu of Maga Men. This is the entry point of the game where the title screen is shown, waiting for the player to start the game.
    /// </summary>
    class MainMenuState : public water::GameState
    {
        public:

            #pragma region Constructors and destructor

            MainMenuState();         
            MainMenuState (MainMenuState&& move);
            MainMenuState& operator= (MainMenuState&& move);            
            ~MainMenuState() override final { }
            
            MainMenuState (const MainMenuState& copy)               = delete;
            MainMenuState& operator= (const MainMenuState& copy)    = delete;

            #pragma endregion


            #pragma region Game flow

            /// <summary> Loads all of the graphics and sound required by the title screen. </summary>
            /// <returns> Whether the loading was successful or not. </returns>
            bool onAdd() override final;

            /// <summary> Removes any graphics and sound specific to the title screen. </summary>
            /// <returns> If something went wrong and an error should be flagged. </returns>
            bool onRemove() override final;

            /// <summary> Swaps to the title screen music. </summary>
            void onEntry() override final;

            /// <summary> Absolutely nothing. </summary>
            void onExit() override final { }

            /// <summary> Absolutely nothing. </summary>
            void updatePhysics() override final { }

            /// <summary> Checks if the player is ready to start the game or not. </summary>
            void update() override final;

            /// <summary> Renders the entire title screen! </summary>
            void render() override final;

            #pragma endregion

        private:

            #pragma region Implementation data

            std::string     m_loadFrom  = "";   //!< The file to load data from.
            water::SoundID  m_bgm       { };    //!< The ID of the background music.

            #pragma endregion
    };
}


#endif