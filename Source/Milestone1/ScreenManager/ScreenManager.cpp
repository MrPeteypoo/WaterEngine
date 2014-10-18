#include "ScreenManager.h"


#pragma region Constructors and destructor


ScreenManager::ScreenManager (const int screenWidth, const int screenHeight)
    
    : m_screenWidth (screenWidth), m_screenHeight (screenHeight), m_screenSize ((unsigned int) screenWidth * (unsigned int) screenHeight)
{
    // We don't want silly screen resolutions now do we?
    if (screenWidth < 1 || screenHeight < 1)
    {
        throw std::runtime_error ("Invalid screen resolution given in ScreenManager::ScreenManager()");
    }

    // Check if a valid screen pointer is available
    if (!(m_screen = HAPI->GetScreenPointer()))
    {
        throw std::runtime_error ("Unable to obtain screen pointer from HAPI.");
    }
}

#pragma endregion Constructors and destructor




#pragma region Colouring functionality


void ScreenManager::clearToBlack (const unsigned int blackLevel)
{
    // Pre-condition: Black level is from 0 to 255.
    if (m_screen && blackLevel < 256)
    {
        std::memset (m_screen, blackLevel, m_screenSize * m_kColourSize);
    }
}


void ScreenManager::clearToColour (const HAPI_TColour& colour)
{    
    // Ensure we have a valid pointer to the screen
    
    if (m_screen)
    {
        // Don't call setPixel, instead implement with unnecessary error checking removed for efficiency.
        for (unsigned int i = 0; i < m_screenSize; ++i)
        {
            // Find the correct memory address
            auto pixel = m_screen + i * m_kColourSize;

            std::memcpy (pixel, &colour, m_kColourSize);
        }
    }
}


void ScreenManager::setPixel (const unsigned int pixelNumber, const HAPI_TColour& colour)
{
    // Pre-condition: The screen pointer must be valid and a valid pixel number has been given
    if (m_screen && pixelNumber < m_screenSize)
    {
        // Find and set the correct pixel
        auto pixel = m_screen + pixelNumber * m_kColourSize;

        std::memcpy (pixel, &colour, m_kColourSize);
    }
}


#pragma endregion Colouring functionality