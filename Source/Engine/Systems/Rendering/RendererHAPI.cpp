#include "RendererHAPI.hpp"


// STL headers.
#include <exception>
#include <iostream>
#include <unordered_map>


// Engine headers.
#include <Systems.hpp>
#include <Systems/Rendering/HAPITexture.hpp>
#include <Utility/Maths.hpp>


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
        float                                   unitToPixel { };            //!< The scalar applied to world units to create a pixel-space vector.
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


    #pragma region System management

    void RendererHAPI::initialise (const unsigned int screenWidth, const unsigned int screenHeight, const float unitToPixelScale)
    {
        // Pre-condition: Width and height are valid.
        if (screenWidth == 0 || screenHeight == 0)
        {
            throw std::invalid_argument ("RendererHAPI::initialise(): Invalid screen resolution given (" + 
                                          std::to_string (screenWidth) + "x" + std::to_string (screenHeight) + ").");
        }

        // Pre-condition: Pixel scale is valid.
        if (unitToPixelScale <= 0)
        {
            throw std::invalid_argument ("RendererHAPI::initialise(): Invalid scale values given (" +
                                          std::to_string (unitToPixelScale) + ").");
        }


        // Initialise HAPI.
        int width = (int) screenWidth, height = (int) screenHeight;

        if (!HAPI->Initialise (&width, &height))
        {
            throw std::runtime_error ("RendererHAPI::initialise(), unable to initialise HAPI.");
        }

        #if defined _DEBUG || defined SHOW_FPS

        HAPI->SetShowFPS (true);

        #endif

        // Initialise data.
        m_impl->screen = HAPI->GetScreenPointer();
        m_impl->screenSpace = { 0, 0, width - 1, height - 1 };
        m_impl->unitToPixel = unitToPixelScale;

        // Ensure the screen pointer is valid.
        if (!m_impl->screen)
        {
            throw std::runtime_error ("RendererHAPI::initialise(): Failed to obtain a pointer to the screenbuffer.");
        }
    }


    bool RendererHAPI::update()
    {
        if (HAPI->Update())
        {
            clearToBlack();
            return true;
        }

        return false;
    }

    #pragma endregion


    #pragma region Data management

    TextureID RendererHAPI::loadTexture (const std::string& fileLocation)
    {
        // Determine the texture ID.
        const auto textureID = m_impl->hasher (fileLocation);

        // Check if it already exists.
        if (m_impl->textures.count (textureID) == 0)
        {
            // Attempt to load the texture.
            Texture texture { };

            if (texture.initialise (fileLocation))
            {
                // Add it to the unordered map.
                m_impl->textures.emplace (textureID, std::move (texture));
            
                return textureID;
            }
            
            Systems::logger().logError ("RendererHAPI::loadTexture(), unable to initialise texture at \"" + fileLocation + "\".");
        }

        else
        {
            // Return the ID of the texture since it's already loaded!
            return textureID;
        }

        return (TextureID) 0;
    }
    

    TextureID RendererHAPI::createBlankTexture (const Vector2<float>& textureDimensions, const bool pixelDimensions)
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

        if (texture.initialise (dimensions))
        {
            // Finally load the texture and return the ID.
            m_impl->textures.emplace (textureID, std::move (texture));

            return textureID;
        }

        // We has an error captain!
        Systems::logger().logError ("RendererHAPI::createBlankTexture(), unable to create a blank texture with dimensions " + 
                                        std::to_string (dimensions.x) + "x" + std::to_string (dimensions.y) + ".");

        return 0;
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
                Systems::logger().logWarning ("RendererHAPI::scaleTexture(), unable to scale texture to " +
                                                  std::to_string (size.x) + "x" + std::to_string (size.y) + ".");
            }
        }
        
        else
        {
            Systems::logger().logWarning ("RendererHAPI::scaleTexture(), attempt to scale non-existent texture.");
        }
    }


    void RendererHAPI::cropTexture (const TextureID target, const int right, const int bottom)
    {
        // Pre-condition: Right and bottom are actually valid values.
        if (right >= 0 && bottom >= 0)
        {
            // Check if the texture is valid.
            auto& iterator = m_impl->textures.find (target);

            if (iterator != m_impl->textures.end())
            {
                // Scale if necessary.
                const Point crop { right, bottom };

                iterator->second.crop (crop);
            }

            else
            {
                Systems::logger().logWarning ("RendererHAPI::cropTexture(), attempt to crop non-existent texture.");
            }
        }

        else
        {
            Systems::logger().logError ("RendererHAPI::cropTexture(), attempt to crop a texture with negative cropping values. Request will be ignored.");
        }
    }


    void RendererHAPI::setFrameDimensions (const TextureID target, const Point& dimensions)
    {
        // Attempt to find the texture.
        auto& iterator = m_impl->textures.find (target);

        if (iterator != m_impl->textures.end())
        {
            iterator->second.setFrameDimensions (dimensions);
        }

        else
        {
            Systems::logger().logWarning ("RendererHAPI::setFrameDimensions(), attempt to set the dimensions of a non-existent texture.");
        }
    }


    void RendererHAPI::removeTexture (const TextureID texture)
    {
        // Log whether anything was actually erased.
        if (m_impl->textures.erase (texture) == 0)
        {
            Systems::logger().logWarning ("RendererHAPI::removeTexture(), attempt to erase non-existent texture.");
        }
    }


    void RendererHAPI::clearTextureData()
    {
        m_impl->textures.clear();
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
                Systems::logger().logError ("RendererHAPI::drawToScreen(), unable to blit texture, ensure frame \"" + 
                                                std::to_string (frame.x) + "x" + std::to_string (frame.y) + "\" is valid.");
            }
        }

        else
        {
            // We have a problem, inform the logger!
            Systems::logger().logWarning ("RendererHAPI::drawToScreen(), attempt to draw an invalid texture.");
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
                Systems::logger().logError ("RendererHAPI::drawToScreen(), unable to blit texture, ensure frame \"" + 
                                                std::to_string (frame.x) + "x" + std::to_string (frame.y) + "\" is valid.");            
            }
        }

        else
        {
            Systems::logger().logWarning ("RendererHAPI::drawToTexture(), invalid source or target texture IDs given.");
        }
    }

    #pragma endregion Rendering
}