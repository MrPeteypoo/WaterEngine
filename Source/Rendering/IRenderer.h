#if !defined INTERFACE_RENDERER_INCLUDED
#define INTERFACE_RENDERER_INCLUDED


// STL headers.
#include <string>


// Forward declarations.
template <typename T> struct Vector2;
using Point = Vector2<int>;
using TextureID = size_t;



/// <summary> 
/// Represents how an image should be blended when blit using the ScreenManager. 
/// </summary>
enum class BlendType : int
{
    Opaque = 0,         //!< Enables fast blitting by disregarding alpha values.
    Transparent = 1     //!< Enables alpha blending when blitting, slower but allows for transparency.
};



/// <summary> 
/// An interface for 2D rendering systems, used for platform-independant drawing functionality. 
/// </summary>
class IRenderer
{
    public:

        // Ensure destructor is virtual since this is an interface.
        virtual ~IRenderer() {}


        #pragma region Initialisation
    
        /// <summary> Initialise all data and prepare for rendering. </summary>
        /// <param name="screenWidth"> The width of the screen resolution. </param>
        /// <param name="screenHeight"> The height of the screen resolution. </param>
        /// <param name="unitToPixelScale"> How much to scale the position values during the rendering process. </param>
        virtual void initialise (const int screenWidth, const int screenHeight, const Vector2<float>& unitToPixelScale) = 0;

        /// <summary> Causes all loaded texture data to be deleted, invalidating all current keys. </summary>
        virtual void clearTextureData() = 0;

        /// <summary> Creates a blank texture with the specified dimensions, allows for the creation of custom textures. </summary>
        /// <param name="textureDimensions"> The width and height of the blank texture. Can represent world units or pixels. </param>
        /// <param name="frameDimensions"> The frame dimensions of the texture, if x or y is zero then it is ignored. </param>
        /// <param name="textureDimensions"> Effects how the texture dimensions are interpreted, if false the values will be scaled, if true they will be in pixels. </param>
        /// <returns> The ID of the newly created texture. </returns>
        virtual TextureID createBlankTexture (const Vector2<float>& textureDimensions, const Point& frameDimensions, const bool pixelDimensions) = 0;

        /// <summary> Loads a texture from local storage, ready for rendering. </summary>
        /// <returns> Returns the ID for the loaded texture. </returns>
        virtual TextureID loadTexture (const std::string& fileLocation, const Point& frameDimensions) = 0;

        /// <summary> Scales a texture to an arbitrary width and height value. This is a permanent effect. </summary>
        /// <param name="target"> The texture to modify. </param>
        /// <param name="dimensions"> The desired width and height in pixels for the texture. </param>
        /// <param name="pixelUnits"> Specifies whether the dimensions should be treat as world or pixel units. </param>
        virtual void scaleTexture (const TextureID target, const Vector2<float>& dimensions, const bool pixelUnits) = 0;

        #pragma endregion


        #pragma region Rendering
        
        /// <summary> Clears the screen to a black level between 0 and 1, quicker than clearing to a colour. </summary>
        virtual void clearToBlack (const float blackLevel = 0) = 0;        

        /// <summary> Clears the entire screen to a single colour. </summary>
        virtual void clearToColour (const float red, const float green, const float blue, const float alpha = 1.f) = 0;

        /// <summary> Requests that a texture be drawn onto the screen at a particular point. </summary>
        /// <param name="point"> The top-left point where the texture should render from. </param>
        /// <param name="id"> The ID of the texture to render. </param>
        virtual void drawToScreen (const Vector2<float>& point, const TextureID id, const BlendType blend) = 0;

        /// <summary> Requests that a texture be drawn onto the screen at a particular point. </summary>
        /// <param name="point"> The top-left point where the texture should render from. </param>
        /// <param name="id"> The ID of the texture to render. </param>
        /// <param name="frame"> Which frame to render from the texture. If no frames exist the entire texture will be drawn. </param>
        virtual void drawToScreen (const Vector2<float>& point, const TextureID id, const BlendType blend, const Point& frame) = 0;

        /// <summary> Draws a texture onto another texture, this effect is permanent and cannot be reversed. </summary>
        /// <param name="point"> The target top-left point on the texture to draw onto. </param>
        /// <param name="source"> The source texture to draw. </param>
        /// <param name="target"> The target texture to draw onto. </param>
        /// <param name="blend"> The type of alpha blending to perform. </param>
        virtual void drawToTexture (const Vector2<float>& point, const TextureID source, const TextureID target, const BlendType blend) = 0;

        /// <summary> Draws a texture onto another texture, this effect is permanent and cannot be reversed. </summary>
        /// <param name="point"> The target top-left point on the texture to draw onto. </param>
        /// <param name="source"> The source texture to draw. </param>
        /// <param name="target"> The target texture to draw onto. </param>
        /// <param name="blend"> The type of alpha blending to perform. </param>
        /// <param name="frame"> The frame of the source texture to draw. </param>
        virtual void drawToTexture (const Vector2<float>& point, const TextureID source, const TextureID target, const BlendType blend, const Point& frame) = 0;

        #pragma endregion
};

#endif // INTERFACE_RENDERER_INCLUDED