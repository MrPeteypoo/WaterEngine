#if !defined WATER_RENDERER_HAPI_INCLUDED
#define WATER_RENDERER_HAPI_INCLUDED


// STL headers.
#include <unordered_map>


// Engine headers.
#include <Systems/IEngineRenderer.hpp>
#include <Systems/Rendering/HAPITexture.hpp>


// Forward declarations.
using BYTE = unsigned char;


// Engine namespace
namespace water
{
    /// <summary>
    /// A 2D rendering engine which uses the HAPI library for all drawing requirements.
    /// </summary>
    class RendererHAPI final : public IEngineRenderer
    {
        public:

            #pragma region Constructors & destructor

            RendererHAPI()                                      = default;
            RendererHAPI (RendererHAPI&& move);
            RendererHAPI& operator= (RendererHAPI&& move);

            ~RendererHAPI() override final;
        
            RendererHAPI (const RendererHAPI& copy)             = delete;
            RendererHAPI& operator= (const RendererHAPI&& copy) = delete;

            #pragma endregion

            
            #pragma region System management
    
            /// <summary> Initialise all data and prepare for rendering. </summary>
            /// <param name="screenWidth"> The width of the screen resolution. </param>
            /// <param name="screenHeight"> The height of the screen resolution. </param>
            /// <param name="internalWidth"> The width of the internal framebuffer. </param>
            /// <param name="internalHeight"> The height of the internal framebuffer. </param>
            /// <param name="filter"> The filter to be applied when scaling the internal framebuffer to the screen. </param>
            void initialise (const int screenWidth, const int screenHeight, const int internalWidth, const int internalHeight, const FilterMode filter) override final;

            /// <summary> Updates the screen and updates HAPI. </summary>
            /// <returns> Whether HAPI is closing. </returns>
            bool update() override final;

            #pragma endregion


            #pragma region Viewport
            
            /// <summary> Sets the viewport of the renderer. This will not scale drawn textures, it just determines how the world units are interpreted. </summary>
            /// <param name="viewport"> The desired viewable area of the screen in world units. This will be tested for validity. </param>
            void setViewport (const Rectangle<float>& viewport) override final;

            /// <summary> Translates the current viewable area to the point given, maintaining the viewport which has been set. </summary
            /// <param name="translateTo"> The desired top-left point of the viewport. </param>
            void translateViewportTo (const Vector2<float>& translateTo) override final;

            #pragma endregion


            #pragma region Data management

            /// <summary> Loads a texture from local storage, ready for rendering. </summary>
            /// <param name="cropRight"> How many pixels, if any, to crop from the right of the texture. </param>
            /// <param name="cropBottom"> How many pixels, if any, to crop from the bottom of the texture. </param>
            /// <returns> Returns the ID for the loaded texture. </returns>
            TextureID loadTexture (const std::string& fileLocation, const int cropRight = 0, const int cropBottom = 0) override final;

            /// <summary> Creates a blank texture with the specified dimensions, allows for the creation of custom textures. </summary>
            /// <param name="dimensions"> The width and height of the blank texture in pixels. </param>
            /// <returns> The ID of the newly created texture. </returns>
            TextureID createBlankTexture (const Vector2<float>& dimensions) override final;

            /// <summary> Crops a part of a texture, permenantly removing data which will become inaccessible. </summary>
            /// <param name="target"> The target texture to crop. </param>
            /// <param name="right"> How many pixels to remove from the right side of the texture. </param>
            /// <param name="bottom"> How many pixels to remove from the bottom of the texture. </param>
            void cropTexture (const TextureID target, const int right, const int bottom) override final;

            /// <summary> Set the number width and height of a texture in frames. This enables spritesheet functionality. </summary>
            /// <param name="target"> The texture to modify. </param>
            /// <param name="dimensions"> The new frame dimensions of the texture. If the X or Y is zero then spritesheet functionality is disabled. </param>
            void setFrameDimensions (const TextureID target, const Point& dimensions) override final;

            /// <summary> Deletes a loaded texture from the system. </summary>
            /// <param name="texture"> The ID of the texture to delete. </param>
            void removeTexture (const TextureID texture) override final;

