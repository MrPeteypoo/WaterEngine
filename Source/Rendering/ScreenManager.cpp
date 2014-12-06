#include "ScreenManager.h"


// Engine headers.
#include <HAPI_lib.h>
#include <Rendering/Texture.h>


// Useful constants
const size_t sizeOfColour { sizeof (Colour) };    //!< The size in bytes of the Colour class.



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


void ScreenManager::blit (const Vector2D<int>& position, const Texture& texture, const BlendType blendType)
{    
    // Start by constructing the rectangle in screen space.
    Rectangle textureRect   { position.x, 
                              position.y, 
                              position.x + static_cast<int> (texture.getWidth()) - 1,
                              position.y + static_cast<int> (texture.getHeight()) - 1 };

    // We will only draw if the texture is on-screen.
    if (m_screenRect.intersects (textureRect))
    {
        // Check if any clipping is necessary.
        if (!m_screenRect.contains (textureRect))
        {
            // Clip the rectangle.
            textureRect.clipTo (m_screenRect);
        }
            
        // Translate back to texture space, ready for blitting.
        textureRect.translate (-position.x, -position.y);

        // Call the correct blitting function.
        switch (blendType)
        {
            case BlendType::Opaque:
                blitOpaque (position, textureRect, texture);
                break;

            case BlendType::Transparent:
                blitTransparent (position, textureRect, texture);
                break;
        }
    }

    // Do nothing if it doesn't intersect.
}


void ScreenManager::blitOpaque (const Vector2D<int>& position, const Rectangle& drawArea, const Texture& texture)
{
    // Cache zee variables captain!
    const auto  blitWidth       = drawArea.width(),
                blitHeight      = drawArea.height();
               
    const auto  dataWidth       = blitWidth * sizeOfColour,
                screenWidth     = m_screenRect.width() * sizeOfColour,
                textureWidth    = texture.getWidth() * sizeOfColour,
                
                dataOffset      = drawArea.getLeft() * sizeOfColour + drawArea.getTop() * textureWidth,
                screenOffset    = (position.x + drawArea.getLeft()) * sizeOfColour + (position.y + drawArea.getTop()) * screenWidth;
    
    // Obtain the data from the texture.
    const auto  textureData     = texture.getData() + dataOffset;
    
    // Calculate the starting pointer to the screen position.
    const auto  screen          = m_screen + screenOffset;
    BYTE*       currentLine     = nullptr;

    for (unsigned int y = 0; y < blitHeight; ++y)
    {
        // Increment the pointer and copy line-by-line.
        currentLine = screen + y * screenWidth;
        std::memcpy (currentLine, (textureData + y * textureWidth), dataWidth);
    }
}

void ScreenManager::blitTransparent (const Vector2D<int>& position, const Rectangle& drawArea, const Texture& texture)
{    
    // Cache zee variables captain!
    const auto  blitWidth       = drawArea.width(),
                blitHeight      = drawArea.height();
               
    const auto  dataWidth       = blitWidth * sizeOfColour,
                screenWidth     = m_screenRect.width() * sizeOfColour,
                textureWidth    = texture.getWidth() * sizeOfColour,
                
                dataOffset      = drawArea.getLeft() * sizeOfColour + drawArea.getTop() * textureWidth,
                screenOffset    = (position.x + drawArea.getLeft()) * sizeOfColour + (position.y + drawArea.getTop()) * screenWidth;

    // Create the required pointers for the blitting process.
    auto        currentData     = texture.getData() + dataOffset;
    auto        currentPixel    = m_screen + screenOffset;

    // Avoid those magic constants!
    const unsigned int alphaIndex = 3;

    for (unsigned int y = 0; y < blitHeight; ++y)
    {
        for (unsigned int x = 0; x < blitWidth; ++x)
        {
            const auto alpha = currentData[alphaIndex];

            // Avoid unnecessary blending when alpha is 0 or 255.
            switch (alpha)
            {
                case 0:
                    break;

                case 255:
                    for (unsigned int i = 0; i < alphaIndex; ++i)
                    {
                        currentPixel[i] = currentData[i];
                    }

                    break;

                default:                     
                    for (unsigned int i = 0; i < alphaIndex; ++i)
                    {
                        // Avoid floating-point arithmetic by bit-shifting.
                        const auto currentChannel = currentPixel[i];
                        currentPixel[i] = currentChannel + ((alpha * (currentData[i] - currentChannel)) >> 8);
                    }

                    break;
            }
            
            // Increment each pointer.
            currentPixel += sizeOfColour;
            currentData += sizeOfColour;
        }

        // Since the width is done we must go onto the next line.
        currentPixel += screenWidth - dataWidth;
        currentData += textureWidth - dataWidth;
    }
}


#pragma endregion Rendering functionality