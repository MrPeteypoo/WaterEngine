#include <Rendering/RendererHAPI.h>


// STL headers.
#include <exception>
#include <iostream>
#include <unordered_map>


// Engine headers.
#include <Systems.h>
#include <ILogger.h>
#include <Rendering/HAPITexture.h>
#include <Utility/Maths.h>


// Third party headers.
#include <HAPI/HAPI_lib.h>


// Forward declarations.
using       Colour       = HAPI_TColour;
const auto  channel      = 255U;
const auto  sizeOfColour = sizeof (Colour);


// Engine namespace
namespace water
{
    #pragma region Implmentation data

    /// <summary> A POD structure with all working data for the RendererHAPI class. </summary>
    struct RendererHAPI::Impl final
    {
        BYTE*                                   screen      { nullptr };    //!< A pointer to the memory address of the screen buffer.
        Rectangle<int>                          screenSpace { };            //!< A rectangle representing the screen space, used for clipping.
        Vector2<float>                          unitToPixel { };            //!< The scalar applied to world units to create a pixel-space vector.
        std::hash<std::string>                  hasher      { };            //!< A hashing function used to speed up map lookup at the expense of map insertion.
        std::unordered_map<TextureID, Texture>  textures    { };            //!< A container for all loaded texture data.
    };

    #pragma endregion


    #pragma region Constructors and destructor

    RendererHAPI::RendererHAPI()
    {
        // Ensure we have our implementation data.
        m_impl = new Impl();
    }


    RendererHAPI::RendererHAPI (RendererHAPI&& move)
    {
        // Just use the operator implementation.
        *this = std::move (move);
    }


    RendererHAPI& RendererHAPI::operator= (RendererHAPI&& move)
    {
        if (this != &move)
        {
            // Ensure we don't have memory leaks.
            if (m_impl)
            {
                delete m_impl;
            }

            // Take ownership of the movee implementation data.
            m_impl = move.m_impl;
            move.m_impl = nullptr;
        }

        return *this;
    }


    RendererHAPI::~RendererHAPI()
    {
        // Clean up after ourselves.
        if (m_impl)
        {
            delete m_impl;
            m_impl = nullptr;
        }
    }

    #pragma endregion


    #pragma region Initialisation

    void RendererHAPI::initialise (const int screenWidth, const int screenHeight, const Vector2<float>& unitToPixelScale)
    {
        // Pre-condition: Width and height are valid.
        if (screenWidth <= 0 || screenHeight <= 0)
        {
            throw std::invalid_argument ("RendererHAPI::initialise(): Invalid screen resolution given (" + 
                                          std::to_string (screenWidth) + "x" + std::to_string (screenHeight) + ").");
        }

        // Pre-condition: Pixel scale is valid.
        if (unitToPixelScale.x <= 0 || unitToPixelScale.y <= 0)
        {
            throw std::invalid_argument ("RendererHAPI::initialise(): Invalid scale values given (" +
                                          std::to_string (unitToPixelScale.x) + "x" + std::to_string (unitToPixelScale.y) + ").");
       }

        // Initialise data.
        m_impl->screen = HAPI->GetScreenPointer();
        m_impl->screenSpace = { 0, 0, screenWidth - 1, screenHeight - 1 };
        m_impl->unitToPixel = unitToPixelScale;

        // Ensure the screen pointer is valid.
        if (!m_impl->screen)
        {
            throw std::runtime_error ("RendererHAPI::initialise(): Failed to obtain a pointer to the screenbuffer.");
        }
    }


    void RendererHAPI::clearTextureData()
    {
        m_impl->textures.clear();
    }


    TextureID RendererHAPI::createBlankTexture (const Vector2<float>& textureDimensions, const Point& frameDimensions, const bool pixelDimensions)
    {
        // We know that all valid string values will end in .* so we can just increment an integer value and guarantee we'll have a valid
        // hashed TextureID at the end of it!
        static TextureID blankID = 0;

        // Determine the correct dimensions.
        const auto dimensions = pixelDimensions ? (Point) textureDimensions : (Point) (textureDimensions * m_impl->unitToPixel);

        // Determine the texture ID.
        const auto textureID = m_impl->hasher (std::to_string (blankID++));
    
        // Attempt to creae the blank texture.
        Texture texture { }; 

        if (texture.initialise (dimensions, frameDimensions))
        {
            // Finally load the texture and return the ID.
            m_impl->textures.emplace (textureID, std::move (texture));

            return textureID;
        }

        // We has an error captain!
        Systems::getLogger().logError ("RendererHAPI::createBlankTexture(), unable to create a blank texture with dimensions " + 
                                        std::to_string (dimensions.x) + "x" + std::to_string (dimensions.y) + ".");

        return 0;
    }


