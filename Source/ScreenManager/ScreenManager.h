#if !defined SCREEN_MANAGER_INCLUDED
#define SCREEN_MANAGER_INCLUDED


#include <Misc/Pixel.h>     // Required for setPixel.



/// <summary>
/// ScreenManager provides basic functionality required for a more robust way of managing the rendering capabilities of the HAPI library.
/// </summary>
class ScreenManager final
{
    public:

        #pragma region Constructors and destructor

        /// <summary> Create a ScreenManager with valid resolution values. </summary>
        ScreenManager (const int screenWidth = 1, const int screenHeight = 1);

        ScreenManager (const ScreenManager& copy)               = default;
        ScreenManager& operator= (const ScreenManager& copy);

        ScreenManager (ScreenManager&& move);
        ScreenManager& operator= (ScreenManager&& move);

        ~ScreenManager() = default;

        #pragma endregion

        
        #pragma region Colouring functionality

        /// <summary> Clears the screen to a black level between 0 and 255, quicker than clearing to a colour. </summary>
        void clearToBlack (const unsigned char blackLevel = 0);

        /// <summary> Clears the entire screen to a single colour. </summary>
        void clearToColour (const HAPI_TColour& colour);

        /// <summary> Blits
        //void blit ()
        

        #pragma endregion 

    private:
        
        #pragma region Private helper functions

        /// <summary> Sets the colour of an individual pixel. </summary>
        void setPixel (const Pixel& pixel);

        #pragma endregion


        #pragma region Member variables

        BYTE*               m_screen        { nullptr };    //!< A cached copy of the HAPI screen pointer. Never to be deleted.

        int                 m_screenWidth   { 0 };          //!< The pixel width of the screen.
        int                 m_screenHeight  { 0 };          //!< The pixel height of the screen.
        int                 m_screenSize    { 0 };          //!< How many pixels in total exist on the screen.

        #pragma endregion
};

#endif // SCREEN_MANAGER_INCLUDED