#if !defined WATER_HAPI_TEXTURE_INCLUDED
#define WATER_HAPI_TEXTURE_INCLUDED


// Engine headers.
#include <Interfaces/IRenderer.hpp>
#include <Misc/Rectangle.hpp>
#include <Misc/Vector2.hpp>


// Forward declarations.
struct HAPI_TColour;

using Colour    = HAPI_TColour;
using BYTE      = unsigned char;


// Engine namespace.
namespace water
{
    /// <summary> 
    /// A texture is a self-managed wrapper for any texture to be used in the HAPI engine. 
    /// </summary>
    class HAPITexture final
    {
        public:
        
            #pragma region Constructors and destructor

            HAPITexture()                                   = default;
            HAPITexture (HAPITexture&& move);
            HAPITexture& operator= (HAPITexture&& move);
            ~HAPITexture();
        
            HAPITexture (const HAPITexture& copy)               = delete;
            HAPITexture& operator= (const HAPITexture& copy)    = delete;
        
            #pragma endregion

            #pragma region Initialisation

            /// <summary> Create a texture by loading image data. Assumes there are no individual frames. </summary>
            bool initialise (const std::string& fileLocation);

            /// <summary> Create a texture by loading image data, the frame dimensions specified cause the texture to act like a spritesheet. </summary>
            bool initialise (const std::string& fileLocation, const Point& frameDimensions);

            /// <summary> Create a texture with blank data. </summary>
            bool initialise (const Point& pixelDimensions);

            /// <summary> Create a texture with blank data, the frame dimensions specified cause the texture to act like a spritesheet. </summary>
            bool initialise (const Point& pixelDimensions, const Point& frameDimensions);

            #pragma endregion


            #pragma region Getters and setters

            /// <summary> Checks to see if the texture has loaded any data from an image file. </summary>
            bool hasLoaded() const              { return m_data != nullptr; }

            /// <summary> Returns the raw data, useful for accessing line-by-line. </summary>
            const BYTE* const getData() const   { return m_data; }

            /// <summary> Returns how many frames exist, 0 means the texture is not a spritesheet. </summary>
            int getFrameCount() const           { return m_frames; }

            /// <summary> Sets the frame dimensions to the figures specified, (0, 0) disables spritesheet functionality and anything else enables it. </summary>
            void setFrameDimensions (const Point& dimensions);

            /// <summary> Resets frame information, effectively disabling spritesheet functionality. </summary>
            void resetFrameDimensions();

            #pragma endregion


            #pragma region Loading functionality

            /// <summary> Cleans the currently held data. </summary>
            void cleanUp();
        
            /// <summary> Attempts to load a texture using the file location specified. </summary>
            /// <returns> Whether the texture could be loaded or not. </returns>
            bool loadTexture (const std::string& fileLocation);

            /// <summary> Fills the texture with blank data according to the dimensions specified. </summary>
            /// <returns> Whether the data could be allocated or not. </returns>
            bool fillWithBlankData (const Point& dimensions);
    
            #pragma endregion


            #pragma region Scaling

            /// <summary> Scales a texture by the given ratio, this is very useful for load-time image scaling. </summary>
            /// <param name="ratios"> The X and Y ratio to scale the texture by. </param>
            /// <param name="mode"> The mode to use for scaling. </param>
            bool scaleByFactor (const Vector2<float>& ratios, const FilterMode mode);

            /// <summary> Crops a number of pixels from the right and bottom of the texture. </summary>
            /// <param name="crop"> The values to crop. </param>
            void crop (const Point& crop);

            /// <summmary> Calculates the nearest neighbour to a pixel at the given x and y values. </summary>
            /// <param name="source" The source texture to create a filtered pixel from. </param>
            /// <param name="x"> The X co-ordinate to be calculated. </param>
            /// <param name="y"> The Y co-ordinate to be calculated. </param>
            /// <param name="width"> The width of the texture, allows for increased efficiency. </param>
            static Colour nearestNeighbourPixel (const HAPITexture& source, const float x, const float y, const int width);

            /// <summary> Calculates the bilinear filtered pixel of the given x and y values. The width, right and bottom values allow efficiency to be increased. </summary>
            /// <param name="source"> The source texture to create a filtered pixel from. </param>
            /// <param name="x"> The X co-ordinate to be calculated. </param>
            /// <param name="y"> The Y co-ordinate to be calculated. </param>
            /// <param name="width"> The width of the texture, allows for increased efficiency. </param>
            /// <returns> The calculated filtered pixel. </returns>
            static Colour bilinearFilteredPixel (const HAPITexture& source, const float x, const float y, const int width);

            #pragma endregion


            #pragma region Blitting

            /// <summary> The entry point for blitting functionality. Determines whether blitting is necessary then calls the correct blitting function. </summary>
            /// <param name="target"> The data buffer to write to, this is likely to be the screen. </param>
            /// <param name="blitTo"> The Rectangle representing the blittable area of the target. </param>
            /// <param name="clipTo"> The Rectangle representing the visible area of the target. </param>
            /// <param name="point"> Where the blitting should begin, if this is out-of-bounds then the texture will be clipped. </param>
            /// <param name="frame"> The co-ordinate of the frame to be drawn, (0, 0) should be used for single images. </param>
            /// <param name="blend"> Determines how the texture should be blended, can have a huge impact on speed. </param>
            /// <returns> Whether the blitting was successful. </returns>
            bool blitTo (BYTE* const target, const Rectangle<int>& blitTo, const Rectangle<int>& clipTo, const Point& point, const Point& frame, const BlendType blend);

            /// <summary> An overload which blits the current texture onto the target texture. </summary>
            /// <param name="target"> The target texture to be altered. </param>
            /// <param name="point"> Where the blitting should begin, if this is out-of-bounds then the texture will be clipped. </param>
            /// <param name="frame"> The co-ordinate of the frame to be drawn, (0, 0) should be used for single images. </param>
            /// <param name="blend"> Determines how the texture should be blended, can have a huge impact on speed. </param>
            /// <returns> Whether the blitting was successful. </returns>
            bool blitTo (HAPITexture& target, const Point& point, const Point& frame, const BlendType blend);

            /// <summary> Clears the entire texture to a black level. </summary>
            /// <param name="blackLevel"> A number between 0 and 255 to set the texture to. </param>
            void clearToBlack (const BYTE blackLevel = 0);

            /// <summary> Clears the entire texture to a given colour. </summary>
            /// <param name="colour"> The desired colour of the texture. </param>
            void clearToColour (const Colour& colour);

            #pragma endregion
        
        private:
        
            #pragma region Blitting implementation

            /// <summary> Will blit the texture line-by-line without performing any alpha blending. </summary>
            void blitOpaque (BYTE* const target, const Rectangle<int>& targetSpace, const Point& point, const Point& frameOffset, const Rectangle<int>& drawArea);

            /// <summary> Will blit the texture pixel-by-pixel taking into account alpha values, slower. </summary>
            void blitTransparent (BYTE* const target, const Rectangle<int>& targetSpace, const Point& point, const Point& frameOffset, const Rectangle<int>& drawArea);

            #pragma endregion


            #pragma region Member variables
        
            int             m_frames            { 0 };          //!< How many frames the texture has, assuming it's a spritesheet.
            Point           m_frameDimensions   { 0, 0 };       //!< The width and height of the spritesheet.

            Rectangle<int>  m_textureSpace      { };            //!< The total rectangular area of the texture.

            BYTE*           m_data              { nullptr };    //!< The raw memory data of the texture.

            #pragma endregion
    };
}

#endif