#if !defined SCREEN_MANAGER_INCLUDED
#define SCREEN_MANAGER_INCLUDED


// Forward declarations.
typedef unsigned char BYTE;
class Texture;
struct HAPI_TColour;


/// <summary>
/// ScreenManager provides basic functionality required for a more robust way of managing the rendering capabilities of the HAPI library.
/// </summary>
class ScreenManager final
{
    public:

        #pragma region Constructors and destructor

        /// <summary> Create a ScreenManager with valid resolution values. Throws exceptions if the resolution is invalid or the
        /// HAPI screen pointer cannot be obtained. </summary>
        ScreenManager (const int screenWidth = 1, const int screenHeight = 1);

        ScreenManager (const ScreenManager& copy)               = default;
        ScreenManager& operator= (const ScreenManager& copy);

        ScreenManager (ScreenManager&& move);
        ScreenManager& operator= (ScreenManager&& move);

        ~ScreenManager() = default;

        #pragma endregion

        
        #pragma region Rendering functionality

        /// <summary> Clears the screen to a black level between 0 and 255, quicker than clearing to a colour. </summary>
        void clearToBlack (const unsigned char blackLevel = 0);

        /// <summary> Clears the entire screen to a single colour. </summary>
        void clearToColour (const HAPI_TColour& colour);

        /// <summary> Blits an image to the screen, taking into account alpha blending. </summary>
        void blit (const int posX, const int posY, const Texture& texture);

        /// <summary> Blits an image to the screen without taking into account alpha blending. </summary>
        void blitOpaque (const int posX, const int posY, const Texture& texture);

        #pragma endregion 

    private:
        
        #pragma region Helper functions

        /// <summary> Returns the colour information of the given pixel on the screen. </summary>
        HAPI_TColour getPixel (const int pixel) const;

        /// <summary> Sets the colour of an individual pixel. </summary>
        void setPixel (const int pixel, const HAPI_TColour& colour);

        #pragma endregion


        #pragma region Member variables

        BYTE*               m_screen        { nullptr };    //!< A cached copy of the HAPI screen pointer. Never to be deleted.

        int                 m_screenWidth   { 0 };          //!< The pixel width of the screen.
        int                 m_screenHeight  { 0 };          //!< The pixel height of the screen.
        int                 m_screenSize    { 0 };          //!< How many pixels in total exist on the screen.

        #pragma endregion
};

#endif // SCREEN_MANAGER_INCLUDED