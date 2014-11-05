#if !defined TEXTURE_INCLUDED
#define TEXTURE_INCLUDED


// STL headers.
#include <memory>
#include <string>


// Forward declarations.
typedef unsigned char BYTE;

using Colour = struct HAPI_TColour;


/// <summary> A texture is a self-managed wrapper for any texture to be used in the HAPI engine. </summary>
class Texture final
{
    public:
        
        #pragma region Constructors and destructor
        
        /// <summary> Create a texture and load the data at the same time. If the texture loading fails an exception will be thrown.</summary>
        Texture (const std::string& fileLocation);

        Texture (Texture&& move);
        Texture& operator= (Texture&& move);

        Texture()   = default;
        ~Texture()  = default;

        
        // Explicitly remove copy functionality.
        Texture (const Texture& copy)               = delete;
        Texture& operator= (const Texture& copy)    = delete;
        
        #pragma endregion


        #pragma region Getters and setters
        
        int getResolution() const           { return m_resolution; }
        int getWidth() const                { return m_width; }
        int getHeight() const               { return m_height; }


        /// <summary> Returns the raw data, useful for accessing line-by-line. </summary>
        const BYTE* const getData() const   { return m_data.get(); }

        /// <summary> Obtains each colour channel for the specified pixel. The first pixel is 0. </summary>
        Colour getPixel (const int pixelNumber) const;

        /// <summary> Obtains each colour channel for the specified pixel. The first pixel is (0, 0). </summary>
        Colour getPixel (const int x, const int y) const;

        #pragma endregion


        #pragma region Loading functionality

        /// <summary> Checks to see if the texture has loaded any data from an image file. </summary>
        bool hasLoaded() const              { return m_data != nullptr; }
        
        /// <summary> 
        /// Attempts to load a texture using the file location specified.
        /// <returns> Returns whether it was possible or not. </returns>
        /// </summary>
        bool loadTexture (const std::string& fileLocation);
    
        #pragma endregion

    private:

        #pragma region Member variables
        
        int                     m_resolution    { 0 };  //!< The total number of pixels in the texture.
        int                     m_width         { 0 };  //!< The width of the loaded texture.
        int                     m_height        { 0 };  //!< The height of the loaded texture.

        std::unique_ptr<BYTE[]> m_data          {  };   //!< The raw memory data of the texture.

        #pragma endregion
};

#endif // TEXTURE_INCLUDED