#include "HAPITexture.hpp"


// Engine headers.
#include <Utility/Maths.hpp>


// Third party headers.
#include <HAPI/HAPI_lib.h>


// Forward declarations.
const auto sizeOfColour = sizeof (HAPI_TColour);


// Engine namespace.
namespace water
{
    #pragma region Constructors and destructor

    HAPITexture::HAPITexture (HAPITexture&& move)
    {
        // Just use the operator implementation.
        *this = std::move (move);
    }


    HAPITexture& HAPITexture::operator= (HAPITexture&& move)
    {
        if (this != &move)
        {
            cleanUp();

            // Obtain all of the data from the movee.
            m_frames            = move.m_frames;

            m_frameDimensions   = std::move (move.m_frameDimensions);
            m_textureSpace      = std::move (move.m_textureSpace);
            m_data              = move.m_data;


            // Reset the movee.
            move.m_frames       = 0;
            move.m_data         = nullptr;
        }

        return *this;
    }


    HAPITexture::~HAPITexture()
    {
        cleanUp();
    }

    #pragma endregion


    #pragma region Initialisation

    bool HAPITexture::initialise (const std::string& fileLocation)
    {   
        // Attempt to load the texture during construction.
        return loadTexture (fileLocation);
    }


    bool HAPITexture::initialise (const std::string& fileLocation, const Point& frameDimensions)
    {
        // Attempt to load the texture during construction.
        const bool result = loadTexture (fileLocation);

        // Attempt to set the frame dimensions.
        setFrameDimensions (frameDimensions);

        return result;
    }


    bool HAPITexture::initialise (const Point& pixelDimensions)
    {
        // Load all the data captain!
        return fillWithBlankData (pixelDimensions);
    }


    bool HAPITexture::initialise (const Point& pixelDimensions, const Point& frameDimensions)
    {
        // Load all the data captain!
        const bool result = fillWithBlankData (pixelDimensions);

        // Set up the spritesheet!
        setFrameDimensions (frameDimensions);

        return result;
    }

    #pragma endregion


    #pragma region Getters and setters

    void HAPITexture::resetFrameDimensions()
    {
        m_frames = 0;
        m_frameDimensions.x = 0;
        m_frameDimensions.y = 0;
    }


    void HAPITexture::setFrameDimensions (const Point& dimensions)
    {
        if (dimensions.x <= 0 || dimensions.y <= 0)
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
    }

    #pragma endregion


    #pragma region Loading functionality

    void HAPITexture::cleanUp()
    {
        if (m_data)
        {
            delete[] m_data;
            m_data = nullptr;
        }
    }


    bool HAPITexture::loadTexture (const std::string& fileLocation)
    {
        // Make sure we don't leak memory.
        cleanUp();

        // Create variable cache for interfacing with HAPI.
        int width = 0, height = 0;

        // Attempt to load the data. If the loading succeeds then the width and height will be set.
        if (HAPI->LoadTexture (fileLocation, &m_data, &width, &height))
        {
            resetFrameDimensions();
        
            // Initialise the texture space.
            m_textureSpace = { 0, 0, width - 1, height - 1 };

            return true;
        }
    
        return false;
    }


    bool HAPITexture::fillWithBlankData (const Point& dimensions)
    {
        // Pre-condition: Ensure the dimensions are valid.
        if (dimensions.x <= 0 || dimensions.y <= 0)
        {
            return false;
        }

        // Make sure we don't leak memory.
        cleanUp();

        // Allocate enough data.
        const auto size = dimensions.x * dimensions.y;
        m_data = new BYTE[size * sizeOfColour];

        // Check whether the allocation was successful.
        if (!m_data)
        {
            return false;
        }

        // Prepare the texture.
        resetFrameDimensions();

        m_textureSpace = { 0, 0, dimensions.x - 1, dimensions.y - 1 };

        clearToBlack();

        return true;
    }

    #pragma endregion


    #pragma region Scaling

