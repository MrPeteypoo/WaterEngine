// STL headers.
#include <memory>


// Engine headers.
#include <HAPI_lib.h>
#include <Misc/Texture.h>
#include <ScreenManager/ScreenManager.h>



int screenWidth = 256, screenHeight = 256;
DWORD oldTime = 0, currentTime = 0;


/// <summary> Should only be called once a frame, returns the delta time as a float </summary>
const float calcDeltaTime()
{
    // Update the time figures.
    oldTime = currentTime;
    currentTime = HAPI->GetTime();

    // Convert from milliseconds to seconds.
    return (currentTime - oldTime) / 1000.f;
}


void HAPI_Main()
{
    if (HAPI->Initialise (&screenWidth, &screenHeight))
    {
        // Set up the screen
        auto screenManager = std::make_unique<ScreenManager> (ScreenManager (screenWidth, screenHeight));
        HAPI->SetShowFPS (true);

        // Set up the textures
        Texture background {}, foreground {};

        //if (background.loadTexture ())
        
        // Move and draw the stars
        while (HAPI->Update())
        {
            // Clear the screen
            screenManager->clearToBlack();

            
        }
    }
}