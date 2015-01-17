#include <Rendering/Renderer2DHAPI.h>


#pragma region Implmentation data


// STL headers.
#include <exception>
#include <iostream>
#include <unordered_map>


// Engine headers.
#include <HAPI_lib.h>
#include <Maths/Vector2D.h>
#include <Rendering/Texture.h>


// Forward declarations
using Colour = HAPI_TColour;

const unsigned int  channel      = 255;
const auto          sizeOfColour = sizeof (Colour);



/// <summary> A POD structure with all working data for the HAPI renderer. </summary>
struct Renderer2DHAPIImpl final
{
    BYTE*                                   screen      { nullptr };    //!< A pointer to the memory address of the screen buffer.
    Rectangle                               screenSpace { };            //!< A rectangle representing the screen space, used for clipping.
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

        // Ensure we don't break the movee.
        move.m_pImpl = new Renderer2DHAPIImpl();
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


TextureID Renderer2DHAPI::loadTexture (const std::string& fileLocation, const Vector2D<unsigned int>& frameDimensions)
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
    const auto black = static_cast<BYTE> (channel * blackLevel);

    // Use memset for efficiency.
    std::memset (m_pImpl->screen, black, m_pImpl->screenSpace.area() * sizeOfColour);
}


void Renderer2DHAPI::clearToColour (const float red, const float green, const float blue, const float alpha)
{
    const auto      screenSize  =   m_pImpl->screenSpace.area();
    const Colour    colour      {   static_cast<BYTE> (channel * red),
                                    static_cast<BYTE> (channel * green), 
                                    static_cast<BYTE> (channel * blue), 
                                    static_cast<BYTE> (channel * alpha) };

    // Loop through each pixel
    for (unsigned int i = 0; i < screenSize; ++i)
    {
        // Find the correct memory address
        auto pixel = m_pImpl->screen + i * sizeOfColour;

        std::memcpy (pixel, &colour, sizeOfColour);
    }
}


void Renderer2DHAPI::drawTexture (const Vector2D<int>& point, const TextureID id, const BlendType blend)
{
    drawTexture (point, id, blend, { 0, 0 });
}


void Renderer2DHAPI::drawTexture (const Vector2D<int>& point, const TextureID id, const BlendType blend, const Vector2D<unsigned int>& frame)
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
        std::cerr << "Exception caught in Renderer2DHAPI::drawTexture(): " << error.what() << std::endl;
    }

    catch (...) 
    {
        std::cerr << "Unknown error caught in Renderer2DHAPI::drawTexture()." << std::endl;    
    }
}


#pragma endregion Rendering