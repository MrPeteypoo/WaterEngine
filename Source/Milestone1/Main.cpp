#include "ScreenManager/ScreenManager.h"




void HAPI_Main()
{
    int screenWidth = 1024, screenHeight = 768, 
        screenTotal = screenWidth * screenHeight;

    ScreenManager screenManager = ScreenManager ((unsigned) screenWidth, (unsigned) screenHeight);


	if (HAPI->Initialise (&screenWidth, &screenHeight))
	{
        HAPI->SetShowFPS (true);
        HAPI->SetShowCursor (true);
        
        HAPI_TColour colour (HAPI_TColour (255, 0, 0, 255));

        unsigned int i = 0;

        while (HAPI->Update())
        {
            switch (i++ % 3)
            {
                case 0:
                    colour = HAPI_TColour (255, 0, 0, 255);
                    break;

                case 1:
                    colour = HAPI_TColour (0, 255, 0, 255);
                    break;

                case 2:
                    colour = HAPI_TColour (0, 0, 255, 255);
            }

            screenManager.clearScreen (colour);
        }
	}
}