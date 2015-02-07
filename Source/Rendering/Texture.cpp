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


Texture::Texture (const std::string& fileLocation, const Point& frameDimensions)
{
    // Attempt to load the texture during construction.
    loadTexture (fileLocation);

    // Attempt to set the frame dimensions.
    setFrameDimensions (frameDimensions);
}


Texture::Texture (const Point& pixelDimensions)
{
    // Load all the data captain!
    fillWithBlankData (pixelDimensions);
}


Texture::Texture (const Point& pixelDimensions, const Point& frameDimensions)
{
    // Load all the data captain!
    fillWithBlankData (pixelDimensions);

    // Set up the spritesheet!
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
        m_pData = move.m_pData;


        // Reset the movee.
        move.m_frames = 0;
        move.m_pData = nullptr;
    }

    return *this;
}


Texture::~Texture()
{
    cleanUp();
}

#pragma endregion


#pragma region Getters and setters

Texture& Texture::resetFrameDimensions()
{
    m_frames = 0;
    m_frameDimensions.x = 0;
    m_frameDimensions.y = 0;

    return *this;
}


Texture& Texture::setFrameDimensions (const Point& dimensions)
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

#pragma endregion


#pragma region Loading functionality

void Texture::cleanUp()
{
    if (m_pData)
    {
        delete[] m_pData;
        m_pData = nullptr;
    }
}


void Texture::loadTexture (const std::string& fileLocation)
{
    // Make sure we don't leak memory.
    cleanUp();

    // Create variable cache for interfacing with HAPI.
    int width = 0, height = 0;

    // Attempt to load the data. If the loading succeeds then the width and height will be set.
    if (HAPI->LoadTexture (fileLocation, &m_pData, &width, &height))
    {
        resetFrameDimensions();
        
        // Initialise the texture space.
        m_textureSpace = { 0, 0, width - 1, height - 1 };
    }
    
    else
    {   
        throw std::runtime_error ("Texture::loadTexture(): Unable to initialise Texture with file \"" + fileLocation + "\"");
    }
}


void Texture::fillWithBlankData (const Point& dimensions)
{
    // Pre-condition: Ensure the dimensions are valid.
    if (dimensions.x <= 0 || dimensions.y <= 0)
    {
        throw std::invalid_argument ("Texture::fillWithBlankData(): Invalid texture dimensions given (" + 
                                      std::to_string (dimensions.x) + "x" + std::to_string (dimensions.y) + ").");     
    }

    // Make sure we don't leak memory.
    cleanUp();

    // Allocate enough data.
    const auto size = dimensions.x * dimensions.y;
    m_pData         = new BYTE[size];

    // Prepare the texture.
    resetFrameDimensions();

    m_textureSpace  = { 0, 0, dimensions.x, dimensions.y };

    // Ensure all data is black.
    for (int i = 0; i < size; ++i)
    {
        m_pData[i] = 0;
    }
}

#pragma endregion


#pragma region Scaling

void Texture::scaleToSize (const Point& dimensions)
{
    // Pre-condition: Ensure the dimensions are valid.
    if (dimensions.x <= 0 || dimensions.y <= 0)
    {
        throw std::invalid_argument ("Texture::fillWithBlankData(): Invalid texture dimensions given (" + 
                                      std::to_string (dimensions.x) + "x" + std::to_string (dimensions.y) + ").");
    }

    // Work on the basis of colours instead of bytes for the scaling.
    const auto  width       = m_textureSpace.width(),
                height      = m_textureSpace.height();

    // Don't do anything if no scaling is to be performed.
    if (dimensions.x != width || dimensions.y != height)
    {
        const auto  floatWidth  = (float) dimensions.x,
                    floatHeight = (float) dimensions.y;

        // Allocate the required data.
        Colour* scaledData      = new Colour[dimensions.x * dimensions.y];

        // Filter each pixel.
        for (int y = 0; y < dimensions.y; ++y)
        {
            for (int x = 0; x < dimensions.x; ++x)
            {
                // We need to find where we are in terms of the unscaled data.
                const float unscaledX = x / floatWidth * width,
                            unscaledY = y / floatHeight * height;

                // Now we need to calculate the current scaled pixel using bilinear filtering.
                scaledData[x + y * dimensions.x] = bilinearFilteredPixel (unscaledX, unscaledY);
            }
        }

        // Clean up the old texture data and replace it with the new data.
        cleanUp();
        m_pData         = (BYTE*) scaledData;
        m_textureSpace  = { 0, 0, dimensions.x - 1, dimensions.y - 1 };
        setFrameDimensions (m_frameDimensions);
    }
}


