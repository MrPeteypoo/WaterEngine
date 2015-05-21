#include "RendererSFML.hpp"


// STL headers.
#include <cassert>
#include <stdexcept>


// Engine headers.
#include <Systems/Window/WindowSFML.hpp>



// Engine namespace.
namespace water
{
    /////////////////
    // Constructor //
    /////////////////

    RendererSFML::RendererSFML (IEngineWindow* const window)
    {
        // Ensure we have a valid pointer.
        assert (window);

        // Pre-condition: The window system must be SFML-based.
        m_window = dynamic_cast<WindowSFML*> (window);

        if (!m_window)
        {
            throw std::runtime_error ("RendererSFML::RendererSFML(), SFML rendering cannot be enabled without a SFML window system.");
        }
    }


    ///////////////////////
    // System management //
    ///////////////////////

    void RendererSFML::initialise (const unsigned int internalWidth, const unsigned int internalHeight, const bool smooth)
    {
        // Pre-condition: Internal resolution is valid.
        if (internalWidth == 0 || internalHeight == 0)
        {
            throw std::invalid_argument ("RendererSFML::initialise(), invalid internal resolution given (" +
                                         std::to_string (internalWidth) + "x" + std::to_string (internalHeight) + ").");
        }

        m_internalWidth  = internalWidth;
        m_internalHeight = internalHeight;
        m_smooth         = smooth;
    }


    /////////////////////
    // Data management //
    /////////////////////

    TextureID RendererSFML::loadTexture (const std::string& fileLocation, const int cropRight, const int cropBottom)
    {
        return 0;
    }


    TextureID RendererSFML::createBlankTexture (const Vector2<float>& dimensions)
    {
        return 0;
    }


    void RendererSFML::setFrameDimensions (const TextureID target, const Point& dimensions)
    {

    }


    void RendererSFML::removeTexture (const TextureID texture)
    {

    }


    void RendererSFML::clearTextureData()
    {

    }

    //////////////
    // Viewport //
    //////////////

    void RendererSFML::setViewport (const Rectangle<float>& viewport)
    {

    }


    void RendererSFML::translateViewportTo (const Vector2<float>& translateTo)
    {

    }

    ///////////////
    // Rendering //
    ///////////////

    void RendererSFML::drawToScreen (const Vector2<float>& point, const TextureID id, const BlendType blend)
    {

    }


    void RendererSFML::drawToScreen (const Vector2<float>& point, const TextureID id, const Point& frame, const BlendType blend)
    {

    }


    void RendererSFML::drawToTexture (const Vector2<float>& point, const TextureID source, const TextureID target, const BlendType blend)
    {

    }


    void RendererSFML::drawToTexture (const Vector2<float>& point, const TextureID source, const TextureID target, const Point& frame, const BlendType blend)
    {

    }
}
