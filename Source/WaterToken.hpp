#if !defined WATER_TOKEN_INCLUDED
#define WATER_TOKEN_INCLUDED


// STL headers.
#include <memory>


// Engine headers.
#include <Engine/WaterEngineForward.hpp>


// Third party headers.
#include <HAPI/HAPI_lib.h>


// Game namespace.
namespace wt
{
    /// <summary>
    /// A basic encapsulation class for the game loop process, the handling of deltaTime and other main loop activities.
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

            #pragma region Member variables

            std::unique_ptr<water::Engine>  m_engine    { nullptr };    //!< The engine used in the game.

            #pragma endregion

    };
}

void HAPI_Main()
{
    // Create and run water token!
    wt::Game game { };
    game.run();
}


#endif