#include "ScreenManager.h"


// Engine headers.
#include <HAPI_lib.h>
#include <Rendering/Texture.h>


// Useful constants
const size_t sizeOfColour { sizeof (Colour) };    //!< The size in bytes of the HAPI_TColour.



#pragma region Constructors and destructor


ScreenManager::ScreenManager (const int screenWidth, const int screenHeight)
{
    // Check if a valid screen pointer is available
    m_screen = HAPI->GetScreenPointer();

    if (!m_screen)
    {
        throw std::runtime_error ("ScreenManager::ScreenManager(): Unable to obtain screen pointer from HAPI.");
    }
    
    // We don't want silly screen resolutions now do we?
    if (screenWidth < 1 || screenHeight < 1)
    {
        throw std::runtime_error ("ScreenManager::ScreenManager(): Invalid screen resolution (" +   std::to_string (screenWidth) + "x" + 
                                                                                                    std::to_string (screenHeight) + ").");
    }

    m_screenRect = { 0, 0, screenWidth - 1, screenHeight - 1 };
}


#pragma endregion Constructors and destructor


#pragma region Rendering functionality


void ScreenManager::clearToBlackLevel (const BYTE blackLevel)
{
    // Use memset for efficiency.
    std::memset (m_screen, blackLevel, m_screenRect.area() * sizeOfColour);
}


void ScreenManager::clearToColour (const Colour& colour)
{
    const int screenSize = m_screenRect.area();

    // Don't call setPixel, instead implement with unnecessary error checking removed for efficiency.
    for (int i = 0; i < screenSize; ++i)
    {
        // Find the correct memory address
        auto pixel = m_screen + i * sizeOfColour;

        std::memcpy (pixel, &colour, sizeOfColour);
    }
}


void ScreenManager::blit (const Vector2D<int>& position, const Texture& texture, bool blendAlpha = true)
{    
    // Start by constructing the rectangle in screen space.
    Rectangle textureRect   { position.x, position.y, 
                              position.x + texture.getWidth() - 1, position.y + texture.getHeight() - 1 };
    
    // We will only draw if the texture is on-screen.
    if (m_screenRect.intersects (textureRect))
    {
        // Check if any clipping is necessary.
        if (!m_screenRect.contains (textureRect))
        {
            textureRect.clipTo (m_screenRect);
            
            // Translate back to texture space, ready for blitting.
            textureRect.translate (-position.x, -position.y);
        }

        // Call the correct blitting function.
        if (blendAlpha)
        {
            blitBlend (position, textureRect, texture);
        }

        else
        {
            blitOpaque (position, textureRect, texture);
        }
    }

    // Do nothing if it doesn't intersect.
}


void ScreenManager::blitOpaque (const Vector2D<int>& position, const Rectangle& drawArea, const Texture& texture)
{

}

void ScreenManager::blitBlend (const Vector2D<int>& position, const Rectangle& drawArea, const Texture& texture)
{/*
    // Start by obtaining the width and height of the image.
    const int width = texture.getWidth(), height = texture.getHeight();

    // Ensure it's on-screen.
    if (posX >= 0 && posX + width <= m_screenWidth &&
        posY >= 0 && posY + height <= m_screenHeight)
    {
        // Obtain the raw data from the texture.
        const auto textureData = texture.getData();

        const int destination = (posX + posY * m_screenWidth) * sizeOfColour;

        BYTE* currentPixel = m_screen + destination;
        const BYTE* currentData = textureData;

        for (int y = 0; y < height; ++y)
        {
            for (int x = 0; x < width; ++x)
            {
                const auto alpha = currentData[3];

                // Avoid unnecessary blending when alpha is 0 or 255.
                switch (alpha)
                {
                    case 0:
                        break;

                    case 255:
                        for (unsigned int i = 0; i < 3; ++i)
                        {
                            currentPixel[i] = currentData[i];
                        }

                        break;

                    default:                        
                        // Avoid floating-point arithmetic by bit-shifting.
                        for (unsigned int i = 0; i < 3; ++i)
                        {
                            const auto current = currentPixel[i];
                            currentPixel[i] = current + ((alpha * (currentData[i] - current)) >> 8);
                        }

                        break;
                }
                
                // Increment each pointer.
                currentPixel += sizeOfColour;
                currentData += sizeOfColour;
            }

            // Since the width is done we must go onto the next line.
            currentPixel += (m_screenWidth - width) * sizeOfColour;
        }
    }*/
}
    /*
    // Start by obtaining the width and height of the image.
    const int width = texture.getWidth(), height = texture.getHeight();

    // Ensure it's on-screen.
    if (posX >= 0 && posX + width <= m_screenWidth &&
        posY >= 0 && posY + height <= m_screenHeight)
    {
        // Obtain the data from the texture.
        const auto textureData = texture.getData();

        const int   dataWidth   = width * sizeOfColour,
                    screenWidth = m_screenWidth * sizeOfColour;

        // Calculate the starting pointer to the position.
        BYTE* const screen = m_screen + (posX + posY * screenWidth);
        BYTE* currentLine = nullptr;

        for (int y = 0; y < height; ++y)
        {
            // Increment the pointer and copy line-by-line.
            currentLine = screen + y * screenWidth;
            std::memcpy (currentLine, (textureData + y * dataWidth), dataWidth);
        }
    }
}*/


#pragma endregion Rendering functionality