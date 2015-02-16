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
        BYTE*                                   screen          { nullptr };    //!< A pointer to the memory address of the screen buffer.
        Rectangle<int>                          screenSpace     { };            //!< A rectangle representing the screen space, used for blitting.
        Rectangle<int>                          drawSpace       { };            //!< The actual drawable area of the screen. Useful for clipping.
        Point                                   screenOffset    { 0, 0 };       //!< The offset to apply when upscaling, this is useful when maintaining the aspect ratio of the internal resolution.
        
        Vector2<int>                            internalRes     { };            //!< The internal resolution of the renderer, required for world to pixel conversion.
        Vector2<float>                          textureScale    { 1, 1 };       //!< The scale ratios used to filter loaded textures to a desired size.
        FilterMode                              filter          { };            //!< The filter to be applied during the framebuffer scaling.

        Rectangle<float>                        viewport        { };            //!< The visible area of the screen in world units.
        Vector2<float>                          worldToPixel    { };            //!< The scalar applied to world units to create a pixel-space vector.

        std::hash<std::string>                  hasher          { };            //!< A hashing function used to speed up map lookup at the expense of map insertion.
        std::unordered_map<TextureID, Texture>  textures        { };            //!< A container for all loaded texture data.
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

    void RendererHAPI::initialise (const int screenWidth, const int screenHeight, const int internalWidth, const int internalHeight, const FilterMode filter)
    {
        // Pre-condition: Screen resolution is valid.
        if (screenWidth <= 0 || screenHeight <= 0)
        {
            throw std::invalid_argument ("RendererHAPI::initialise(), invalid screen resolution given (" + 
                                          std::to_string (screenWidth) + "x" + std::to_string (screenHeight) + ").");
        }

        // Pre-condition: Internal resolution is valid.
        if (internalWidth <= 0 || internalHeight <= 0)
        {
            throw std::invalid_argument ("RendererHAPI::initialise(), invalid internal resolution given (" +
                                          std::to_string (internalWidth) + "x" + std::to_string (internalHeight) + ").");
        }

        // Pre-condition: If no filtering is applied the internal resolution can't be higher than the screen.
        if (filter == FilterMode::None && (screenWidth < internalWidth || screenHeight < internalHeight))
        {
            throw std::invalid_argument ("RendererHAPI::initialise(), attempt to set internal resolution higher than screen resolution with no scaling filter.");
        }

        // Attempt to initialise the renderer.
        initialiseHAPI (screenWidth, screenHeight);
        initialiseScaling (internalWidth, internalHeight, filter);
    }


    bool RendererHAPI::update()
    {
        if (HAPI->Update())
        {
            // Clear the screen area to black.
            std::memset (m_impl->screen, 0, m_impl->screenSpace.area() * sizeOfColour);
            return true;
        }

        return false;
    }

    #pragma endregion


    #pragma region Internal workings

    void RendererHAPI::initialiseHAPI (const int width, const int height)
    {
        // Prepare to initialise HAPI.
        int finalWidth = width, 
            finalHeight = height;

        if (!HAPI->Initialise (&finalWidth, &finalHeight))
        {
            throw std::runtime_error ("RendererHAPI::initialise(), unable to initialise HAPI.");
        }

        // Set the correct screen values.
        m_impl->screen          = HAPI->GetScreenPointer();
        m_impl->screenSpace     = { 0, 0, finalWidth - 1, finalHeight - 1 };
        m_impl->screenOffset    = { 0, 0 };

        // Ensure the screen pointer is valid.
        if (!m_impl->screen)
        {
            throw std::runtime_error ("RendererHAPI::initialise(): Failed to obtain a pointer to the screenbuffer.");
        }

        // Enable the FPS display if necessary.
        #if defined _DEBUG || defined SHOW_FPS

        HAPI->SetShowFPS (true);

        #endif
    }


    void RendererHAPI::initialiseScaling (const int internalWidth, const int internalHeight, const FilterMode filter)
    {
        // Set default values.
        m_impl->filter       = filter;
        m_impl->viewport     = { 0, 0, 1, 1 };

        // Prepare the internal resolution, required for world to pixel scaling.
        m_impl->internalRes = { internalWidth, internalHeight };

        // Ensure we have correct offset values.
        calculateScalingOffset();

        // We need to calculate the drawable area of the screen.
        const auto  screenWidth  = m_impl->screenSpace.width(),
                    screenHeight = m_impl->screenSpace.height();

        m_impl->drawSpace.setLeft (m_impl->screenOffset.x);
        m_impl->drawSpace.setTop (m_impl->screenOffset.y);
        m_impl->drawSpace.setRight (screenWidth - m_impl->screenOffset.x - 1);
        m_impl->drawSpace.setBottom (screenHeight - m_impl->screenOffset.y - 1);


        // We need to calculate the ratio between the internal resolution and the external resolution.
        const auto  drawWidth  = (float) m_impl->drawSpace.width(),
                    drawHeight = (float) m_impl->drawSpace.height();
        
        if (screenWidth == internalWidth && screenHeight == internalHeight)
        {
            m_impl->textureScale = { 1.f, 1.f };
        }

        else
        {
            m_impl->textureScale.x = drawWidth / internalWidth;
            m_impl->textureScale.y = drawHeight / internalHeight;
        }

        // We need to enable the correct world to unit scale.
        m_impl->worldToPixel = { drawWidth, drawHeight };
    }


    void RendererHAPI::calculateScalingOffset()
    {
        // We should just centre the screen if no scaling is to be applied.
        if (m_impl->filter == FilterMode::None)
        {
            // Simply set the offset to be the difference between the internal and external resolutions.
            m_impl->screenOffset.x = (m_impl->screenSpace.width() - m_impl->internalRes.x) / 2;
            m_impl->screenOffset.y = (m_impl->screenSpace.height() - m_impl->internalRes.y) / 2;
        }
        
        else
        {
            // We need to obtain the aspect ratio of the screen and the internal framebuffer and compare the two.
            const auto  screenWidth     = m_impl->screenSpace.width(),
                        screenHeight    = m_impl->screenSpace.height();

            const auto  screenRatio     = (float) screenWidth / screenHeight,
                        internalRatio   = (float) m_impl->internalRes.x / m_impl->internalRes.y;

            // If the internal ratio is wider we need to correct the Y offset.
            if (internalRatio > screenRatio)
            {
                // We need to divide by the aspect ratio to determine the drawable height of the screen!
                const auto desiredHeight = screenWidth / internalRatio;

                // We use half of the difference to have padding on the top and bottom of the screen.
                m_impl->screenOffset.y = (int) ((screenHeight - desiredHeight) / 2.f);
            }

            // If the internal ratio is narrower we need to correct the X offset.
            if (internalRatio < screenRatio)
            {
                // We need to multiply the current height by the internal ratio to calculate the desired width.
                const auto desiredWidth = screenHeight * internalRatio;

                // We use half of the difference to add padding on the left and right of the screen.
                m_impl->screenOffset.x = (int) ((screenWidth - desiredWidth) / 2.f);
            }
        }
    }

    #pragma endregion


    #pragma region Viewport

    void RendererHAPI::setViewport (const Rectangle<float>& viewport)
    {
        // Pre-condition: The viewport is valid.
        if (viewport.isValid())
        {
            // Set the viewport and calculate the world-to-pixel scale.
            m_impl->viewport        = viewport;
            m_impl->worldToPixel    = { m_impl->drawSpace.width() / viewport.width(), m_impl->drawSpace.height() / viewport.height() };
        }

        else
        {
            Systems::logger().logError ("RendererHAPI::setViewport(), attempt to set an invalid viewport. Request ignored.");
        }
    }


    void RendererHAPI::translateViewportTo (const Vector2<float>& translateTo)
    {
        // Simply calculate the translation and translate the viewport there.
        const auto transX = translateTo.x - m_impl->viewport.getLeft();
        const auto transY = translateTo.y - m_impl->viewport.getTop();

        m_impl->viewport.translate (transX, transY);
    }

    #pragma endregion


    #pragma region Data management

    TextureID RendererHAPI::loadTexture (const std::string& fileLocation, const int cropRight, const int cropBottom)
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
                // Ensure the cropping is only done once unless manually cropped to avoid over-cropping.
                texture.crop ({ cropRight, cropBottom });

                // Next scale the texture to the desired size.
                if (!texture.scaleByFactor (m_impl->textureScale, m_impl->filter))
                {
                    Systems::logger().logError ("RendererHAPI::loadTexture(), unable to scale loaded texture, possibly ran out of memory.");
                }

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
    

    TextureID RendererHAPI::createBlankTexture (const Vector2<float>& dimensions)
    {
        // We know that all valid string values will end in .* so we can just increment an integer value and guarantee we'll have a valid
        // hashed TextureID at the end of it!
        static auto blankID = 0U;

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

    void RendererHAPI::setFilteringMode (const FilterMode mode)
    {
        m_impl->filter = mode;
    }


    void RendererHAPI::drawToScreen (const Vector2<float>& point, const TextureID id, const BlendType blend)
    {
        drawToScreen (point, id, { 0, 0 }, blend);
    }


    void RendererHAPI::drawToScreen (const Vector2<float>& point, const TextureID id, const Point& frame, const BlendType blend)
    {
        // Check if the texture exists.
        auto& iterator = m_impl->textures.find (id);

        if (iterator != m_impl->textures.end())
        {
            // We need to make use of the viewport by translating the world position.
            const auto translation  = Vector2<float> (m_impl->viewport.getLeft(), m_impl->viewport.getTop());
            const auto drawPoint    = point + translation;

            const auto pixelSpace   = (Point) (point * m_impl->worldToPixel) + m_impl->screenOffset;

            // Blit the valid texture.
            if (!iterator->second.blitTo (m_impl->screen, m_impl->screenSpace, m_impl->drawSpace, pixelSpace, frame, blend))
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
        drawToTexture (point, source, target, { 0, 0 }, blend);
    }


    void RendererHAPI::drawToTexture (const Vector2<float>& point, const TextureID source, const TextureID target, const Point& frame, const BlendType blend)
    {
        // Obtain the iterators to both textures.
        auto& sourceIterator = m_impl->textures.find (source);
        auto& targetIterator = m_impl->textures.find (target);

        // Ensure that both textures exist.
        if (sourceIterator != m_impl->textures.end() && targetIterator != m_impl->textures.end())
        {
            // Convert the point into pixel space.
            const auto pixelSpace = (Point) (point * m_impl->worldToPixel);

            // Blit the source texture onto the target.
            if (!sourceIterator->second.blitTo (targetIterator->second, pixelSpace, frame, blend))
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