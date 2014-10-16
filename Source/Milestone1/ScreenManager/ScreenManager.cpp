#include "ScreenManager.h"


BYTE* ScreenManager::getScreenPointer()
{
    if (!m_screen)
    {
        m_screen = HAPI->GetScreenPointer();
    }

    return m_screen;
}


void ScreenManager::clearScreen (const HAPI_TColour& colour)
{
    const unsigned int colourSize = 4;
    auto screenByte = getScreenPointer();

    if (screenByte)
    {
        for (unsigned int i = 0; i < getScreenSize(); ++i)
        {
            auto pixel = screenByte + i * colourSize;

            std::memcpy (pixel, &colour, colourSize);
        }
    }
}