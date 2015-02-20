#if !defined MAGA_MEN_MAIN_MENU_STATE_INCLUDED
#define MAGA_MEN_MAIN_MENU_STATE_INCLUDED


// STL headers.
#include <vector>


// Game headers.
#include <MagaMen/States/MagaMenState.hpp>
#include <MagaMen/GameObjects/StaticObject.hpp>


// Game namespace.
namespace mm
{
    /// <summary>
    /// The main menu of Maga Men. This is the entry point of the game where the title screen is shown, waiting for the player to start the game.
    /// </summary>
    class MainMenuState : public MagaMenState
    {
        public:

            #pragma region Constructors and destructor

            /// <summary> A simple constructor which specifies the .XML file to load the states data from. </summary>
            /// <param name="loadFrom"> An .XML file containing information the state needs to load from. </param>
            MainMenuState (const std::string& loadFrom) : MagaMenState (loadFrom) { }

            MainMenuState (MainMenuState&& move);
            MainMenuState& operator= (MainMenuState&& move);
            ~MainMenuState() override final { }
            
            MainMenuState (const MainMenuState& copy)               = delete;
            MainMenuState& operator= (const MainMenuState& copy)    = delete;

            #pragma endregion


            #pragma region Game flow

            /// <summary> Loads all of the graphics required by the title screen. Also clears previous data. </summary>
            /// <returns> Whether the loading was successful or not. </returns>
            bool onAdd() override final;

            /// <summary> Removes all graphical and audio data from the game. </summary>
            /// <returns> If something went wrong and an error should be flagged. </returns>
            bool onRemove() override final;

            /// <summary> Plays the title music. </summary>
            void onEntry() override final;

            /// <summary> Stops the music from playing. </summary>
            void onExit() override final;

            /// <summary> Absolutely nothing. </summary>
            void updatePhysics() override final { }

            /// <summary> Checks if the player is ready to start the game or not. </summary>
            void update() override final;

            /// <summary> Renders the entire title screen! </summary>
            void render() override final;

            #pragma endregion

        private:

            #pragma region Implementation data

            // Allow the construction of the state externally.
            friend class StateBuilder;

            std::vector<StaticObject> m_objects { };    //!< The objects managed by the MainMenuState.

            #pragma endregion
    };
}


#endif