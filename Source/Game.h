#if !defined GAME_INCLUDED
#define GAME_INCLUDED


// Engine headers.
#include <HAPI_lib.h>
#include <Misc/Texture.h>
#include <ScreenManager/ScreenManager.h>


// Alias declarations.
using Keyboard = HAPI_TKeyboardData;



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

        int             m_screenWidth       { 256 };    //!< The width of the screen in pixels.
        int             m_screenHeight      { 256 };    //!< The height of the screen in pixels.

        DWORD           m_oldTime           { 0 };      //!< The previous time in milliseconds, used to calculate delta time.
        DWORD           m_currentTime       { 0 };      //!< The current time in milliseconds, used to calculate delta time.

        float           m_deltaTime         { 0.f };    //!< The deltaTime calculated each frame.
        float           m_sixtyFPSDeltaTime { 0.f };    //!< The deltaTime used to limit certain actions such as input to 60FPS.

        Keyboard        m_keyboard          { };        //!< A cache of keyboard input, updated every 60 FPS.

        Texture         m_background        { };        //!< The static background for milestone 2.
        Texture         m_circle            { };        //!< The alpha-blended circle for milestone 2.
        
        float           m_circleX           { 0.f };    //!< The X position for the circle.
        float           m_circleY           { 0.f };    //!< The Y position for the cicle.

        ScreenManager   m_screenManager     { };        //!< The screen manager used to draw everything onto the screen.

        #pragma endregion

};


void HAPI_Main()
{
    // Create and run the game.
    Game game { };
    game.run();
}


float min (const float a, const float b)
{
    return a < b ? a : b;
}


float max (const float a, const float b)
{
    return a > b ? a : b;
}

#endif // GAME_INCLUDED