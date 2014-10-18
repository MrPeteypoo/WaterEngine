#include <memory>

#include "ScreenManager/ScreenManager.h"
#include "Helper/RNG.h"
#include "Helper/Time.h"



void HAPI_Main()
{
    int screenWidth = 1024, screenHeight = 768;
    const unsigned int screenSize = (unsigned int) screenWidth * (unsigned int) screenHeight;

    if (HAPI->Initialise (&screenWidth, &screenHeight))
    {
        HAPI->SetShowFPS (true);
        
        auto screenManager = std::make_unique<ScreenManager> (ScreenManager (screenWidth, screenHeight));
        screenManager->clearToBlack();

        auto pixelRNG = RNG<unsigned int> (0, screenSize, helper::getCurrentTimeUInt());
        auto colourRNG = RNG<unsigned int> (0, 255, helper::getCurrentTimeUInt());

        while (HAPI->Update())
        {
            screenManager->setPixel (pixelRNG.getRandom(), HAPI_TColour (colourRNG.getRandom(), colourRNG.getRandom(), colourRNG.getRandom(), 255));
        }
    }
}