    TextureID RendererHAPI::loadTexture (const std::string& fileLocation, const Point& frameDimensions)
    {
        // Determine the texture ID.
        const auto textureID = m_impl->hasher (fileLocation);

        // Check if it already exists.
        if (m_impl->textures.count (textureID) == 0)
        {
            // Attempt to load the texture.
            Texture texture { };

            if (texture.initialise (fileLocation, frameDimensions))
            {
                // Add it to the unordered map.
                m_impl->textures.emplace (textureID, std::move (texture));
            
                return textureID;
            }
            
            Systems::getLogger().logError ("RendererHAPI::loadTexture(), unable to initialise texture at \"" + fileLocation + "\".");
        }

        return (TextureID) 0;
    }


    void RendererHAPI::scaleTexture (const TextureID target, const Vector2<float>& dimensions, const bool pixelUnits)
    {
        // Ensure the texture actually exists.
        auto& iterator = m_impl->textures.find (target);

        if (iterator != m_impl->textures.end())
        {
            // Scale the dimensions if necessary.
            const Point size = pixelUnits ? dimensions : dimensions * m_impl->unitToPixel;

            // Inform the texture to change its dimensions.
            if (!iterator->second.scaleToSize (size))
            {
                // The texture will still be valid so just output an error.
                Systems::getLogger().logWarning ("RendererHAPI::scaleTexture(), unable to scale texture to " +
                                                  std::to_string (size.x) + "x" + std::to_string (size.y) + ".");
            }
        }
    }


    #pragma endregion



    #pragma region Rendering


    void RendererHAPI::clearToBlack (const float blackLevel)
    {
        const auto black = (BYTE) (channel * util::clamp (blackLevel, 0.f, 1.f));

        // Use memset for efficiency.
        std::memset (m_impl->screen, black, m_impl->screenSpace.area() * sizeOfColour);
    }


    void RendererHAPI::clearToColour (const float red, const float green, const float blue, const float alpha)
    {
        const auto      screenSize  =   m_impl->screenSpace.area();
        const Colour    colour      {   (BYTE) (channel * red),
                                        (BYTE) (channel * green), 
                                        (BYTE) (channel * blue), 
                                        (BYTE) (channel * alpha) };

        // Loop through each pixel
        for (auto i = 0; i < screenSize; ++i)
        {
            // Find the correct memory address
            auto pixel = m_impl->screen + i * sizeOfColour;

            std::memcpy (pixel, &colour, sizeOfColour);
        }
    }


    void RendererHAPI::drawToScreen (const Vector2<float>& point, const TextureID id, const BlendType blend)
    {
        drawToScreen (point, id, blend, { 0, 0 });
    }


    void RendererHAPI::drawToScreen (const Vector2<float>& point, const TextureID id, const BlendType blend, const Point& frame)
    {
        // Check if the texture exists.
        auto& iterator = m_impl->textures.find (id);

        if (iterator != m_impl->textures.end())
        {
            // Convert the world-space units to pixel-space.
            const auto pixelSpace = (Point) (point * m_impl->unitToPixel);

            // Blit the valid texture.
            if (!iterator->second.blit (m_impl->screen, m_impl->screenSpace, pixelSpace, blend, frame))
            {
                // Output a silly error!
                Systems::getLogger().logError ("RendererHAPI::drawToScreen(), unable to blit texture, ensure frame \"" + 
                                                std::to_string (frame.x) + "x" + std::to_string (frame.y) + "\" is valid.");
            }
        }

        else
        {
            // We have a problem, inform the logger!
            Systems::getLogger().logWarning ("RendererHAPI::drawToScreen(), attempt to draw an invalid texture.");
        }
    }


    void RendererHAPI::drawToTexture (const Vector2<float>& point, const TextureID source, const TextureID target, const BlendType blend)
    {
        drawToTexture (point, source, target, blend, { 0, 0 });
    }


    void RendererHAPI::drawToTexture (const Vector2<float>& point, const TextureID source, const TextureID target, const BlendType blend, const Point& frame)
    {
        // Obtain the iterators to both textures.
        auto& sourceIterator = m_impl->textures.find (source);
        auto& targetIterator = m_impl->textures.find (target);

        // Ensure that both textures exist.
        if (sourceIterator != m_impl->textures.end() && targetIterator != m_impl->textures.end())
        {
            // Convert the point into pixel space.
            const auto pixelSpace = (Point) (point * m_impl->unitToPixel);

            // Blit the source texture onto the target.
            if (!sourceIterator->second.blit (targetIterator->second, pixelSpace, blend, frame))
            {
                // Output a silly error!
                Systems::getLogger().logError ("RendererHAPI::drawToScreen(), unable to blit texture, ensure frame \"" + 
                                                std::to_string (frame.x) + "x" + std::to_string (frame.y) + "\" is valid.");            
            }
        }

        else
        {
            Systems::getLogger().logWarning ("RendererHAPI::drawToTexture(), invalid source or target texture IDs given.");
        }
    }

    #pragma endregion Rendering
}