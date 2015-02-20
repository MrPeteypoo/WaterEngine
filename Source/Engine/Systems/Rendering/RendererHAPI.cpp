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
    #pragma region Constructors and destructor

    RendererHAPI::RendererHAPI (RendererHAPI&& move)
    {
        // Just use the operator implementation.
        *this = std::move (move);
    }


    RendererHAPI& RendererHAPI::operator= (RendererHAPI&& move)
    {
        if (this != &move)
        {
            // Take ownership of the movee implementation data.
            m_screen        = move.m_screen;
            m_screenSpace   = std::move (move.m_screenSpace);
            m_drawSpace     = std::move (move.m_drawSpace);
            m_screenOffset  = std::move (move.m_screenOffset);
            
            m_internalRes   = std::move (move.m_internalRes);
            m_textureScale  = std::move (move.m_textureScale);
            m_filter        = std::move (move.m_filter);

            m_viewport      = std::move (move.m_viewport);
            m_worldToPixel  = std::move (move.m_worldToPixel);
            
            m_hasher        = std::move (move.m_hasher);
            m_textures      = std::move (move.m_textures);

            // Reset primitives.
            move.m_screen   = nullptr;
        }

        return *this;
    }


    RendererHAPI::~RendererHAPI()
    {
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
            std::memset (m_screen, 0, m_screenSpace.area() * sizeOfColour);
            return true;
        }

        return false;
    }

    #pragma endregion


    #pragma region Internal workings

    void RendererHAPI::initialiseHAPI (const int width, const int height)
    {
        // Prepare to initialise HAPI.
        int finalWidth  = width, 
            finalHeight = height;

        if (!HAPI->Initialise (&finalWidth, &finalHeight))
        {
            throw std::runtime_error ("RendererHAPI::initialise(), unable to initialise HAPI.");
        }

        // Set the correct screen values.
        m_screen        = HAPI->GetScreenPointer();
        m_screenSpace   = { 0, 0, finalWidth - 1, finalHeight - 1 };
        m_screenOffset  = { 0, 0 };

        // Ensure the screen pointer is valid.
        if (!m_screen)
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
        m_filter       = filter;
        m_viewport     = { 0, 0, 1, 1 };

        // Prepare the internal resolution, required for world to pixel scaling.
        m_internalRes = { internalWidth, internalHeight };

        // Ensure we have correct offset values.
        calculateScalingOffset();

        // We need to calculate the drawable area of the screen.
        const auto  screenWidth  = m_screenSpace.width(),
                    screenHeight = m_screenSpace.height();

        m_drawSpace.setLeft (m_screenOffset.x);
        m_drawSpace.setTop (m_screenOffset.y);
        m_drawSpace.setRight (screenWidth - m_screenOffset.x - 1);
        m_drawSpace.setBottom (screenHeight - m_screenOffset.y - 1);


        // We need to calculate the ratio between the internal resolution and the external resolution.
        const auto  drawWidth  = (float) m_drawSpace.width(),
                    drawHeight = (float) m_drawSpace.height();
        
        if (screenWidth == internalWidth && screenHeight == internalHeight)
        {
            m_textureScale = { 1.f, 1.f };
        }

        else
        {
            m_textureScale.x = drawWidth / internalWidth;
            m_textureScale.y = drawHeight / internalHeight;
        }

        // We need to enable the correct world to unit scale.
        m_worldToPixel = { drawWidth, drawHeight };
    }


    void RendererHAPI::calculateScalingOffset()
    {
        // We should just centre the screen if no scaling is to be applied.
        if (m_filter == FilterMode::None)
        {
            // Simply set the offset to be the difference between the internal and external resolutions.
            m_screenOffset.x = (m_screenSpace.width() - m_internalRes.x) / 2;
            m_screenOffset.y = (m_screenSpace.height() - m_internalRes.y) / 2;
        }
        
        else
        {
            // We need to obtain the aspect ratio of the screen and the internal framebuffer and compare the two.
            const auto  screenWidth     = m_screenSpace.width(),
                        screenHeight    = m_screenSpace.height();

            const auto  screenRatio     = (float) screenWidth / screenHeight,
                        internalRatio   = (float) m_internalRes.x / m_internalRes.y;

            // If the internal ratio is wider we need to correct the Y offset.
            if (internalRatio > screenRatio)
            {
                // We need to divide by the aspect ratio to determine the drawable height of the screen!
                const auto desiredHeight = screenWidth / internalRatio;

                // We use half of the difference to have padding on the top and bottom of the screen.
                m_screenOffset.y = (int) ((screenHeight - desiredHeight) / 2.f);
            }

            // If the internal ratio is narrower we need to correct the X offset.
            if (internalRatio < screenRatio)
            {
                // We need to multiply the current height by the internal ratio to calculate the desired width.
                const auto desiredWidth = screenHeight * internalRatio;

                // We use half of the difference to add padding on the left and right of the screen.
                m_screenOffset.x = (int) ((screenWidth - desiredWidth) / 2.f);
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
            m_viewport        = viewport;
            m_worldToPixel    = { m_drawSpace.width() / viewport.width(), m_drawSpace.height() / viewport.height() };
        }

        else
        {
            Systems::logger().logError ("RendererHAPI::setViewport(), attempt to set an invalid viewport. Request ignored.");
        }
    }


    void RendererHAPI::translateViewportTo (const Vector2<float>& translateTo)
    {
        // Simply calculate the translation and translate the viewport there.
        const auto transX = translateTo.x - m_viewport.getLeft();
        const auto transY = translateTo.y - m_viewport.getTop();

        m_viewport.translate (transX, transY);
    }

    #pragma endregion


    #pragma region Data management

    TextureID RendererHAPI::loadTexture (const std::string& fileLocation, const int cropRight, const int cropBottom)
    {
        // Determine the texture ID.
        const auto textureID = m_hasher (fileLocation);

        // Check if it already exists.
        if (m_textures.count (textureID) == 0)
        {
            // Attempt to load the texture.
            HAPITexture texture { };

            if (texture.initialise (fileLocation))
            {
                // Ensure the cropping is only done once unless manually cropped to avoid over-cropping.
                texture.crop ({ cropRight, cropBottom });

                // Next scale the texture to the desired size.
                if (!texture.scaleByFactor (m_textureScale, m_filter))
                {
                    Systems::logger().logError ("RendererHAPI::loadTexture(), unable to scale loaded texture, possibly ran out of memory.");
                }

                // Add it to the unordered map.
                m_textures.emplace (textureID, std::move (texture));
            
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
        const auto textureID = m_hasher (std::to_string (blankID++));
    
        // Attempt to creae the blank texture.
        HAPITexture texture { }; 

        if (texture.initialise (dimensions))
        {
            // Finally load the texture and return the ID.
            m_textures.emplace (textureID, std::move (texture));

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
            auto& iterator = m_textures.find (target);

            if (iterator != m_textures.end())
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
        auto& iterator = m_textures.find (target);

        if (iterator != m_textures.end())
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
        if (m_textures.erase (texture) == 0)
        {
            Systems::logger().logWarning ("RendererHAPI::removeTexture(), attempt to erase non-existent texture.");
        }
    }


    void RendererHAPI::clearTextureData()
    {
        m_textures.clear();
    }


    #pragma endregion



    #pragma region Rendering

    void RendererHAPI::setFilteringMode (const FilterMode mode)
    {
        m_filter = mode;
    }


    void RendererHAPI::drawToScreen (const Vector2<float>& point, const TextureID id, const BlendType blend)
    {
        drawToScreen (point, id, { 0, 0 }, blend);
    }


    void RendererHAPI::drawToScreen (const Vector2<float>& point, const TextureID id, const Point& frame, const BlendType blend)
    {
        // Check if the texture exists.
        auto& iterator = m_textures.find (id);

        if (iterator != m_textures.end())
        {
            // We need to make use of the viewport by translating the world position.
            const auto translation  = Vector2<float> (m_viewport.getLeft(), m_viewport.getTop());
            const auto drawPoint    = point + translation;

            const auto pixelSpace   = (Point) (point * m_worldToPixel) + m_screenOffset;

            // Blit the valid texture.
            if (!iterator->second.blitTo (m_screen, m_screenSpace, m_drawSpace, pixelSpace, frame, blend))
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
        auto& sourceIterator = m_textures.find (source);
        auto& targetIterator = m_textures.find (target);

        // Ensure that both textures exist.
        if (sourceIterator != m_textures.end() && targetIterator != m_textures.end())
        {
            // Convert the point into pixel space.
            const auto pixelSpace = (Point) (point * m_worldToPixel);

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