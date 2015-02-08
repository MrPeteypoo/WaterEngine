#if !defined RENDERER2D_HAPI_INCLUDED
#define RENDERER2D_HAPI_INCLUDED


// Engine headers.
#include <Rendering/IRenderer2D.h>



/// <summary>
/// A 2D rendering engine which uses the HAPI library for all drawing requirements.
/// </summary>
class Renderer2DHAPI final : public IRenderer2D
{
    public:

        #pragma region Constructors & destructor

        Renderer2DHAPI();
        Renderer2DHAPI (Renderer2DHAPI&& move);
        Renderer2DHAPI& operator= (Renderer2DHAPI&& move);

        ~Renderer2DHAPI() override final;
        
        Renderer2DHAPI (const Renderer2DHAPI& copy)             = delete;
        Renderer2DHAPI& operator= (const Renderer2DHAPI&& copy) = delete;

        #pragma endregion

            
        #pragma region Initialisation
    
        /// <summary> Initialise all data and prepare for rendering. </summary>
        /// <param name="screenWidth"> The width of the screen resolution. </param>
        /// <param name="screenHeight"> The height of the screen resolution. </param>
        /// <param name="unitToPixelScale"> How much to scale the position values during the rendering process. </param>
        void initialise (const int screenWidth, const int screenHeight, const Vector2D<float>& unitToPixelScale) override final;

        /// <summary> Causes all loaded texture data to be wiped. </summary>
        void clearTextureData() override final;

        /// <summary> Creates a blank texture with the specified dimensions, allows for the creation of custom textures. </summary>
        /// <param name="textureDimensions"> The width and height of the blank texture. Can represent world units or pixels. </param>
        /// <param name="frameDimensions"> The frame dimensions of the texture, if x or y is zero then it is ignored. </param>
        /// <param name="textureDimensions"> Effects how the texture dimensions are interpreted, if false the values will be scaled, if true they will be in pixels. </param>
        /// <returns> The ID of the newly created texture. </returns>
        TextureID createBlankTexture (const Vector2D<float>& textureDimensions, const Point& frameDimensions, const bool pixelDimensions) override final;

        /// <summary> Loads a texture from local storage, ready for rendering. </summary>
        /// <returns> Returns the ID for the loaded texture. </returns>
        TextureID loadTexture (const std::string& fileLocation, const Point& frameDimensions) override final;

        /// <summary> Scales a texture to an arbitrary width and height value. This is a permanent effect. </summary>
        /// <param name="target"> The texture to modify. </param>
        /// <param name="dimensions"> The desired width and height in pixels for the texture. </param>
        /// <param name="pixelUnits"> Specifies whether the dimensions should be treat as world or pixel units. </param>
        void scaleTexture (const TextureID target, const Vector2D<float>& dimensions, const bool pixelUnits) override final;
        
        #pragma endregion


        #pragma region Rendering

        /// <summary> Clears the screen to a black level between 0 and 1, quicker than clearing to a colour. </summary>
        void clearToBlack (const float blackLevel = 0) override final;

        /// <summary> Clears the entire screen to a single colour. Values should be between 0 and 1. </summary>
        void clearToColour (const float red, const float green, const float blue, const float alpha = 1.f) override final;

        /// <summary> Requests that a texture be drawn onto the screen at a particular point. </summary>
        /// <param name="point"> The top-left point where the texture should render from. </param>
        /// <param name="id"> The ID of the texture to render. </param>
        void drawToScreen (const Vector2D<float>& position, const TextureID id, const BlendType blend) override final;

        /// <summary> Requests that a texture be drawn onto the screen at a particular point. </summary>
        /// <param name="point"> The top-left point where the texture should render from. </param>
        /// <param name="id"> The ID of the texture to render. </param>
        /// <param name="frame"> Which frame to render from the texture. If no frames exist the entire texture will be drawn. </param>
        void drawToScreen (const Vector2D<float>& point, const TextureID id, const BlendType blend, const Point& frame) override final;

        /// <summary> Draws a texture onto another texture, this effect is permanent and cannot be reversed. </summary>
        /// <param name="point"> The target top-left point on the texture to draw onto. </param>
        /// <param name="source"> The source texture to draw. </param>
        /// <param name="target"> The target texture to draw onto. </param>
        /// <param name="blend"> The type of alpha blending to perform. </param>
        void drawToTexture (const Vector2D<float>& point, const TextureID source, const TextureID target, const BlendType blend) override final;

        /// <summary> Draws a texture onto another texture, this effect is permanent and cannot be reversed. </summary>
        /// <param name="point"> The target top-left point on the texture to draw onto. </param>
        /// <param name="source"> The source texture to draw. </param>
        /// <param name="target"> The target texture to draw onto. </param>
        /// <param name="blend"> The type of alpha blending to perform. </param>
        /// <param name="frame"> The frame of the source texture to draw. </param>
        void drawToTexture (const Vector2D<float>& point, const TextureID source, const TextureID target, const BlendType blend, const Point& frame) override final;

        #pragma endregion

    private:

        // Forward declarations
        struct Impl;


        Impl*   m_pImpl { nullptr };   //!< A pointer to the implementation data.
};


#endif // RENDERER_HAPI_INCLUDED