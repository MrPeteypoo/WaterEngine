#include <Rendering/Renderer2DHAPI.h>


// STL headers.
#include <exception>
#include <iostream>
#include <unordered_map>


// Engine headers.
#include <HAPI_lib.h>
#include <Maths/Vector2D.h>
#include <Rendering/Texture.h>
#include <Utility/Maths.h>


// Forward declarations
using       Colour       = HAPI_TColour;
const auto  channel      = 255U;
const auto  sizeOfColour = sizeof (Colour);



#pragma region Implmentation data

/// <summary> A POD structure with all working data for the HAPI renderer. </summary>
struct Renderer2DHAPI::Impl final
{
    BYTE*                                   screen      { nullptr };    //!< A pointer to the memory address of the screen buffer.
    Rectangle<int>                          screenSpace { };            //!< A rectangle representing the screen space, used for clipping.
    Vector2D<float>                         unitToPixel { };            //!< The scalar applied to world units to create a pixel-space vector.
    std::hash<std::string>                  hasher      { };            //!< A hashing function used to speed up map lookup at the expense of map insertion.
    std::unordered_map<TextureID, Texture>  textures    { };            //!< A container for all loaded texture data.
};

#pragma endregion


#pragma region Constructors and destructor

Renderer2DHAPI::Renderer2DHAPI()
{
    // Ensure we have our implementation data.
    m_pImpl = new Impl();
}


Renderer2DHAPI::Renderer2DHAPI (Renderer2DHAPI&& move)
{
    // Just use the operator implementation.
    *this = std::move (move);
}


Renderer2DHAPI& Renderer2DHAPI::operator= (Renderer2DHAPI&& move)
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


Renderer2DHAPI::~Renderer2DHAPI()
{
    // Clean up after ourselves.
    if (m_pImpl)
    {
        delete m_pImpl;
        m_pImpl = nullptr;
    }
}

#pragma endregion Constructors and destructor


#pragma region Initialisation

void Renderer2DHAPI::initialise (const int screenWidth, const int screenHeight, const Vector2D<float>& unitToPixelScale)
{
    // Pre-condition: Width and height are valid.
    if (screenWidth <= 0 || screenHeight <= 0)
    {
        throw std::invalid_argument ("Renderer2DHAPI::initialise(): Invalid screen resolution given (" + 
                                      std::to_string (screenWidth) + "x" + std::to_string (screenHeight) + ").");
    }

    // Pre-condition: Pixel scale is valid.
    if (unitToPixelScale.x <= 0 || unitToPixelScale.y <= 0)
    {
        throw std::invalid_argument ("Renderer2DHAPI::initialise(): Invalid scale values given (" +
                                      std::to_string (unitToPixelScale.x) + "x" + std::to_string (unitToPixelScale.y) + ").");
   }

    // Initialise data.
    m_pImpl->screen = HAPI->GetScreenPointer();
    m_pImpl->screenSpace = { 0, 0, screenWidth - 1, screenHeight - 1 };
    m_pImpl->unitToPixel = unitToPixelScale;

    // Ensure the screen pointer is valid.
    if (!m_pImpl->screen)
    {
        throw std::runtime_error ("Renderer2DHAPI::initialise(): Failed to obtain a pointer to the screenbuffer.");
    }
}


void Renderer2DHAPI::clearTextureData()
{
    m_pImpl->textures.clear();
}


TextureID Renderer2DHAPI::createBlankTexture (const Vector2D<float>& textureDimensions, const Point& frameDimensions, const bool pixelDimensions)
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


TextureID Renderer2DHAPI::loadTexture (const std::string& fileLocation, const Point& frameDimensions)
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

    catch (std::exception& error)
    {
        std::cerr << "Exception caught in Renderer2DHAPI::loadTexture(): " << error.what() << std::endl;
    }

    catch (...)
    {
        std::cerr << "Unknown error caught in Renderer2DHAPI::loadTexture()." << std::endl;
    }
    
    return (TextureID) 0;
}


#pragma endregion Rendering



#pragma region Rendering


void Renderer2DHAPI::clearToBlack (const float blackLevel)
{
    const auto black = (BYTE) (channel * util::clamp (blackLevel, 0.f, 1.f));

    // Use memset for efficiency.
    std::memset (m_pImpl->screen, black, m_pImpl->screenSpace.area() * sizeOfColour);
}


void Renderer2DHAPI::clearToColour (const float red, const float green, const float blue, const float alpha)
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


void Renderer2DHAPI::drawToScreen (const Vector2D<float>& point, const TextureID id, const BlendType blend)
{
    drawToScreen (point, id, blend, { 0, 0 });
}


void Renderer2DHAPI::drawToScreen (const Vector2D<float>& point, const TextureID id, const BlendType blend, const Point& frame)
{
    try
    {
        // If the texture doesn't exist an out-of-range error will be thrown.
        auto& texture           = m_pImpl->textures.at (id);

        const auto pixelSpace   = (Point) (point * m_pImpl->unitToPixel);

        // Blit the valid texture.
        texture.blit (m_pImpl->screen, m_pImpl->screenSpace, pixelSpace, blend, frame);
    }

    catch (std::exception& error)
    {
        std::cerr << "Exception caught in Renderer2DHAPI::drawToScreen(): " << error.what() << std::endl;
    }

    catch (...) 
    {
        std::cerr << "Unknown error caught in Renderer2DHAPI::drawToScreen()." << std::endl;    
    }
}


void Renderer2DHAPI::drawToTexture (const Vector2D<float>& point, const TextureID source, const TextureID target, const BlendType blend)
{
    drawToTexture (point, source, target, blend, { 0, 0 });
}


void Renderer2DHAPI::drawToTexture (const Vector2D<float>& point, const TextureID source, const TextureID target, const BlendType blend, const Point& frame)
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

    catch (std::exception& error)
    {
        std::cerr << "Exception caught in Renderer2DHAPI::drawToTexture(): " << error.what() << std::endl;
    }

    catch (...) 
    {
        std::cerr << "Unknown error caught in Renderer2DHAPI::drawToTexture()." << std::endl;
    }
}


#pragma endregion Rendering