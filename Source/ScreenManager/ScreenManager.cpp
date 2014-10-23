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
    m_screen = HAPI->GetScreenPointer();

    if (!m_screen)
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
        const int destination = posX + posY * m_screenWidth;

        // Avoid creating the colour each loop.
        HAPI_TColour colour;

        for (int y = 0; y < height; ++y)
        {
            for (int x = 0; x < width; ++x)
            {
                colour = texture.getPixel (x, y);
                
                const int pixel = destination + (x + y * m_screenWidth);
                const auto alpha = colour.alpha;

                // Avoid unnecessary blending when alpha is 0 or 255.
                switch (alpha)
                {
                    case 0:
                        break;

                    case 255:
                        setPixel (pixel, colour);
                        break;

                    default:                        
                        const HAPI_TColour screen = getPixel (pixel);

                        // Avoid floating-point arithmetic by bit-shifting.
                        colour.red = screen.red + ((alpha * (colour.red - screen.red)) >> 8);
                        colour.green = screen.green + ((alpha * (colour.green - screen.green)) >> 8);
                        colour.blue = screen.blue + ((alpha * (colour.blue - screen.blue)) >> 8);
                        
                        // Calculate the blended value with floating-points.
                        /*const float alpha = colour.alpha / 255.f, leftover = 1.f - alpha;
                      
                        colour.red = (BYTE) (colour.red * alpha + screen.red * leftover);
                        colour.green = (BYTE) (colour.green * alpha + screen.green * leftover);
                        colour.blue = (BYTE) (colour.blue * alpha + screen.blue * leftover);*/
                        
                        setPixel (pixel, colour);
                        break;
                }
            }
        }
    }

    // Don't bother blitting an off-screen image for now.
}


void ScreenManager::blitFast (const int posX, const int posY, const Texture& texture)
{
    
}


void ScreenManager::blitOpaque (const int posX, const int posY, const Texture& texture)
{
    // Start by obtaining the width and height of the image.
    const int width = texture.getWidth(), height = texture.getHeight();

    // Ensure it's on-screen.
    if (posX >= 0 && posX + width <= m_screenWidth &&
        posY >= 0 && posY + height <= m_screenHeight)
    {
        // Obtain the data from the texture.
        const auto textureData = texture.getData();

        const int dataWidth   = width * sizeOfColour,
                    screenWidth = m_screenWidth * sizeOfColour;

        // Calculate the starting pointer to the position.
        BYTE* const screen = m_screen + (posX + posY * screenWidth);
        BYTE* currentLine = m_screen;

        for (int y = 0; y < height; ++y)
        {
            // Increment the pointer and copy line-by-line.
            currentLine = screen + y * screenWidth;
            std::memcpy (currentLine, (textureData + y * dataWidth), dataWidth);
        }
    }
    
}


#pragma endregion Rendering functionality


#pragma region Helper functions


HAPI_TColour ScreenManager::getPixel (const int pixel) const
{
    try
    {
        // Find the correct address and return the colour.
        const auto pixelAddress = m_screen + pixel * sizeOfColour;
        
        // The order of channels in memory is BGRA, but the constructor takes RGBA.
        return
        {
            *(pixelAddress + 2),
            *(pixelAddress + 1),
            *(pixelAddress),
            *(pixelAddress + 3)
        };
    }
    
    catch (const std::exception& error)
    {
        HAPI->DebugText ("ScreenManager::getPixel(): " + (std::string) error.what());
    }

    catch (...)
    {
        HAPI->DebugText ("ScreenManager::getPixel(): Unknown error occurred.");
    }
    
    return { };
}


void ScreenManager::setPixel (const int pixel, const HAPI_TColour& colour)
{
    // Find and set the correct pixel.
    const auto pixelAddress = m_screen + pixel * sizeOfColour;

    std::memcpy (pixelAddress, &colour, sizeOfColour);
}


#pragma endregion Helper functions