    bool HAPITexture::scaleByFactor (const Vector2<float>& ratios, const FilterMode mode)
    {
        // Pre-condition: Check if we need to scale at all.
        if (mode == FilterMode::None)
        {
            return true;
        }

        // Pre-condition: Ensure the ratios aren't 1:1
        if (ratios.x == 1.f && ratios.y == 1.f)
        {
            return true;
        }

        // Work on the basis of colours instead of bytes for the scaling.
        const auto  width       = m_textureSpace.width(),
                    height      = m_textureSpace.height();

        const auto  newWidth    = (int) (width * ratios.x),
                    newHeight   = (int) (height * ratios.y);

        // Allocate the required data.
        Colour* scaledData      = new Colour[newWidth * newHeight];

        // Ensure the allocation is valid.
        if (!scaledData)
        {
            return false;
        }

        // Filter each pixel.
        for (int y = 0; y < newHeight; ++y)
        {
            for (int x = 0; x < newWidth; ++x)
            {
                // We need to find where we are in terms of the unscaled data.
                const float unscaledX = (float) x / newWidth * (width - 1),
                            unscaledY = (float) y / newHeight * (height - 1);

                // Now we need to use the correct algorithm.
                switch (mode)
                {
                    case FilterMode::NearestNeighbour:
                        scaledData[x + y * newWidth] = nearestNeighbourPixel (*this, unscaledX, unscaledY, width);
                        break;
                    
                    case FilterMode::Bilinear:
                        scaledData[x + y * newWidth] = bilinearFilteredPixel (*this, unscaledX, unscaledY, width);
                        break;

                    // Not implemented!
                    default:
                        return false;
                }
            }
        }

        // Clean up the old texture data and replace it with the new data.
        cleanUp();
        m_data          = (BYTE*) scaledData;
        m_textureSpace  = { 0, 0, newWidth - 1, newHeight - 1 };

        // Reset the frame calculations.
        setFrameDimensions (m_frameDimensions);

        return true;
    }


    Colour HAPITexture::nearestNeighbourPixel (const HAPITexture& source, const float x, const float y, const int width)
    {
        // We need to floor the X and Y values then return the correct pixel.
        const auto px    = (int) x, py = (int) y;

        const auto pixel = (Colour*) (source.m_data) + px + py * width;

        return *pixel;
    }


    Colour HAPITexture::bilinearFilteredPixel (const HAPITexture& source, const float x, const float y, const int width)
    {
        /// This code is a modified version of a very useful blog post.
        /// theowl84 (2011) 'Bilinear Pixel Interpolation using SSE', FastC++: Coding Cpp Efficiently, 16 June.
        /// Available at: http://fastcpp.blogspot.co.uk/2011/06/bilinear-pixel-interpolation-using-sse.html (Accessed: 07/02/2015).

        // Start by flooring X and Y so we can interpolate between pixels.
        const auto  px  = (int) x,
                    py  = (int) y;

        // Obtain the base pixel.
        const auto  p0  = (Colour*) (source.m_data) + px + py * width;
 
        // Obtain a reference to the four neighbouring pixels.
        const auto& p1  = p0[0 + 0 * width],
                    p2  = p0[1 + 0 * width],
                    p3  = p0[0 + 1 * width],
                    p4  = p0[1 + 1 * width];
 
        // Calculate the weights for each pixel. This is where we determine how much to blend each neighbour by.
        const auto  fx  = x - px,
                    fy  = y - py,
                    fx1 = 1.0f - fx,
                    fy1 = 1.0f - fy;
  
        const auto  w1  = (BYTE) (fx1 * fy1 * 256.f),
                    w2  = (BYTE) (fx  * fy1 * 256.f),
                    w3  = (BYTE) (fx1 * fy * 256.f),
                    w4  = (BYTE) (fx  * fy * 256.f);
 
        // Interpolate each channel, for cache reasons calculate in BGRA order.
        const BYTE  b   = (p1.blue * w1 + p2.blue * w2 + p3.blue * w3 + p4.blue * w4) >> 8,
                    g   = (p1.green * w1 + p2.green * w2 + p3.green * w3 + p4.green * w4) >> 8,
                    r   = (p1.red * w1 + p2.red * w2 + p3.red * w3 + p4.red * w4) >> 8,
                    a   = (p1.alpha * w1 + p2.alpha * w2 + p3.alpha * w3 + p4.alpha * w4) >> 8;
 
        return { r, g, b, a };
    }


    void HAPITexture::crop (const Point& crop)
    {
        // Don't do anything if no cropping is to be performed.
        if (crop.x != 0 || crop.y != 0)
        {
            // Work on the basis of bytes for cropping.
            const auto  width       = m_textureSpace.width(),
                        height      = m_textureSpace.height(),
                        copyWidth   = width * (int) sizeOfColour;

            // Determine the new width and height values. Require at least 1x1 size.
            const auto  newWidth    = util::max (width - crop.x, 1),
                        newHeight   = util::max (height - crop.y, 1),
                        newOffset   = newWidth * (int) sizeOfColour;                        

            // Allocate the required data.
            BYTE* scaledData    = new BYTE[newOffset * newHeight];

            // Ensure the allocation is valid.
            if (scaledData)
            {
                // Use memcpy to move the data across.
                for (int y = 0; y < newHeight; ++y)
                {
                    // Calculte the pointers to the memory we'll be using.
                    auto source = m_data + y * copyWidth,
                         target = scaledData + y * newOffset;

                    std::memcpy (target, source, newOffset);
                }

                // Clean up the old texture data and replace it with the new data.
                cleanUp();
                m_data          = scaledData;
                m_textureSpace  = { 0, 0, newWidth - 1, newHeight - 1 };

                // Reset the frame calculations.
                setFrameDimensions (m_frameDimensions);
            }
        }    
    }