Colour Texture::bilinearFilteredPixel (const float x, const float y) const
{
    /// This code is a modified version of a very useful blog post.
    /// theowl84 (2011) 'Bilinear Pixel Interpolation using SSE', FastC++: Coding Cpp Efficiently, 16 June.
    /// Available at: http://fastcpp.blogspot.co.uk/2011/06/bilinear-pixel-interpolation-using-sse.html (Accessed: 07/02/2015).

    // We avoid casting as much as possible here since this is a load-time function, we do not need to sacrifice clarity for efficiency.
    // Start by flooring X and Y so we can interpolate between pixels.
    const auto  px      = (int) x,
                py      = (int) y,
                stride  = m_textureSpace.width();

    // Obtain the base pixel.
    const auto  p0      = (Colour*) (m_pData) + px + py * stride;
 
    // Obtain a reference to the four neighbouring pixels.
    const auto& p1      = p0[0 + 0 * stride],
                p2      = p0[1 + 0 * stride],
                p3      = p0[0 + 1 * stride],
                p4      = p0[1 + 1 * stride];
 
    // Calculate the weights for each pixel. This is where we determine how much to blend each neighbour by. We need to clamp to prevent overshooting.
    const auto  fx      = x < m_textureSpace.getRight() ? x - px : 0.f,
                fy      = y < m_textureSpace.getBottom() ? y - py : 0.f,
                fx1     = 1.0f - fx,
                fy1     = 1.0f - fy,
  
                w1      = fx1 * fy1,
                w2      = fx  * fy1,
                w3      = fx1 * fy,
                w4      = fx  * fy;
 
    // Interpolate each channel, for cache reasons calculate in BGRA order.
    const auto  b       = (BYTE) (p1.blue * w1 + p2.blue * w2 + p3.blue * w3 + p4.blue * w4),
                g       = (BYTE) (p1.green * w1 + p2.green * w2 + p3.green * w3 + p4.green * w4),
                r       = (BYTE) (p1.red * w1 + p2.red * w2 + p3.red * w3 + p4.red * w4),
                a       = (BYTE) (p1.alpha * w1 + p2.alpha * w2 + p3.alpha * w3 + p4.alpha * w4);
 
    return { r, g, b, a };
}

#pragma endregion


#pragma region Blitting

void Texture::blit (BYTE* const target, const Rectangle<int>& targetSpace, const Point& point, const BlendType blend, const Point& frame)
{
    // Pre-conditions: target is a valid pointer.
    if (!target)
    {
        throw std::invalid_argument ("Texture::blit(): Invalid pointer to the target was given.");
    }

    // Pre-condition: frame is valid.
    if (m_frames != 0 && 
       (frame.x >= m_frameDimensions.x || frame.x < 0 ||
        frame.y >= m_frameDimensions.y || frame.y < 0))
    {
        throw std::invalid_argument ("Texture::blit(): Invalid frame number given (" + 
                                     std::to_string (frame.x) + ", " + std::to_string (frame.y) + "). " +

                                     "Texture frame count is (" + 
                                     std::to_string (m_frameDimensions.x) + ", " + std::to_string (m_frameDimensions.y) + ").");
    }

    const auto  textureWidth = m_textureSpace.width(), 
                textureHeight = m_textureSpace.height();

    // We need to check if the texture will actually need to be drawn.
    Rectangle<int>  texture     {   point.x,
                                    point.y,
                                    point.x + textureWidth - 1,
                                    point.y + textureHeight - 1 };

    // Determine the frame offset and check if the texture size needs changing.
    Point frameOffset { 0, 0 };

    // If m_frames is zero it's a single texture and therefore do not need to calculate offsets.
    if (m_frames != 0)
    {
        const unsigned int  frameWidth = textureWidth / m_frameDimensions.x,
                            frameHeight = textureHeight / m_frameDimensions.y;

        // Update the texture co-ordinates.
        texture.setRight (point.x + frameWidth - 1);
        texture.setBottom (point.y + frameHeight - 1);

        // Calculate how much we need to translate by.
        frameOffset.x = frame.x * frameWidth, 0;
        frameOffset.y = frame.y * frameHeight, 0;
    }
    
    // We will only draw if the texture is out-of-bounds.
    if (targetSpace.intersects (texture))
    {
        // Check if any clipping is necessary.
        if (!targetSpace.contains (texture))
        {
            // Clip the rectangle.
            texture.clipTo (targetSpace);
        }
            
        // Translate back to texture space, ready for blitting.
        texture.translate (-point.x, -point.y);

        // Call the correct blitting function.
        switch (blend)
        {
            case BlendType::Opaque:
                blitOpaque (target, targetSpace, point, frameOffset, texture);
                break;

            case BlendType::Transparent:
                blitTransparent (target, targetSpace, point, frameOffset, texture);
                break;
        }
    }

    // Do nothing if it doesn't intersect.
}


