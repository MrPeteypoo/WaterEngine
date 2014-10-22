#include "Texture.h"


const size_t sizeOfColour { sizeof (HAPI_TColour) };    //!< The size in bytes of the HAPI_TColour.


#pragma region Constructors and destructor

Texture::Texture (const std::string& fileLocation)
{   
    // Attempt to load the texture during construction.
    loadTexture (fileLocation);    
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


const HAPI_TColour Texture::getPixel (const unsigned int pixel) const
{
    // Pre-condition: Ensure there is data available.
    if (!m_data)
    {
        throw std::exception ("Attempt to obtain a pixel from an empty texture.");   
    }

    else
    {
        // The order of channels in memory is BGRA
        const BYTE  blue    = m_data[pixel * sizeOfColour],
                    green   = m_data[pixel * sizeOfColour + 1],
                    red     = m_data[pixel * sizeOfColour + 2],
                    alpha   = m_data[pixel * sizeOfColour + 3];
                    

        return { red, green, blue, alpha };
    }
}


const HAPI_TColour Texture::getPixel (const unsigned int x, const unsigned int y) const
{
    return getPixel (x + y * m_width);
}


#pragma endregion Getters and setters


#pragma region Loading functionality


const bool Texture::loadTexture (const std::string& fileLocation)
{
    // Create the data pointer.
    BYTE* data = nullptr;

    // Attempt to load the data.
    if (HAPI->LoadTexture (fileLocation, &data, &m_width, &m_height))
    {
        // Calculate the correct resolution
        m_resolution = m_width * m_height;

        // Create the data unique_ptr.
        m_data = std::unique_ptr<BYTE[]> (data);

        return true;
    }

    
}


#pragma endregion Loading functionality