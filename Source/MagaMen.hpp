#if !defined MAGA_MEN_INCLUDED
#define MAGA_MEN_INCLUDED


// STL headers.
#include <memory>


// Engine headers.
#include <Engine/WaterEngineForward.hpp>


// Third party headers.
#include <HAPI/HAPI_lib.h>


/// <summary>
/// The namespace of which all Maga Men related components exist. This includes all game states, game objects, AI, etc. that are specific to the game.
/// </summary>
namespace mm
{
    // Forward declarations.
    class MainMenuState;


    /// <summary>
    /// The entry point for the Maga Men game! Maga Men is effectively a small demo of the Water engine in the spirit of a Mega Man clone.
    /// </summary>
    class Game final
    {
        public: 
        
            #pragma region Constructors and destructor

            // Use default construction and destruction.
            Game()                              = default;
            ~Game()                             = default;

            // Delete all other constructors.
            Game (const Game& copy)             = delete;
            Game& operator= (const Game& copy)  = delete;

            Game (Game&& move)                  = delete;
            Game& operator= (Game&& move)       = delete;

            #pragma endregion

            /// <summary> Provides the entry point to the game. </summary>
            void run();
        

        private:

            #pragma region Internal workings

            /// <summary> Attempt to create the game states. </summary>
            void createStates();

            /// <summary> Adds each created state to the game world, ready for playing! </summary>
            void addStatesToWorld();

            #pragma endregion


            #pragma region Member variables

            std::unique_ptr<water::Engine>      m_engine    { nullptr };    //!< The engine used in the game.
            std::shared_ptr<MainMenuState>      m_mainMenu  { nullptr };    //!< The entry point of the game, this displays the title screen.

            #pragma endregion

    };
}

/// <summary> The entry point of the HAPI application. </summary>
void HAPI_Main()
{
    // Create and run water token!
    mm::Game game { };
    game.run();
}


#endif