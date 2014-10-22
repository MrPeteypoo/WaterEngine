#if !defined TEXTURE_INCLUDED
#define TEXTURE_INCLUDED


// STL headers
#include <memory>
#include <string>


// Engine headers
#include <HAPI_lib.h>


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

        /// <summary> Obtains each colour channel for the specified pixel. The first pixel is 0. </summary>
        const HAPI_TColour getPixel (const unsigned int pixel) const;

        /// <summary> Obtains each colour channel for the specified pixel. The first pixel is (0, 0). </summary>
        const HAPI_TColour getPixel (const unsigned int x, const unsigned int y) const;
        
        const int getResolution() const { return m_resolution; }
        const int getWidth() const      { return m_width; }
        const int getHeight() const     { return m_height; }

        #pragma endregion


        #pragma region Loading functionality
        
        /// <summary> 
        /// Attempts to load a texture using the file location specified.
        /// <returns> Returns whether it was possible or not. </returns>
        /// </summary>
        const bool loadTexture (const std::string& fileLocation);
    
        #pragma endregion

    private:

        #pragma region Member variables
        
        int                     m_resolution    { 0 };          //!< The total number of pixels in the texture.
        int                     m_width         { 0 };          //!< The width of the loaded texture.
        int                     m_height        { 0 };          //!< The height of the loaded texture.

        std::unique_ptr<BYTE[]> m_data          {  };    //!< The raw memory data of the texture.

        #pragma endregion
};

#endif // TEXTURE_INCLUDED