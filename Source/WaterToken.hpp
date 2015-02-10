#if !defined WATER_TOKEN_INCLUDED
#define WATER_TOKEN_INCLUDED


// STL headers.
#include <memory>


// Engine headers.
#include <EngineForward.hpp>
#include <Audio/IAudioEngine.hpp>
#include <Logging/ILoggerEngine.hpp>
#include <Rendering/IRendererEngine.hpp>
#include <Time/ITimeEngine.hpp>


// Third party headers.
#include <HAPI/HAPI_lib.h>


// Forward declarations.
class IEntity;
namespace water 
{ 
    class IAudio;
    class IRenderer;
    using SoundID = size_t;
}

using Controller    = HAPI_TControllerData;
using Keyboard      = HAPI_TKeyboardData;


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

            #pragma region Engine functionality

            /// <summary> Sets up HAPI and the various different systems, ready for use. </summary>
            /// <returns> Whether the initialisation was successful or not. </returns>
            bool initialise();

            /// <summary> The update function which handles all systems limited to 60FPS. </summary>
            void physicsUpdate();

            /// <summary> The main update function which gets called each frame. </summary>
            void update();

            /// <summary> Renders each object to the screen. </summary>
            void render();

            #pragma endregion


            #pragma region Member variables

            int                                     m_width         { 1024 };       //!< The width of the screen in pixels.
            int                                     m_height        { 1024 };       //!< The height of the screen in pixels.

            bool                                    m_controllerOn  { false };      //!< Indicates whether a controller is plugged in and turned on.

            Controller                              m_controller    {  };           //!< A cache of controller input, updated every 60 FPS.
            Keyboard                                m_keyboard      {  };           //!< A cache of keyboard input, updated every 60 FPS.
            
            std::shared_ptr<water::IAudioEngine>    m_audio         { nullptr };
            std::shared_ptr<water::ILoggerEngine>   m_logger        { nullptr };
            std::shared_ptr<water::IRendererEngine> m_renderer      { nullptr };
            std::shared_ptr<water::ITimeEngine>     m_time          { nullptr };


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