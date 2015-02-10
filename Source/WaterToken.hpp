#if !defined WATER_TOKEN_INCLUDED
#define WATER_TOKEN_INCLUDED


// Engine headers.
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

            /// <summary> 
            /// Sets up HAPI and the various different systems, ready for use.
            /// <returns> Whether the initialisation was successful or not. </returns>
            /// </summary>
            bool initialise();

            /// <summary> Will update each deltaTime value, ready for usage by the different aspects of the game. </summary>
            void updateDeltaTime();

            /// <summary> The update function which handles all systems limited to 60FPS. </summary>
            void updateCapped();

            /// <summary> The main update function which gets called each frame. </summary>
            void updateMain();

            /// <summary> Renders each object to the screen. </summary>
            void renderAll();

            #pragma endregion


            #pragma region Member variables

            /*int                                     m_screenWidth       { 1024 };        //!< The width of the screen in pixels.
            int                                     m_screenHeight      { 1024 };        //!< The height of the screen in pixels.

            DWORD                                   m_oldTime           { 0 };          //!< The previous time in milliseconds, used to calculate delta time.
            DWORD                                   m_currentTime       { 0 };          //!< The current time in milliseconds, used to calculate delta time.

            float                                   m_deltaTime         { 0.f };        //!< The deltaTime calculated each frame.
            float                                   m_sixtyFPSDeltaTime { 0.f };        //!< The deltaTime used to limit certain actions such as input to 60FPS.

            bool                                    m_controllerOn      { false };      //!< Indicates whether a controller is plugged in and turned on.

            Controller                              m_controller        {  };           //!< A cache of controller input, updated every 60 FPS.
            Keyboard                                m_keyboard          {  };           //!< A cache of keyboard input, updated every 60 FPS.

            Rectangle<float>                        m_centreZone        {  };           //!< The centre point of the screen.

            std::vector<std::unique_ptr<IEntity>>   m_entities          {  };           //!< A vector containing every entity in the game.

            std::shared_ptr<water::IRenderer>       m_renderer          { nullptr };    //!< The renderer used to draw everything onto the screen.
            std::shared_ptr<water::IAudio>          m_audio             { nullptr };    //!< CRAPPAPAPDWAKDWAJKJSEFJFSEJFS.
            std::vector<water::SoundID>                    m_sounds            { };*/

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