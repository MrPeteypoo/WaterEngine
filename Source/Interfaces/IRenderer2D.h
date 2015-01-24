#if !defined INTERFACE_RENDERER_INCLUDED
#define INTERFACE_RENDERER_INCLUDED


// STL headers.
#include <string>


// Engine headers.
#include <Rendering/BlendType.h>


// Forward declarations.
using TextureID = size_t;
template <typename T> struct Vector2D;



/// <summary> Represents how an image should be blended when blit using the ScreenManager. </summary>
enum class BlendType : int
{
    Opaque = 0,         //!< Enables fast blitting by disregarding alpha values.
    Transparent = 1     //!< Enables alpha blending when blitting, slower but allows for transparency.
};



/// <summary> An interface for 2D rendering systems, used for platform-independant drawing functionality. </summary>
class IRenderer2D
{
    public:

        // Ensure destructor is virtual since this is an interface.
        virtual ~IRenderer2D() {}

    
        /// <summary> Initialise all data and prepare for rendering. </summary>
        virtual void initialise (const int screenWidth, const int screenHeight) = 0;

        /// <summary> Causes all loaded texture data to be deleted, invalidating all current keys. </summary>
        virtual void clearTextureData() = 0;

        /// <summary>
        /// Loads a texture from local storage, ready for rendering. 
        /// <returns> Returns the ID for the loaded texture. </returns>
        /// </summary>
        virtual TextureID loadTexture (const std::string& fileLocation, const Vector2D<unsigned int>& frameDimensions) = 0;


        /// <summary> Clears the screen to a black level between 0 and 255, quicker than clearing to a colour. </summary>
        virtual void clearToBlack (const float blackLevel = 0) = 0;        

        /// <summary> Clears the entire screen to a single colour. </summary>
        virtual void clearToColour (const float red, const float green, const float blue, const float alpha = 1.f) = 0;

        /// <summary> 
        /// Requests that a texture be drawn onto the screen at a particular point. 
        /// </summary>
        /// <param name="point"> The top-left point where the texture should render from. </param>
        /// <param name="id"> The ID of the texture to render. </param>
        virtual void drawTexture (const Vector2D<int>& point, const TextureID id, const BlendType blend) = 0;

        /// <summary> 
        /// Requests that a texture be drawn onto the screen at a particular point. 
        /// </summary>
        /// <param name="position"> The top-left point where the texture should render from. </param>
        /// <param name="id"> The ID of the texture to render. </param>
        /// <param name="frame"> Which frame to render from the texture. If no frames exist the entire texture will be drawn. </param>
        virtual void drawTexture (const Vector2D<int>& point, const TextureID id, const BlendType blend, const Vector2D<unsigned int>& frame) = 0;
};


#endif // INTERFACE_RENDERER_INCLUDED