            /// <summary> Causes all loaded texture data to be wiped. </summary>
            void clearTextureData() override final;
        
            #pragma endregion


            #pragma region Rendering

            /// <summary> Sets the scaling mode of the renderer. Nearest-neighbour is the fastest but bilinear should provide nicer results. </summary
            /// <param name="mode"> The desired filtering mode to use when upscaling to the screen resolution. </param>
            void setFilteringMode (const FilterMode mode) override final;

            /// <summary> Requests that a texture be drawn onto the screen at a particular point. </summary>
            /// <param name="point"> The top-left point where the texture should render from. This should be in world units. </param>
            /// <param name="id"> The ID of the texture to render. </param>
            void drawToScreen (const Vector2<float>& position, const TextureID id, const BlendType blend) override final;

            /// <summary> Requests that a texture be drawn onto the screen at a particular point. </summary>
            /// <param name="point"> The top-left point where the texture should render from. This should be in world units. </param>
            /// <param name="id"> The ID of the texture to render. </param>
            /// <param name="frame"> Which frame to render from the texture. If no frames exist the entire texture will be drawn. </param>
            void drawToScreen (const Vector2<float>& point, const TextureID id, const Point& frame, const BlendType blend) override final;

            /// <summary> Draws a texture onto another texture, this effect is permanent and cannot be reversed. </summary>
            /// <param name="point"> The target top-left point on the texture to draw onto. This should be in pixels. </param>
            /// <param name="source"> The source texture to draw. </param>
            /// <param name="target"> The target texture to draw onto. </param>
            /// <param name="blend"> The type of alpha blending to perform. </param>
            void drawToTexture (const Vector2<float>& point, const TextureID source, const TextureID target, const BlendType blend) override final;

            /// <summary> Draws a texture onto another texture, this effect is permanent and cannot be reversed. </summary>
            /// <param name="point"> The target top-left point on the texture to draw onto. This should be in pixels. </param>
            /// <param name="source"> The source texture to draw. </param>
            /// <param name="target"> The target texture to draw onto. </param>
            /// <param name="blend"> The type of alpha blending to perform. </param>
            /// <param name="frame"> The frame of the source texture to draw. </param>
            void drawToTexture (const Vector2<float>& point, const TextureID source, const TextureID target, const Point& frame, const BlendType blend) override final;

            #pragma endregion

        private:

            #pragma region Internal workings

            /// <summary> Initialise HAPI with the given screen resolution. </summary>
            void initialiseHAPI (const int width, const int height);

            /// <summary> Initialise scaling values required to support custom resolutions. </summary>
            void initialiseScaling (const int internalWidth, const int internalHeight, const FilterMode filter);

            /// <summary> Calculate the offset required to correct the aspect ratio of the screen resolution so that it matches the internal framebuffer. </summary>
            void calculateScalingOffset();

            #pragma endregion


            #pragma region Implementation data

            BYTE*                                       m_screen          { nullptr };    //!< A pointer to the memory address of the screen buffer.
            Rectangle<int>                              m_screenSpace     { };            //!< A rectangle representing the screen space, used for blitting.
            Rectangle<int>                              m_drawSpace       { };            //!< The actual drawable area of the screen. Useful for clipping.
            Point                                       m_screenOffset    { 0, 0 };       //!< The offset to apply when upscaling, this is useful when maintaining the aspect ratio of the internal resolution.
        
            Vector2<int>                                m_internalRes     { };            //!< The internal resolution of the renderer, required for world to pixel conversion.
            Vector2<float>                              m_textureScale    { 1, 1 };       //!< The scale ratios used to filter loaded textures to a desired size.
            FilterMode                                  m_filter          { };            //!< The filter to be applied during the framebuffer scaling.

            Rectangle<float>                            m_viewport        { };            //!< The visible area of the screen in world units.
            Vector2<float>                              m_worldToPixel    { };            //!< The scalar applied to world units to create a pixel-space vector.

            std::hash<std::string>                      m_hasher          { };            //!< A hashing function used to speed up map lookup at the expense of map insertion.
            std::unordered_map<TextureID, HAPITexture>  m_textures        { };            //!< A container for all loaded texture data.

            #pragma endregion
    };
}

#endif