// STL headers.
#include <memory>


// Engine headers.
#include <HAPI_lib.h>
#include <Misc/Texture.h>
#include <ScreenManager/ScreenManager.h>


const std::string   backgroundLocation  = "background.tga",
                    alphaThingLocation  = "alphaThing.tga";


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
        HAPI->SetShowFPS (true);

        // Set up the screen.
        auto screenManager = ScreenManager (screenWidth, screenHeight);
        screenManager.clearToBlack();

        // Set up the textures.
        Texture background { }, alphaThing { };
       
        if (!background.loadTexture (backgroundLocation) || !alphaThing.loadTexture (alphaThingLocation))
        {
            HAPI->UserMessage ("Unable to load texture.", "Error");
            HAPI->Close();
        }

        
        // Calculate boundaries now.
        const float boundaryLeft = 0.f,   boundaryRight = (float) (screenWidth - alphaThing.getWidth()),
                    boundaryUp = 0.f,     boundaryDown = (float) (screenHeight - alphaThing.getHeight());

        float movableX = screenWidth / 2.f - alphaThing.getWidth() / 2.f, 
              movableY = screenHeight / 2.f - alphaThing.getHeight() / 2.f;

        HAPI_TKeyboardData keyboard { };

        const float speed = 0.1f;
        
        // Move and draw the stars.
        while (HAPI->Update())
        {
            // Update keyboard data.
            HAPI->GetKeyboardData (&keyboard);

            // Handle keyboard input.
            if (keyboard.scanCode[HK_LEFT] && movableX > boundaryLeft)
            {
                movableX -= speed;
            }

            if (keyboard.scanCode[HK_UP] && movableY > boundaryUp)
            {
                movableY -= speed;
            }

            if (keyboard.scanCode[HK_RIGHT] && movableX <= boundaryRight)
            {
                movableX += speed;
            }

            if (keyboard.scanCode[HK_DOWN] && movableY <= boundaryDown)
            {
                movableY += speed;
            }

            // Render images.
            screenManager.blitOpaque (0, 0, background);
            screenManager.blitFast ((int) movableX, (int) movableY, alphaThing);
        }
    }
}