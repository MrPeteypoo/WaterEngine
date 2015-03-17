#if !defined WATER_INTERFACE_RENDERER_INCLUDED
#define WATER_INTERFACE_RENDERER_INCLUDED


// STL headers.
#include <string>


// Forward declarations.
template <typename T> class Rectangle;
template <typename T> struct Vector2;
using Point = Vector2<int>;


// Engine namespace.
namespace water
{
    // Aliases.
    using TextureID = size_t;


    /// <summary>
    /// The filtering modes available to the rendering system. This has a huge impact on speed.
    /// </summary>
    enum class FilterMode : int
    {
        None                = 0,    //!< Don't apply any up or downscaling filter.
        NearestNeighbour    = 1,    //!< Copies neighbouring pixels to create an upscaled effect.
        Bilinear            = 2     //!< Blends pixels together to give the illusion of more detail.
    };


    /// <summary>
    /// Represents how an image should be blended when blit using the ScreenManager.
    /// </summary>
    enum class BlendType : int
    {
        Opaque      = 0,    //!< Enables fast blitting by disregarding alpha values.
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


            //////////////
            // Viewport //
            //////////////

            /// <summary> Sets the viewport of the renderer. This will not scale drawn textures, it just determines how the world units are interpreted. </summary>
            /// <param name="viewport"> The desired viewable area of the screen in world units. This will be tested for validity. </param>
            virtual void setViewport (const Rectangle<float>& viewport) = 0;

            /// <summary> Translates the current viewable area to the point given, maintaining the viewport which has been set. </summary
            /// <param name="translateTo"> The desired top-left point of the viewport. </param>
            virtual void translateViewportTo (const Vector2<float>& translateTo) = 0;


            /////////////////////
            // Data management //
            /////////////////////

            /// <summary> Loads a texture from local storage, ready for rendering. </summary>
            /// <param name="cropRight"> How many pixels, if any, to crop from the right of the texture. </param>
            /// <param name="cropBottom"> How many pixels, if any, to crop from the bottom of the texture. </param>
            /// <returns> Returns the ID for the loaded texture. </returns>
            virtual TextureID loadTexture (const std::string& fileLocation, const int cropRight = 0, const int cropBottom = 0) = 0;

            /// <summary> Creates a blank texture with the specified dimensions, allows for the creation of custom textures. </summary>
            /// <param name="dimensions"> The width and height of the blank texture in pixels. </param>
            /// <returns> The ID of the newly created texture. </returns>
            virtual TextureID createBlankTexture (const Vector2<float>& dimensions) = 0;

            /// <summary> Crops a part of a texture, permenantly removing data which will become inaccessible. </summary>
            /// <param name="target"> The target texture to crop. </param>
            /// <param name="right"> How many pixels to remove from the right side of the texture. </param>
            /// <param name="bottom"> How many pixels to remove from the bottom of the texture. </param>
            virtual void cropTexture (const TextureID target, const int right, const int bottom) = 0;

            /// <summary> Set the number width and height of a texture in frames. This enables spritesheet functionality. </summary>
            /// <param name="target"> The texture to modify. </param>
            /// <param name="dimensions"> The new frame dimensions of the texture. If the X or Y is zero then spritesheet functionality is disabled. </param>
            virtual void setFrameDimensions (const TextureID target, const Point& dimensions) = 0;

            /// <summary> Deletes a loaded texture from the system. </summary>
            /// <param name="texture"> The ID of the texture to delete. </param>
            virtual void removeTexture (const TextureID texture) = 0;

            /// <summary> Causes all loaded texture data to be deleted, invalidating all current keys. </summary>
            virtual void clearTextureData() = 0;


            ///////////////
            // Rendering //
            ///////////////

            /// <summary> Sets the scaling mode of the renderer. Nearest-neighbour is the fastest but bilinear should provide nicer results. </summary
            /// <param name="mode"> The desired filtering mode to use when upscaling to the screen resolution. </param>
            virtual void setFilteringMode (const FilterMode mode) = 0;

            /// <summary> Requests that a texture be drawn onto the screen at a particular point. </summary>
            /// <param name="point"> The top-left point where the texture should render from. </param>
            /// <param name="id"> The ID of the texture to render. </param>
            virtual void drawToScreen (const Vector2<float>& point, const TextureID id, const BlendType blend) = 0;

            /// <summary> Requests that a texture be drawn onto the screen at a particular point. </summary>
            /// <param name="point"> The top-left point where the texture should render from. </param>
            /// <param name="id"> The ID of the texture to render. </param>
            /// <param name="frame"> Which frame to render from the texture. If no frames exist the entire texture will be drawn. </param>
            virtual void drawToScreen (const Vector2<float>& point, const TextureID id, const Point& frame, const BlendType blend) = 0;

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
            virtual void drawToTexture (const Vector2<float>& point, const TextureID source, const TextureID target, const Point& frame, const BlendType blend) = 0;
    };
}

#endif
