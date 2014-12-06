#if !defined SCREEN_MANAGER_INCLUDED
#define SCREEN_MANAGER_INCLUDED


// Engine headers.
#include <Maths/Rectangle.h>
#include <Maths/Vector2D.h>


// Forward declarations.
class   Texture;
struct  HAPI_TColour;

using   BYTE    = unsigned char;
using   Colour  = HAPI_TColour;


/// <summary> Represents how an image should be blended when blit using the ScreenManager. </summary>
enum class BlendType : int
{
    Opaque = 0,         //!< Enables fast blitting by disregarding alpha values.
    Transparent = 1     //!< Enables alpha blending when blitting, slower but allows for transparency.
};



/// <summary>
/// ScreenManager provides basic functionality required for a more robust way of managing the rendering capabilities of the HAPI library.
/// </summary>
class ScreenManager final
{
    public:

        #pragma region Constructors and destructor

        /// <summary> 
        /// Create a ScreenManager with valid resolution values. Throws exceptions if the resolution is invalid or the HAPI screen pointer cannot be obtained. 
        /// </summary>
        ScreenManager (const int screenWidth, const int screenHeight);

        ScreenManager (const ScreenManager& copy)               = default;
        ScreenManager& operator= (const ScreenManager& copy)    = default;

        ~ScreenManager()                                        = default;

        #pragma endregion

        
        #pragma region Rendering functionality

        /// <summary> Clears the screen to a black level between 0 and 255, quicker than clearing to a colour. </summary>
        void clearToBlackLevel (const BYTE blackLevel = 0);

        /// <summary> Clears the entire screen to a single colour. </summary>
        void clearToColour (const Colour& colour);

        /// <summary> Blits an image to the screen. Defaults to blending alpha values. </summary>
        void blit (const Vector2D<int>& position, const Texture& texture, const BlendType blendType = BlendType::Opaque);

    private:
        
        /// <summary> Will blit an image line-by-line without performing any alpha blending. </summary>
        void blitOpaque (const Vector2D<int>& position, const Rectangle& drawArea, const Texture& texture);

        /// <summary> Will blit an image pixel-by-pixel taking into account alpha values, slower. </summary>
        void blitTransparent (const Vector2D<int>& position, const Rectangle& drawArea, const Texture& texture);

        #pragma endregion


        #pragma region Member variables

        BYTE*               m_screen        { nullptr };    //!< A cached copy of the HAPI screen pointer. Never to be deleted.
        Rectangle           m_screenRect    { };            //!< The rectangular area of the screen.

        #pragma endregion
};

#endif // SCREEN_MANAGER_INCLUDED