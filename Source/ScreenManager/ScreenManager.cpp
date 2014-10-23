#include "ScreenManager.h"


// Engine headers.
#include <HAPI_lib.h>
#include <Misc/Texture.h>


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


void ScreenManager::blit (const int posX, const int posY, const Texture& texture)
{
    // Start by obtaining the width and height of the image.
    const int   width = texture.getWidth(),
                height = texture.getHeight();

    // Ensure it's on-screen.
    if (posX >= 0 && posX + width <= m_screenWidth &&
        posY >= 0 && posY + height <= m_screenHeight)
    {
        // Get the destination pixel and the size of the texture.
        const int   destination = posX + posY * m_screenWidth,
                    size = texture.getResolution();

        // Avoid creating the colour each loop.
        HAPI_TColour colour;

        for (int y = 0; y < height; ++y)
        {
            for (int x = 0; x < width; ++x)
            {
                const int pixel = destination + (x + y * m_screenWidth);
                
                colour = texture.getColour (x, y);
                
                // Avoid unnecessary blending when alpha is 0 or 255.
                switch (colour.alpha)
                {
                    case 0:
                        break;

                    case 255:
                        setPixel (pixel, colour);
                        break;

                    default:
                        const float alpha = colour.alpha / 255.f, 
                                    leftover = 1.f - alpha;

                        const HAPI_TColour current = getPixel (pixel);
                        
                        colour.red = (BYTE) (colour.red * alpha + current.red * leftover);
                        colour.green = (BYTE) (colour.green * alpha + current.green * leftover);
                        colour.blue = (BYTE) (colour.blue * alpha + current.blue * leftover);

                        setPixel (pixel, colour);
                        break;
                }
            }
        }
    }

    // Don't bother blitting an off-screen image for now.
}


#pragma endregion Rendering functionality


#pragma region Helper functions


HAPI_TColour ScreenManager::getPixel (const int pixel) const
{
    // Pre-condition: The pixel must be a valid value.
    if (pixel < 0 || pixel >= m_screenSize)
    {
        std::runtime_error ("ScreenManager::getPixel(): Attempt to get pixel " + std::to_string (pixel) + " when resolution is " + std::to_string (m_screenSize) + ".");
    }


    // Find the correct address and return the colour.
    const auto pixelAddress = m_screen + pixel * sizeOfColour;
        
    // The order of channels in memory is BGRA.
    const BYTE  blue    = *pixelAddress,
                green   = *(pixelAddress + 1),
                red     = *(pixelAddress + 2),
                alpha   = *(pixelAddress + 3);

    return { red, green, blue, alpha };
}


void ScreenManager::setPixel (const int pixel, const HAPI_TColour& colour)
{
    // Pre-condition: The pixel must be a valid value.
    if (pixel < 0 || pixel >= m_screenSize)
    {
        std::runtime_error ("ScreenManager::setPixel(): Attempt to set pixel " + std::to_string (pixel) + " when resolution is " + std::to_string (m_screenSize) + ".");
    }


    // Find and set the correct pixel.
    auto pixelAddress = m_screen + pixel * sizeOfColour;

    std::memcpy (pixelAddress, &colour, sizeOfColour);
}


#pragma endregion Helper functions