#include <Rendering/RendererHAPI.h>


// STL headers.
#include <exception>
#include <iostream>
#include <unordered_map>


// Engine headers.
#include <Rendering/Texture.h>
#include <Utility/Maths.h>


// Third party headers.
#include <HAPI/HAPI_lib.h>


// Forward declarations.
using       Colour       = HAPI_TColour;
const auto  channel      = 255U;
const auto  sizeOfColour = sizeof (Colour);



#pragma region Implmentation data

/// <summary> A POD structure with all working data for the RendererHAPI class. </summary>
struct RendererHAPI::Impl final
{
    BYTE*                                   screen      { nullptr };    //!< A pointer to the memory address of the screen buffer.
    Rectangle<int>                          screenSpace { };            //!< A rectangle representing the screen space, used for clipping.
    Vector2<float>                         unitToPixel { };            //!< The scalar applied to world units to create a pixel-space vector.
    std::hash<std::string>                  hasher      { };            //!< A hashing function used to speed up map lookup at the expense of map insertion.
    std::unordered_map<TextureID, Texture>  textures    { };            //!< A container for all loaded texture data.
};

#pragma endregion


#pragma region Constructors and destructor

RendererHAPI::RendererHAPI()
{
    // Ensure we have our implementation data.
    m_pImpl = new Impl();
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
        if (m_pImpl)
        {
            delete m_pImpl;
        }

        // Take ownership of the movee implementation data.
        m_pImpl = move.m_pImpl;
        move.m_pImpl = nullptr;
    }

    return *this;
}


