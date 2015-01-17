#include "Texture.h"


// STL headers.
#include <exception>


// Engine headers.
#include <HAPI_lib.h>


// Constants used throughout.
const auto sizeOfColour = sizeof (HAPI_TColour);



#pragma region Constructors and destructor


Texture::Texture (const std::string& fileLocation)
{   
    // Attempt to load the texture during construction.
    loadTexture (fileLocation);
}


Texture::Texture (const std::string& fileLocation, const Vector2D<unsigned int>& frameDimensions)
{
    // Attempt to load the texture during construction.
    loadTexture (fileLocation);

    // Attempt to set the frame dimensions.
    setFrameDimensions (frameDimensions);
}


Texture::Texture (Texture&& move)
{
    // Just use the operator implementation.
    *this = std::move (move);
}


Texture& Texture::operator= (Texture&& move)
{
    if (this != &move)
    {
        // Obtain all of the data from the movee.
        m_frames = move.m_frames;

        m_frameDimensions = std::move (move.m_frameDimensions);
        m_textureSpace = std::move (move.m_textureSpace);
        m_pData = std::move (move.m_pData);


        // Reset the movee.
        move.m_frames = 0;
        move.m_pData = nullptr;
    }

    return *this;
}


#pragma endregion Constructors and destructor



#pragma region Getters and setters


Texture& Texture::resetFrameDimensions()
{
    m_frames = 0;
    m_frameDimensions.x = 0;
    m_frameDimensions.y = 0;

    return *this;
}


Texture& Texture::setFrameDimensions (const Vector2D<unsigned int>& dimensions)
{
    if (dimensions.x == 0 || dimensions.y == 0)
    {
        // Disable spritesheet functionality.
        resetFrameDimensions();
    }

    else
    {
        // Recalculate frame values.
        m_frames            = dimensions.x * dimensions.y;
        m_frameDimensions.x = dimensions.x;
        m_frameDimensions.y = dimensions.y;
    }

    return *this;
}


#pragma endregion Getters and setters



#pragma region Loading functionality


void Texture::loadTexture (const std::string& fileLocation)
{
    // Create variable cache for interfacing with HAPI.
    int     width = 0, height = 0;
    BYTE*   data  = nullptr;

    // Attempt to load the data. If the loading succeeds then the width and height will be set.
    if (HAPI->LoadTexture (fileLocation, &data, &width, &height))
    {
        resetFrameDimensions();
        
        // Initialise the texture space.
        m_textureSpace = { 0, 0, width - 1, height - 1 };

        // Turn the raw pointer into a unique_ptr.
        m_pData = std::unique_ptr<BYTE[]> (data);
    }
    
    else
    {   
        throw std::runtime_error ("Texture::loadTexture(): Unable to initialise Texture with file \"" + fileLocation + "\"");
    }
}


#pragma endregion Loading functionality



#pragma region Rendering


