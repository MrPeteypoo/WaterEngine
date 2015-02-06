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
struct Renderer2DHAPIImpl final
{
    BYTE*                                   screen      { nullptr };    //!< A pointer to the memory address of the screen buffer.
    Rectangle<int>                          screenSpace { };            //!< A rectangle representing the screen space, used for clipping.
    std::unordered_map<TextureID, Texture>  textures    { };            //!< A container for all loaded texture data.
    std::hash<std::string>                  hasher      { };            //!< A hashing function used to speed up map lookup at the expense of map insertion.
};

#pragma endregion


#pragma region Constructors and destructor

Renderer2DHAPI::Renderer2DHAPI()
{
    // Ensure we have our implementation data.
    m_pImpl = new Renderer2DHAPIImpl();
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

void Renderer2DHAPI::initialise (const int screenWidth, const int screenHeight)
{
    // Pre-condition: Width and height are valid.
    if (screenWidth <= 0 || screenHeight <= 0)
    {
        throw std::invalid_argument ("Renderer2DHAPI::initialise(): Invalid screen resolution given (" + 
                                      std::to_string (screenWidth) + "x" + std::to_string (screenHeight) + ").");
    }

    // Initialise data.
    m_pImpl->screen = HAPI->GetScreenPointer();
    m_pImpl->screenSpace = { 0, 0, screenWidth - 1, screenHeight - 1 };

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


TextureID Renderer2DHAPI::loadTexture (const std::string& fileLocation, const Point& frameDimensions)
{
    try
    {
        // Attempt to load the texture.
        Texture texture (fileLocation, frameDimensions);

        // Determine the texture ID.
        const TextureID textureID = m_pImpl->hasher (fileLocation);

        // Add it to the unordered map.
        m_pImpl->textures.emplace (textureID, std::move (texture));

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
    
    return static_cast<TextureID> (0);
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


void Renderer2DHAPI::drawToScreen (const Point& point, const TextureID id, const BlendType blend)
{
    drawToScreen (point, id, blend, { 0, 0 });
}


void Renderer2DHAPI::drawToScreen (const Point& point, const TextureID id, const BlendType blend, const Point& frame)
{
    try
    {
        // If the texture doesn't exist an out-of-range error will be thrown.
        auto& texture = m_pImpl->textures.at (id);

        // Blit the valid texture.
        texture.blit (m_pImpl->screen, m_pImpl->screenSpace, point, blend, frame);
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


void Renderer2DHAPI::drawToTexture (const Point& point, const TextureID source, const TextureID target, const BlendType blend)
{
    drawToTexture (point, source, target, blend, { 0, 0 });
}


void Renderer2DHAPI::drawToTexture (const Point& point, const TextureID source, const TextureID target, const BlendType blend, const Point& frame)
{
    try
    {
        // If the texture doesn't exist an out-of-range error will be thrown.
        auto& sourceTexture = m_pImpl->textures.at (source);
        auto& targetTexture = m_pImpl->textures.at (target);

        // Blit the valid texture.
        targetTexture.blit (m_pImpl->screen, m_pImpl->screenSpace, point, blend, frame);
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