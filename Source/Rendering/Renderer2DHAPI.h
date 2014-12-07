#if !defined RENDERER2D_HAPI_INCLUDED
#define RENDERER2D_HAPI_INCLUDED


// Engine headers.
#include <Interfaces/IRenderer.h>


// Forward declarations
struct Renderer2DHAPIImpl;



/// <summary>
/// A 2D rendering engine which uses the HAPI library for all drawing requirements.
/// </summary>
class Renderer2DHAPI final : IRenderer2D
{
    public:

        #pragma region Constructors & destructor

        Renderer2DHAPI();
        Renderer2DHAPI (Renderer2DHAPI&& move);
        Renderer2DHAPI& operator= (Renderer2DHAPI&& move);

        ~Renderer2DHAPI() final override;
        
        Renderer2DHAPI (const Renderer2DHAPI& copy) = delete;
        Renderer2DHAPI& operator= (const Renderer2DHAPI&& copy) = delete;

        #pragma endregion

            
        #pragma region Initialisation

        /// <summary> Initialise all data and prepare for rendering. </summary>
        void initialise (const int screenWidth, const int screenHeight) final override;

        /// <summary> Causes all loaded texture data to be wiped. </summary>
        void clearTextureData() final override;

        /// <summary>
        /// Loads a texture from local storage, ready for rendering. 
        /// <returns> Returns the ID for the loaded texture. </returns>
        /// </summary>
        TextureID loadTexture (const std::string& fileLocation, const Vector2D<unsigned int>& frameDimensions) final override;
        
        #pragma endregion


        #pragma region Rendering

        /// <summary> Clears the screen to a black level between 0 and 1, quicker than clearing to a colour. </summary>
        void clearToBlack (const float blackLevel = 0) final override;

        /// <summary> Clears the entire screen to a single colour. Values should be between 0 and 1. </summary>
        void clearToColour (const float red, const float green, const float blue, const float alpha = 1.f) final override;

        /// <summary> 
        /// Requests that a texture be drawn onto the screen at a particular point. 
        /// </summary>
        /// <param name="position"> The top-left point where the texture should render from. </param>
        /// <param name="id"> The ID of the texture to render. </param>
        void drawTexture (const Vector2D<int>& point, const TextureID id, const BlendType blend) final override;

        /// <summary> 
        /// Requests that a texture be drawn onto the screen at a particular point. 
        /// </summary>
        /// <param name="position"> The top-left point where the texture should render from. </param>
        /// <param name="id"> The ID of the texture to render. </param>
        /// <param name="frame"> Which frame to render from the texture. If no frames exist the entire texture will be drawn. </param>
        void drawTexture (const Vector2D<int>& point, const TextureID id, const BlendType blend, const Vector2D<unsigned int>& frame) final override;

        #pragma endregion

    private:  

        Renderer2DHAPIImpl* m_impl { nullptr };   //!< A pointer to the implementation data.
};


#endif // RENDERER_HAPI_INCLUDED