RendererHAPI::~RendererHAPI()
{
    // Clean up after ourselves.
    if (m_pImpl)
    {
        delete m_pImpl;
        m_pImpl = nullptr;
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
    m_pImpl->screen = HAPI->GetScreenPointer();
    m_pImpl->screenSpace = { 0, 0, screenWidth - 1, screenHeight - 1 };
    m_pImpl->unitToPixel = unitToPixelScale;

    // Ensure the screen pointer is valid.
    if (!m_pImpl->screen)
    {
        throw std::runtime_error ("RendererHAPI::initialise(): Failed to obtain a pointer to the screenbuffer.");
    }
}


void RendererHAPI::clearTextureData()
{
    m_pImpl->textures.clear();
}


TextureID RendererHAPI::createBlankTexture (const Vector2<float>& textureDimensions, const Point& frameDimensions, const bool pixelDimensions)
{
    // We know that all valid string values will end in .* so we can just increment an integer value and guarantee we'll have a valid
    // hashed TextureID at the end of it!
    static auto blankID         = 0U;

    // Determine the correct dimensions.
    const auto dimensions       = pixelDimensions ? (Point) textureDimensions : (Point) (textureDimensions * m_pImpl->unitToPixel);

    // Determine the texture ID.
    const TextureID textureID   = m_pImpl->hasher (std::to_string (blankID++));
    
    // Check if we should enable sprite sheet funcitonality.
    Texture texture             = frameDimensions.x > 0 && frameDimensions.y > 0 ?
                                    Texture (dimensions, frameDimensions) :
                                    Texture (dimensions);

    // Finally load the texture and return the ID.
    m_pImpl->textures.emplace (textureID, std::move (texture));

    return textureID;
}


TextureID RendererHAPI::loadTexture (const std::string& fileLocation, const Point& frameDimensions)
{
    try
    {
        // Determine the texture ID.
        const TextureID textureID = m_pImpl->hasher (fileLocation);

        // Check if it exists first.
        if (m_pImpl->textures.find (textureID) == m_pImpl->textures.end())
        {
            // Attempt to load the texture.
            Texture texture (fileLocation, frameDimensions);

            // Add it to the unordered map.
            m_pImpl->textures.emplace (textureID, std::move (texture));
        }

        return textureID;
    }

    catch (const std::exception& error)
    {
        std::cerr << "Exception caught in RendererHAPI::loadTexture(): " << error.what() << std::endl;
    }

    catch (...)
    {
        std::cerr << "Unknown error caught in RendererHAPI::loadTexture()." << std::endl;
    }
    
    return (TextureID) 0;
}


void RendererHAPI::scaleTexture (const TextureID target, const Vector2<float>& dimensions, const bool pixelUnits)
{
    try
    {
        // Ensure the texture actually exists.
        auto& texture       = m_pImpl->textures.at (target);

        // Scale the dimensions if necessary.
        const Point size    = pixelUnits ? dimensions : dimensions * m_pImpl->unitToPixel;

        // Inform the texture to change its dimensions.
        texture.scaleToSize (size);
    }

    catch (const std::exception& error)
    {
        std::cerr << "Exception caught in RendererHAPI::scaleTexture(): " << error.what() << std::endl;
    }

    catch (...)
    {
        std::cerr << "Unknown error caught in RendererHAPI::scaleTexture()." << std::endl;
    }
}


#pragma endregion



#pragma region Rendering


void RendererHAPI::clearToBlack (const float blackLevel)
{
    const auto black = (BYTE) (channel * util::clamp (blackLevel, 0.f, 1.f));

    // Use memset for efficiency.
    std::memset (m_pImpl->screen, black, m_pImpl->screenSpace.area() * sizeOfColour);
}


void RendererHAPI::clearToColour (const float red, const float green, const float blue, const float alpha)
{
    const auto      screenSize  =   m_pImpl->screenSpace.area();
    const Colour    colour      {   (BYTE) (channel * red),
                                    (BYTE) (channel * green), 
                                    (BYTE) (channel * blue), 
                                    (BYTE) (channel * alpha) };

    // Loop through each pixel
    for (auto i = 0; i < screenSize; ++i)
    {
        // Find the correct memory address
        auto pixel = m_pImpl->screen + i * sizeOfColour;

        std::memcpy (pixel, &colour, sizeOfColour);
    }
}


void RendererHAPI::drawToScreen (const Vector2<float>& point, const TextureID id, const BlendType blend)
{
    drawToScreen (point, id, blend, { 0, 0 });
}


void RendererHAPI::drawToScreen (const Vector2<float>& point, const TextureID id, const BlendType blend, const Point& frame)
{
    try
    {
        // If the texture doesn't exist an out-of-range error will be thrown.
        auto& texture           = m_pImpl->textures.at (id);

        const auto pixelSpace   = (Point) (point * m_pImpl->unitToPixel);

        // Blit the valid texture.
        texture.blit (m_pImpl->screen, m_pImpl->screenSpace, pixelSpace, blend, frame);
    }

    catch (const std::exception& error)
    {
        std::cerr << "Exception caught in RendererHAPI::drawToScreen(): " << error.what() << std::endl;
    }

    catch (...) 
    {
        std::cerr << "Unknown error caught in RendererHAPI::drawToScreen()." << std::endl;    
    }
}


void RendererHAPI::drawToTexture (const Vector2<float>& point, const TextureID source, const TextureID target, const BlendType blend)
{
    drawToTexture (point, source, target, blend, { 0, 0 });
}


void RendererHAPI::drawToTexture (const Vector2<float>& point, const TextureID source, const TextureID target, const BlendType blend, const Point& frame)
{
    try
    {
        // If the texture doesn't exist an out-of-range error will be thrown.
        auto& sourceTexture     = m_pImpl->textures.at (source);
        auto& targetTexture     = m_pImpl->textures.at (target);

        const auto pixelSpace   = (Point) (point * m_pImpl->unitToPixel);

        // Blit the valid texture.
        sourceTexture.blit (targetTexture, pixelSpace, blend, frame);
    }

    catch (const std::exception& error)
    {
        std::cerr << "Exception caught in RendererHAPI::drawToTexture(): " << error.what() << std::endl;
    }

    catch (...) 
    {
        std::cerr << "Unknown error caught in RendererHAPI::drawToTexture()." << std::endl;
    }
}


#pragma endregion Rendering