#if !defined TEXTURE_INCLUDED
#define TEXTURE_INCLUDED


// STL headers.
#include <string>


// Engine headers.
#include <Maths/Rectangle.h>
#include <Maths/Vector2D.h>
#include <Rendering/IRenderer2D.h>


// Forward declarations.
typedef unsigned char BYTE;



/// <summary> A texture is a self-managed wrapper for any texture to be used in the HAPI engine. </summary>
class Texture final
{
    public:
        
        #pragma region Constructors and destructor
        
        /// <summary> Create a texture by loading image data. Assumes there are no individual frames. </summary>
        Texture (const std::string& fileLocation);

        /// <summary> Create a texture by loading image data, the frame dimensions specified cause the texture to act like a spritesheet. </summary>
        Texture (const std::string& fileLocation, const Point& frameDimensions);
        
        Texture()                                   = default;
        Texture (Texture&& move);
        Texture& operator= (Texture&& move);
        ~Texture();

        
        // Explicitly remove copy functionality.
        Texture (const Texture& copy)               = delete;
        Texture& operator= (const Texture& copy)    = delete;
        
        #pragma endregion


        #pragma region Getters and setters

        /// <summary> Returns the raw data, useful for accessing line-by-line. </summary>
        const BYTE* const getData() const   { return m_pData; }

        /// <summary> Returns how many frames exist, 0 means the texture is not a spritesheet. </summary>
        int getFrameCount() const           { return m_frames; }

        /// <summary> Sets the frame dimensions to the figures specified, (0, 0) disables spritesheet functionality and anything else enables it. </summary>
        Texture& setFrameDimensions (const Point& dimensions);

        /// <summary> Resets frame information, effectively disabling spritesheet functionality. </summary>
        Texture& resetFrameDimensions();

        #pragma endregion


        #pragma region Loading functionality

        /// <summary> Checks to see if the texture has loaded any data from an image file. </summary>
        bool hasLoaded() const              { return m_pData != nullptr; }
        
        /// <summary> 
        /// Attempts to load a texture using the file location specified.
        /// <returns> Returns whether it was possible or not. </returns>
        /// </summary>
        void loadTexture (const std::string& fileLocation);
    
        #pragma endregion


        #pragma region Rendering

        /// <summary> The entry point for blitting functionality. Determines whether blitting is necessary then calls the correct blitting function. </summary>
        /// <param name="target"> The data buffer to write to, this is likely to be the screen. </param>
        /// <param name="targetSpace"> The Rectangle representing the drawable area of the target. </param>
        /// <param name="point"> Where the blitting should begin, if this is out-of-bounds then the texture will be clipped. </param>
        /// <param name="blend"> Determines how the texture should be blended, can have a huge impact on speed. </param>
        /// <param name="frame"> The co-ordinate of the frame to be drawn, (0, 0) should be used for single images. </param>
        void blit (BYTE* const target, const Rectangle<int>& targetSpace, const Point& point, const BlendType blend, const Point& frame);

        /// <summary> An overload which blits the current texture onto the target texture. </summary>
        /// <param name="target"> The target texture to be altered. </param>
        void blit (Texture& target, const Point& point, const BlendType blend, const Point& frame);
        
    private:
        
        /// <summary> Will blit the texture line-by-line without performing any alpha blending. </summary>
        void blitOpaque (BYTE* const target, const Rectangle<int>& targetSpace, const Point& point, const Point& frameOffset, const Rectangle<int>& drawArea);

        /// <summary> Will blit the texture pixel-by-pixel taking into account alpha values, slower. </summary>
        void blitTransparent (BYTE* const target, const Rectangle<int>& targetSpace, const Point& point, const Point& frameOffset, const Rectangle<int>& drawArea);

        #pragma endregion


        #pragma region Member variables
        
        int             m_frames            { 0 };  //!< How many frames the texture has, assuming it's a spritesheet.
        Point           m_frameDimensions   {  };   //!< The width and height of the spritesheet.

        Rectangle<int>  m_textureSpace      {  };   //!< The total rectangular area of the texture.

        BYTE*           m_pData             {  };   //!< The raw memory data of the texture.

        #pragma endregion
};

#endif // TEXTURE_INCLUDED