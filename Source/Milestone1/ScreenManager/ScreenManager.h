#if !defined SCREEN_MANAGER_INCLUDED
#define SCREEN_MANAGER_INCLUDED


#include <HAPI_lib.h>



class ScreenManager final
{
    public:
        ScreenManager (unsigned int screenWidth = 0, unsigned int screenHeight = 0) 
                      : m_screenWidth (screenWidth), m_screenHeight (screenHeight) {}

        void clearScreen (const HAPI_TColour& colour);

    private:
        BYTE*               getScreenPointer();
        inline unsigned int getScreenSize()     { return m_screenWidth * m_screenHeight; }


        unsigned int m_screenWidth = 0;
        unsigned int m_screenHeight = 0;

        BYTE* m_screen = nullptr;
};

#endif // SCREEN_MANAGER_INCLUDED