void Texture::blit (BYTE* screen, const Rectangle& screenSpace, const Vector2D<int>& point, const BlendType blend, const Vector2D<unsigned int>& frame)
{
    // Pre-conditions: screen is a valid pointer.
    if (!screen)
    {
        throw std::invalid_argument ("Texture::blit(): Invalid pointer to the screenbuffer was given.");
    }

    // Pre-condition: frame is valid.
    if (m_frames != 0 && (frame.x >= m_frameDimensions.x || frame.y >= m_frameDimensions.y))
    {
        throw std::invalid_argument ("Texture::blit(): Invalid frame number given (" + 
                                     std::to_string (frame.x) + ", " + std::to_string (frame.y) + "). " +

                                     "Texture frame count is (" + 
                                     std::to_string (m_frameDimensions.x) + ", " + std::to_string (m_frameDimensions.y) + ").");
    }

    const unsigned int  textureWidth = m_textureSpace.width(), 
                        textureHeight = m_textureSpace.height();

    // We need to check if the texture will actually need to be drawn.
    Rectangle texture   {   point.x,
                            point.y,
                            point.x + static_cast<int> (textureWidth - 1),
                            point.y + static_cast<int> (textureHeight - 1) };

    // Determine the frame offset and check if the texture size needs changing.
    Vector2D<unsigned int> frameOffset { 0, 0 };

    // If m_frames is zero it's a single texture and therefore do not need to calculate offsets.
    if (m_frames != 0)
    {
        const unsigned int  frameWidth = textureWidth / m_frameDimensions.x,
                            frameHeight = textureHeight / m_frameDimensions.y;

        // Update the texture co-ordinates.
        texture.setRight (point.x + static_cast<int> (frameWidth) - 1);
        texture.setBottom (point.y + static_cast<int> (frameHeight) - 1);

        // Calculate how much we need to translate by.
        frameOffset.x = frame.x * frameWidth;
        frameOffset.y = frame.y * frameHeight;
    }
    
    // We will only draw if the texture is on-screen.
    if (screenSpace.intersects (texture))
    {
        // Check if any clipping is necessary.
        if (!screenSpace.contains (texture))
        {
            // Clip the rectangle.
            texture.clipTo (screenSpace);
        }
            
        // Translate back to texture space, ready for blitting.
        texture.translate (-point.x, -point.y);

        // Call the correct blitting function.
        switch (blend)
        {
            case BlendType::Opaque:
                blitOpaque (screen, screenSpace, point, frameOffset, texture);
                break;

            case BlendType::Transparent:
                blitTransparent (screen, screenSpace, point, frameOffset, texture);
                break;
        }
    }

    // Do nothing if it doesn't intersect.
}


void Texture::blitOpaque (BYTE* screen, const Rectangle& screenSpace, const Vector2D<int> point, const Vector2D<unsigned int>& frameOffset, const Rectangle& drawArea)
{    
    // Cache zee variables captain!
    const auto  blitWidth       = drawArea.width(),
                blitHeight      = drawArea.height();
               
    const auto  dataWidth       = blitWidth * sizeOfColour,
                screenWidth     = screenSpace.width() * sizeOfColour,
                textureWidth    = m_textureSpace.width() * sizeOfColour,
                
                dataOffset      = (drawArea.getLeft() + frameOffset.x) * sizeOfColour + (drawArea.getTop() + frameOffset.y) * textureWidth,
                screenOffset    = (point.x + drawArea.getLeft()) * sizeOfColour + (point.y + drawArea.getTop()) * screenWidth;
    
    // Obtain the data from the texture.
    const auto  textureData     = m_pData.get() + dataOffset;
    
    // Calculate the starting pointer to the screen position.
    screen += screenOffset;    
    BYTE* currentLine = nullptr;

    for (unsigned int y = 0; y < blitHeight; ++y)
    {
        // Increment the pointer and copy line-by-line.
        currentLine = screen + y * screenWidth;
        std::memcpy (currentLine, (textureData + y * textureWidth), dataWidth);
    }
}


void Texture::blitTransparent (BYTE* const screen, const Rectangle& screenSpace, const Vector2D<int> point, const Vector2D<unsigned int>& frameOffset, const Rectangle& drawArea)
{
    // Cache zee variables captain!
    const auto  blitWidth       = drawArea.width(),
                blitHeight      = drawArea.height();
               
    const auto  dataWidth       = blitWidth * sizeOfColour,
                screenWidth     = screenSpace.width() * sizeOfColour,
                textureWidth    = m_textureSpace.width() * sizeOfColour,
                
                dataOffset      = (drawArea.getLeft() + frameOffset.x) * sizeOfColour + (drawArea.getTop() + frameOffset.y) * textureWidth,
                screenOffset    = (point.x + drawArea.getLeft()) * sizeOfColour + (point.y + drawArea.getTop()) * screenWidth;

    // Create the required pointers for the blitting process.
    auto        currentData     = m_pData.get() + dataOffset;
    auto        currentPixel    = screen + screenOffset;

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


#pragma endregion Rendering