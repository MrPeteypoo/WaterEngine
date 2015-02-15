#include "HAPITexture.hpp"


// Third party headers.
#include <HAPI/HAPI_lib.h>


// Forward declarations.
const auto sizeOfColour = sizeof (HAPI_TColour);


// Engine namespace.
namespace water
{
    #pragma region Constructors and destructor

    RendererHAPI::Texture::Texture (Texture&& move)
    {
        // Just use the operator implementation.
        *this = std::move (move);
    }


    RendererHAPI::Texture& RendererHAPI::Texture::operator= (Texture&& move)
    {
        if (this != &move)
        {
            // Obtain all of the data from the movee.
            m_frames = move.m_frames;

            m_frameDimensions = std::move (move.m_frameDimensions);
            m_textureSpace = std::move (move.m_textureSpace);
            m_data = move.m_data;


            // Reset the movee.
            move.m_frames = 0;
            move.m_data = nullptr;
        }

        return *this;
    }


    RendererHAPI::Texture::~Texture()
    {
        cleanUp();
    }

    #pragma endregion


    #pragma region Initialisation

    bool RendererHAPI::Texture::initialise (const std::string& fileLocation)
    {   
        // Attempt to load the texture during construction.
        return loadTexture (fileLocation);
    }


    bool RendererHAPI::Texture::initialise (const std::string& fileLocation, const Point& frameDimensions)
    {
        // Attempt to load the texture during construction.
        const bool result = loadTexture (fileLocation);

        // Attempt to set the frame dimensions.
        setFrameDimensions (frameDimensions);

        return result;
    }


    bool RendererHAPI::Texture::initialise (const Point& pixelDimensions)
    {
        // Load all the data captain!
        return fillWithBlankData (pixelDimensions);
    }


    bool RendererHAPI::Texture::initialise (const Point& pixelDimensions, const Point& frameDimensions)
    {
        // Load all the data captain!
        const bool result = fillWithBlankData (pixelDimensions);

        // Set up the spritesheet!
        setFrameDimensions (frameDimensions);

        return result;
    }

    #pragma endregion


    #pragma region Getters and setters

    void RendererHAPI::Texture::resetFrameDimensions()
    {
        m_frames = 0;
        m_frameDimensions.x = 0;
        m_frameDimensions.y = 0;
    }


    void RendererHAPI::Texture::setFrameDimensions (const Point& dimensions)
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

    void RendererHAPI::Texture::cleanUp()
    {
        if (m_data)
        {
            delete[] m_data;
            m_data = nullptr;
        }
    }


    bool RendererHAPI::Texture::loadTexture (const std::string& fileLocation)
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


    bool RendererHAPI::Texture::fillWithBlankData (const Point& dimensions)
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
        m_data = new BYTE[size];

        // Check whether the allocation was successful.
        if (!m_data)
        {
            return false;
        }

        // Prepare the texture.
        resetFrameDimensions();

        m_textureSpace = { 0, 0, dimensions.x, dimensions.y };

        // Ensure all data is black.
        for (auto i = 0; i < size; ++i)
        {
            m_data[i] = 0;
        }