    #pragma endregion


    #pragma region Blitting

    bool HAPITexture::blitTo (BYTE* const target, const Rectangle<int>& blitTo, const Rectangle<int>& clipTo, const Point& point, const Point& frame, const BlendType blend)
    {
        // Pre-condition: target is a valid pointer.
        if (!target)
        {
            return false;
        }

        // Pre-condition: frame is valid.
        if (m_frames != 0 && 
           (frame.x >= m_frameDimensions.x || frame.x < 0 ||
            frame.y >= m_frameDimensions.y || frame.y < 0))
        {
            return false;
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
            const float frameWidth  = textureWidth / (float) m_frameDimensions.x,
                        frameHeight = textureHeight / (float) m_frameDimensions.y;

            // Update the texture co-ordinates.
            texture.setRight (point.x + (int) frameWidth - 1);
            texture.setBottom (point.y + (int) frameHeight - 1);

            // Calculate how much we need to translate by.
            frameOffset.x = (int) (frame.x * frameWidth);
            frameOffset.y = (int) (frame.y * frameHeight);
        }
    
        // We will only draw if the texture is out-of-bounds.
        if (clipTo.intersects (texture))
        {
            // Check if any clipping is necessary.
            if (!clipTo.contains (texture))
            {
                // Clip the rectangle.
                texture.clipTo (clipTo);
            }
            
            // Translate back to texture space, ready for blitting.
            texture.translate (-point.x, -point.y);

            // Call the correct blitting function.
            switch (blend)
            {
                case BlendType::Opaque:
                    blitOpaque (target, blitTo, point, frameOffset, texture);
                    break;

                case BlendType::Transparent:
                    blitTransparent (target, blitTo, point, frameOffset, texture);
                    break;
            }
        }

        // Do nothing if it doesn't intersect.
        return true;
    }


    bool HAPITexture::blitTo (HAPITexture& target, const Point& point, const Point& frame, const BlendType blend)
    {
        // Forward onto the normal blitting function.
        return blitTo (target.m_data, target.m_textureSpace, target.m_textureSpace, point, frame, blend);
    }
    

    void HAPITexture::blitOpaque (BYTE* const target, const Rectangle<int>& targetSpace, const Point& point, const Point& frameOffset, const Rectangle<int>& drawArea)
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
        const auto  textureData     = m_data + dataOffset;
    
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


    void HAPITexture::blitTransparent (BYTE* const target, const Rectangle<int>& targetSpace, const Point& point, const Point& frameOffset, const Rectangle<int>& drawArea)
    {
        // Cache zee variables captain!
        const auto  blitWidth       = (size_t) drawArea.width(),
                    blitHeight      = (size_t) drawArea.height(),
               
                    dataWidth       = blitWidth * sizeOfColour,
                    targetWidth     = targetSpace.width() * sizeOfColour,
                    textureWidth    = m_textureSpace.width() * sizeOfColour,
                
                    dataOffset      = (drawArea.getLeft() + frameOffset.x) * sizeOfColour + (drawArea.getTop() + frameOffset.y) * textureWidth,
                    targetOffset    = (point.x + drawArea.getLeft()) * sizeOfColour + (point.y + drawArea.getTop()) * targetWidth,

                    sourceIncrement = textureWidth - dataWidth,
                    targetIncrement = targetWidth - dataWidth;

        // Create the required pointers for the blitting process.
        auto        currentData     = m_data + dataOffset;
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
            currentPixel += targetIncrement;
            currentData += sourceIncrement;
        }
    }


    void HAPITexture::clearToBlack (const BYTE blackLevel)
    {
        std::memset (m_data, blackLevel, m_textureSpace.area() * sizeOfColour);
    }


    void HAPITexture::clearToColour (const Colour& colour)
    {
        // Loop through each pixel
        const auto size = m_textureSpace.area();
        for (auto i = 0; i < size; ++i)
        {
            // Find the correct memory address
            auto pixel = m_data + i * sizeOfColour;

            std::memcpy (pixel, &colour, sizeOfColour);
        }
    }

    #pragma endregion
}