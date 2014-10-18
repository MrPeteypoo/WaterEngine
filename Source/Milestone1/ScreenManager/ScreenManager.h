#if !defined SCREEN_MANAGER_INCLUDED
#define SCREEN_MANAGER_INCLUDED


#include <HAPI_lib.h>   // Required for m_screen.




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
        ScreenManager& operator= (const ScreenManager& copy)    = default;

        ~ScreenManager() = default;

        #pragma endregion Constructors and destructor

        
        #pragma region Colouring functionality

        /// <summary> Clears the screen to a black level between 0 and 255, quicker than clearing to a colour. </summary>
        void clearToBlack (const unsigned int blackLevel = 0);

        /// <summary> Clears the entire screen to a single colour. </summary>
        void clearToColour (const HAPI_TColour& colour);

        /// <summary> Sets the colour of an individual pixel. </summary>
        void setPixel (const unsigned int pixelNumber, const HAPI_TColour& colour);

        #pragma endregion Colouring functionality

    private:
        
        const size_t m_kColourSize = sizeof (HAPI_TColour); //!< The correct size of the HAPI_TColour.

        int                 m_screenWidth = 0;      //!< The pixel width of the screen.
        int                 m_screenHeight = 0;     //!< The pixel height of the screen.
        unsigned int        m_screenSize = 0;       //!< How many pixels in total exist on the screen.

        BYTE*               m_screen = nullptr;     //!< A cached copy of the HAPI screen pointer.
};

#endif // SCREEN_MANAGER_INCLUDED