        return true;
    }

    #pragma endregion


    #pragma region Scaling

    bool RendererHAPI::Texture::scaleToSize (const Point& dimensions)
    {
        // Pre-condition: Ensure the dimensions are valid.
        if (dimensions.x <= 0 || dimensions.y <= 0)
        {
            return false;
        }

        // Work on the basis of colours instead of bytes for the scaling.
        const auto  width   = m_textureSpace.width(),
                    height  = m_textureSpace.height();

        // Don't do anything if no scaling is to be performed.
        if (dimensions.x != width || dimensions.y != height)
        {
            const auto  floatWidth  = (float) dimensions.x,
                        floatHeight = (float) dimensions.y;

            // Allocate the required data.
            Colour* scaledData      = new Colour[dimensions.x * dimensions.y];

            // Ensure the allocation is valid.
            if (!scaledData)
            {
                return false;
            }

            // Filter each pixel.
            for (int y = 0; y < dimensions.y; ++y)
            {
                for (int x = 0; x < dimensions.x; ++x)
                {
                    // We need to find where we are in terms of the unscaled data.
                    const float unscaledX = x / floatWidth * width,
                                unscaledY = y / floatHeight * height;

                    // Now we need to calculate the current scaled pixel using bilinear filtering.
                    scaledData[x + y * dimensions.x] = bilinearFilteredPixel (*this, unscaledX, unscaledY, width, m_textureSpace.getRight(), m_textureSpace.getBottom());
                }
            }

            // Clean up the old texture data and replace it with the new data.
            cleanUp();
            m_data          = (BYTE*) scaledData;
            m_textureSpace  = { 0, 0, dimensions.x - 1, dimensions.y - 1 };

            // Reset the frame calculations.
            setFrameDimensions (m_frameDimensions);
        }

        return true;
    }


    Colour RendererHAPI::Texture::bilinearFilteredPixel (const Texture& source, const float x, const float y, const int width, const int right, const int bottom)
    {
        /// This code is a modified version of a very useful blog post.
        /// theowl84 (2011) 'Bilinear Pixel Interpolation using SSE', FastC++: Coding Cpp Efficiently, 16 June.
        /// Available at: http://fastcpp.blogspot.co.uk/2011/06/bilinear-pixel-interpolation-using-sse.html (Accessed: 07/02/2015).

        // Start by flooring X and Y so we can interpolate between pixels.
        const auto  px      = (int) x,
                    py      = (int) y;

        // Obtain the base pixel.
        const auto  p0      = (Colour*) (source.m_data) + px + py * width;
 
        // Obtain a reference to the four neighbouring pixels.
        const auto& p1      = p0[0 + 0 * width],
                    p2      = p0[1 + 0 * width],
                    p3      = p0[0 + 1 * width],
                    p4      = p0[1 + 1 * width];
 
        // Calculate the weights for each pixel. This is where we determine how much to blend each neighbour by. We need to clamp to prevent overshooting.
        const auto  fx      = x < right ? x - px : 0.f,
                    fy      = y < bottom ? y - py : 0.f,
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

        /*int w1 = fx1 * fy1 * 256.0f;
        int w2 = fx  * fy1 * 256.0f;
        int w3 = fx1 * fy  * 256.0f;
        int w4 = fx  * fy  * 256.0f;
 
        // Calculate the weighted sum of pixels (for each color channel)
        int outr = p1.r * w1 + p2.r * w2 + p3.r * w3 + p4.r * w4;
        int outg = p1.g * w1 + p2.g * w2 + p3.g * w3 + p4.g * w4;
        int outb = p1.b * w1 + p2.b * w2 + p3.b * w3 + p4.b * w4;
        int outa = p1.a * w1 + p2.a * w2 + p3.a * w3 + p4.a * w4;
 
        return Pixel(outr >> 8, outg >> 8, outb >> 8, outa >> 8);*/
    }


    void RendererHAPI::Texture::crop (const Point& crop)
    {
        // Don't do anything if no cropping is to be performed.
        if (crop.x != 0 || crop.y != 0)
        {
            // Work on the basis of bytes for cropping.
            const auto  width       = m_textureSpace.width(),
                        height      = m_textureSpace.height(),
                        copyWidth   = width * (int) sizeOfColour;

            // Determine the new width and height values. Require at least 1x1 size.
            const auto  newWidth    = crop.x >= width ? 1 : width - crop.x,
                        newHeight   = crop.y >= height ? 1 : height - crop.y,
                        newOffset   = newWidth * (int) sizeOfColour;                        

            // Allocate the required data.
            BYTE* scaledData    = new BYTE[newWidth * newHeight * sizeOfColour];

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

    bool RendererHAPI::Texture::blit (BYTE* const target, const Rectangle<int>& targetSpace, const Point& point, const BlendType blend, const Point& frame)
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

        return true;
    }


    bool RendererHAPI::Texture::blit (Texture& target, const Point& point, const BlendType blend, const Point& frame)
    {
        // Forward onto the normal blitting function.
        return blit (target.m_data, target.m_textureSpace, point, blend, frame);
    }


    void RendererHAPI::Texture::blitOpaque (BYTE* const target, const Rectangle<int>& targetSpace, const Point& point, const Point& frameOffset, const Rectangle<int>& drawArea)
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


    void RendererHAPI::Texture::blitTransparent (BYTE* const target, const Rectangle<int>& targetSpace, const Point& point, const Point& frameOffset, const Rectangle<int>& drawArea)
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
            currentPixel += targetWidth - dataWidth;
            currentData += textureWidth - dataWidth;
        }
    }


    void RendererHAPI::Texture::clearToBlack (const BYTE blackLevel)
    {
        std::memset (m_data, blackLevel, m_textureSpace.area() * sizeOfColour);
    }


    void RendererHAPI::Texture::clearToColour (const Colour& colour)
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