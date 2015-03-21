#if !defined WATER_RENDERER_SFML_INCLUDED
#define WATER_RENDERER_SFML_INCLUDED


// STL headers.
#include <unordered_map>


// Engine headers.
#include <Systems/IEngineRenderer.hpp>
#include <Systems/Rendering/SFMLTexture.hpp>
#include <Misc/Vector2.hpp>


// Engine namespace.
namespace water
{
    // Forward declarations.
    class IEngineWindow;
    class WindowSFML;


    /// <summary>
    /// A 2D rendering engine which uses SFML for rendering tasks.
    /// </summary>
    class RendererSFML final : public IEngineRenderer
    {
        public:

            /////////////////////////////////
            // Constructors and destructor //
            /////////////////////////////////

            /// <summary> The renderer depends upon water::WindowSFML for certain functionality, as such it much be constructed differently. </summary>
            /// <param name="window"> A reference to the window will be kept and used throughout, if this is not a water::WindowSFML object then an exception will be thrown. </param>
            RendererSFML (IEngineWindow* const window);

            RendererSFML (const RendererSFML& copy)             = default;
            RendererSFML (RendererSFML&& move)                  = default;
            RendererSFML& operator= (RendererSFML && move)      = default;
            RendererSFML& operator= (const RendererSFML& copy)  = default;
            ~RendererSFML() override final                      = default;


            ///////////////////////
            // System management //
            ///////////////////////

            /// <summary> Initialise all data and prepare for rendering. </summary>
            /// <param name="internalWidth"> The base width sprites were scaled to match. </param>
            /// <param name="internalHeight"> The base height sprites were scaled to match. </param>
            /// <param name="smooth"> Whether loaded textures should be smoothed or not. </param>
            void initialise (const unsigned int internalWidth, const unsigned int internalHeight, const bool smooth) override final;

            /// <summary> Does nothing. </summary>
            void update() override final {}


            /////////////////////
            // Data management //
            /////////////////////

            /// <summary> Loads a texture from local storage, ready for rendering. Duplicates won't be loaded or cropped. </summary>
            /// <param name="cropRight"> How many pixels, if any, to crop from the right of the texture. </param>
            /// <param name="cropBottom"> How many pixels, if any, to crop from the bottom of the texture. </param>
            /// <returns> Returns the ID for the loaded or duplicate texture. </returns>
            TextureID loadTexture (const std::string& fileLocation, const int cropRight = 0, const int cropBottom = 0) override final;

            /// <summary> Creates a blank texture with the specified dimensions, allows for the creation of custom textures. </summary>
            /// <param name="dimensions"> The width and height of the blank texture in pixels. </param>
            /// <returns> The ID of the newly created texture. </returns>
            virtual TextureID createBlankTexture (const Vector2<float>& dimensions) override final;

            /// <summary> Set the number width and height of a texture in frames. This enables spritesheet functionality. </summary>
            /// <param name="target"> The texture to modify. </param>
            /// <param name="dimensions"> The new frame dimensions of the texture. If the X or Y is zero then spritesheet functionality is disabled. </param>
            void setFrameDimensions (const TextureID target, const Point& dimensions) override final;

            /// <summary> Deletes a loaded texture from the system. </summary>
            /// <param name="texture"> The ID of the texture to delete. </param>
            void removeTexture (const TextureID texture) override final;

            /// <summary> Causes all loaded texture data to be deleted, invalidating all current keys. </summary>
            void clearTextureData() override final;


            //////////////
            // Viewport //
            //////////////

            /// <summary> Sets the viewport of the renderer. This will determine how zoomed in the screen will be. </summary>
            /// <param name="viewport"> The desired viewable area of the screen in world units. This will be tested for validity. </param>
            void setViewport (const Rectangle<float>& viewport) override final;

            /// <summary> Translates the current viewable area to the point given, maintaining the viewport which has been set. </summary
            /// <param name="translateTo"> The desired top-left point of the viewport. </param>
            void translateViewportTo (const Vector2<float>& translateTo) override final;


            ///////////////
            // Rendering //
            ///////////////

            /// <summary> Requests that a texture be drawn onto the screen at a particular point. </summary>
            /// <param name="point"> The top-left point where the texture should render from. </param>
            /// <param name="id"> The ID of the texture to render. </param>
            void drawToScreen (const Vector2<float>& point, const TextureID id, const BlendType blend) override final;

            /// <summary> Requests that a texture be drawn onto the screen at a particular point. </summary>
            /// <param name="point"> The top-left point where the texture should render from. </param>
            /// <param name="id"> The ID of the texture to render. </param>
            /// <param name="frame"> Which frame to render from the texture. If no frames exist the entire texture will be drawn. </param>
            void drawToScreen (const Vector2<float>& point, const TextureID id, const Point& frame, const BlendType blend) override final;

            /// <summary> Draws a texture onto another texture, this effect is permanent and cannot be reversed. </summary>
            /// <param name="point"> The target top-left point on the texture to draw onto. </param>
            /// <param name="source"> The source texture to draw. </param>
            /// <param name="target"> The target texture to draw onto. </param>
            /// <param name="blend"> The type of alpha blending to perform. </param>
            void drawToTexture (const Vector2<float>& point, const TextureID source, const TextureID target, const BlendType blend) override final;

            /// <summary> Draws a texture onto another texture, this effect is permanent and cannot be reversed. </summary>
            /// <param name="point"> The target top-left point on the texture to draw onto. </param>
            /// <param name="source"> The source texture to draw. </param>
            /// <param name="target"> The target texture to draw onto. </param>
            /// <param name="blend"> The type of alpha blending to perform. </param>
            /// <param name="frame"> The frame of the source texture to draw. </param>
            void drawToTexture (const Vector2<float>& point, const TextureID source, const TextureID target, const Point& frame, const BlendType blend) override final;

        private:

            ///////////////////
            // Internal data //
            ///////////////////

            WindowSFML*                                 m_window            { nullptr };    //!< A weak pointer to the SFML window system, a dependancy of an SFML-based renderer.

            unsigned int                                m_internalWidth     { 0 };          //!< The width of the internal resolution, used for scaling sprites.
            unsigned int                                m_internalHeight    { 0 };
            Vector2<float>                              m_spriteScale       { };            //!< How much to scale all sprites drawn.
            bool                                        m_smooth            { true };       //!< Whether loaded textures should be smoothed or not.

            std::hash<std::string>                      m_hasher            { };            //!< The hasher used to speed up run-time mapping of textures to convert strings to size_t.
            std::unordered_map<TextureID, SFMLTexture>  m_textures          { };
    };
}

#endif // WATER_RENDERER_SFML_INCLUDED