void Texture::blit (Texture& target, const Point& point, const BlendType blend, const Point& frame)
{
    // Forward onto the normal blitting function.
    blit (target.m_pData, target.m_textureSpace, point, blend, frame);
}


void Texture::blitOpaque (BYTE* const target, const Rectangle<int>& targetSpace, const Point& point, const Point& frameOffset, const Rectangle<int>& drawArea)
{    
    // Cache zee variables captain!
    const auto  blitWidth       = (size_t) drawArea.width(),
                blitHeight      = (size_t) drawArea.height();
               
    const auto  dataWidth       = blitWidth * sizeOfColour,
                targetWidth     = targetSpace.width() * sizeOfColour,
                textureWidth    = m_textureSpace.width() * sizeOfColour,
                
                dataOffset      = (drawArea.getLeft() + frameOffset.x) * sizeOfColour + (drawArea.getTop() + frameOffset.y) * textureWidth,
                targetOffset    = (point.x + drawArea.getLeft()) * sizeOfColour + (point.y + drawArea.getTop()) * targetWidth;
    
    // Obtain the data from the texture.
    const auto  textureData     = m_pData + dataOffset;
    
    // Calculate the starting pointer to the target position.
    BYTE* targetLine = target + targetOffset;    
    BYTE* currentLine = nullptr;

    for (size_t y = 0; y < blitHeight; ++y)
    {
        // Increment the pointer and copy line-by-line.
        currentLine = targetLine + y * targetWidth;
        std::memcpy (currentLine, (textureData + y * textureWidth), dataWidth);
    }
}


void Texture::blitTransparent (BYTE* const target, const Rectangle<int>& targetSpace, const Point& point, const Point& frameOffset, const Rectangle<int>& drawArea)
{
    // Cache zee variables captain!
    const auto  blitWidth       = (size_t) drawArea.width(),
                blitHeight      = (size_t) drawArea.height(),
               
                dataWidth       = blitWidth * sizeOfColour,
                targetWidth     = targetSpace.width() * sizeOfColour,
                textureWidth    = m_textureSpace.width() * sizeOfColour,
                
                dataOffset      = (drawArea.getLeft() + frameOffset.x) * sizeOfColour + (drawArea.getTop() + frameOffset.y) * textureWidth,
                targetOffset    = (point.x + drawArea.getLeft()) * sizeOfColour + (point.y + drawArea.getTop()) * targetWidth;

    // Create the required pointers for the blitting process.
    auto        currentData     = m_pData + dataOffset;
    auto        currentPixel    = target + targetOffset;

    // Avoid those magic constants!
    const auto  alphaIndex = 3U;

    for (size_t y = 0; y < blitHeight; ++y)
    {
        for (size_t x = 0; x < blitWidth; ++x)
        {
            const auto alpha = currentData[alphaIndex];

            // Avoid unnecessary blending when alpha is 0 or 255.
            switch (alpha)
            {
                case 0:
                    break;

                case 255:
                    for (auto i = 0U; i < alphaIndex; ++i)
                    {
                        currentPixel[i] = currentData[i];
                    }

                    break;

                default:                     
                    for (auto i = 0U; i < alphaIndex; ++i)
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
        currentPixel += targetWidth - dataWidth;
        currentData += textureWidth - dataWidth;
    }
}

#pragma endregion