#include "Texture.h"


// Engine headers.
#include <HAPI_lib.h>


const size_t sizeOfColour { sizeof (HAPI_TColour) };    //!< The size in bytes of the HAPI_TColour.


#pragma region Constructors and destructor

Texture::Texture (const std::string& fileLocation)
{   
    // Attempt to load the texture during construction.
    if (!loadTexture (fileLocation))
    {
        throw std::runtime_error ("Unable to initialise Texture with file: " + fileLocation);
    }
}


Texture::Texture (Texture&& move)
{
    // Just use the operator implementation.
    *this = std::move (move);
}


Texture& Texture::operator= (Texture&& move)
{
    if (this != &move)
    {
        // Obtain all of the data from the movee.
        m_resolution = move.m_resolution;
        m_width = move.m_width;
        m_height = move.m_height;

        m_data = std::move (move.m_data);


        // Reset the movee.
        move.m_resolution = 0;
        move.m_width = 0;
        move.m_height = 0;
        
        move.m_data = nullptr;
    }

    return *this;
}


#pragma endregion Constructors and destructor


#pragma region Getters and setters


HAPI_TColour Texture::getPixel(const int pixelNumber) const
{
    try
    {
        const int pixel = pixelNumber * sizeOfColour;

        // The order of channels in memory is BGRA, but the constructor takes RGBA.
        return
        {
            m_data[pixel + 2],
            m_data[pixel + 1],
            m_data[pixel],
            m_data[pixel + 3]
        };
    }

    catch (const std::exception& error)
    {
        HAPI->DebugText ("Texture::getPixel(): " + (std::string) error.what());
    }

    catch (...)
    {
        HAPI->DebugText ("Texture::getPixel(): Unknown error occurred.");
    }
    
    // Just return a black colour.
    return { };
}


HAPI_TColour Texture::getPixel(const int x, const int y) const
{
    // Re-implement for efficiency.
    try
    {
        const int pixel = (x + y * m_width) * sizeOfColour;

        // The order of channels in memory is BGRA, but the constructor takes RGBA.
        return
        {
            m_data[pixel + 2],
            m_data[pixel + 1],
            m_data[pixel],
            m_data[pixel + 3]
        };
    }

    catch (const std::exception& error)
    {
        HAPI->DebugText ("Texture::getPixel(): " + (std::string) error.what());
    }

    catch (...)
    {
        HAPI->DebugText ("Texture::getPixel(): Unknown error occurred.");
    }

    return { };
}


#pragma endregion Getters and setters


#pragma region Loading functionality


bool Texture::loadTexture (const std::string& fileLocation)
{
    // Create the data pointer.
    BYTE* data  = nullptr;

    // Attempt to load the data. If the loading succeeds then the width and height will be set.
    if (HAPI->LoadTexture (fileLocation, &data, &m_width, &m_height))
    {
        // Calculate the correct resolution.
        m_resolution = m_width * m_height;

        // Create the data unique_ptr.
        m_data = std::unique_ptr<BYTE[]> (data);

        return true;
    }

    return false;
}


#pragma endregion Loading functionality