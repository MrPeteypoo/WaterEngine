#include "ScreenManager.h"


// STL headers.
#include <string>


// Engine headers.
#include <HAPI_lib.h>


const size_t sizeOfColour { sizeof (HAPI_TColour) };    //!< The size in bytes of the HAPI_TColour.


#pragma region Constructors and destructor


ScreenManager::ScreenManager (const int screenWidth, const int screenHeight)
    : m_screenWidth (screenWidth), m_screenHeight (screenHeight), m_screenSize (screenWidth * screenHeight)
{
    // We don't want silly screen resolutions now do we?
    if (screenWidth < 1 || screenHeight < 1)
    {
        throw std::runtime_error ("ScreenManager::ScreenManager(): Invalid screen resolution (" + std::to_string (m_screenWidth) + "x" + std::to_string (m_screenHeight) + ").");
    }

    // Check if a valid screen pointer is available
    if (!(m_screen = HAPI->GetScreenPointer()))
    {
        throw std::runtime_error ("ScreenManager::ScreenManager(): Unable to obtain screen pointer from HAPI.");
    }
}


ScreenManager::ScreenManager (ScreenManager&& move)
{
    *this = std::move (move);
}


ScreenManager& ScreenManager::operator= (const ScreenManager& copy)
{
    m_screen = copy.m_screen;
    m_screenWidth = copy.m_screenWidth;
    m_screenHeight = copy.m_screenHeight;
    m_screenSize = copy.m_screenSize;

    return *this;
}


ScreenManager& ScreenManager::operator= (ScreenManager&& move)
{
    if (this != &move)
    {
        m_screen = move.m_screen;
        m_screenWidth = move.m_screenWidth;
        m_screenHeight = move.m_screenHeight;
        m_screenSize = move.m_screenSize;

        move.m_screen = nullptr;
        move.m_screenWidth = 0;
        move.m_screenHeight = 0;
        move.m_screenSize = 0;
    }

    return *this;
}


#pragma endregion Constructors and destructor


#pragma region Rendering functionality


void ScreenManager::clearToBlack (const unsigned char blackLevel)
{    
    // Use memset for efficiency.
    std::memset (m_screen, blackLevel, m_screenSize * sizeOfColour);
}


void ScreenManager::clearToColour (const HAPI_TColour& colour)
{
    // Don't call setPixel, instead implement with unnecessary error checking removed for efficiency.
    for (int i = 0; i < m_screenSize; ++i)
    {
        // Find the correct memory address
        auto pixel = m_screen + i * sizeOfColour;

        std::memcpy (pixel, &colour, sizeOfColour);
    }
    
}


#pragma endregion Rendering functionality


#pragma region Helper functions


void ScreenManager::setPixel (const int pixel, const HAPI_TColour& colour)
{
    // Pre-condition: The pixel must be a valid value.
    if (pixel < 0 || pixel >= m_screenSize)
    {
        std::runtime_error ("ScreenManager::setPixel(): Attempt to set pixel " + std::to_string (pixel) + " when resolution is " + std::to_string (m_screenSize));
    }


    // Find and set the correct pixel
    auto pixelAddress = m_screen + pixel * sizeOfColour;

    std::memcpy (pixelAddress, &colour, sizeOfColour);
}


#pragma endregion Helper functions