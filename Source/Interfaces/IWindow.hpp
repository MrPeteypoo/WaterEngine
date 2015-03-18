#ifndef WATER_INTERFACE_WINDOW_INCLUDED
#define WATER_INTERFACE_WINDOW_INCLUDED


// STL headers.
#include <string>


// Forward declarations.
using byte = unsigned char;
template <typename T> struct Vector2;


// Engine namespace.
namespace water
{


    /// <summary>
    /// An interface to every window management system in the engine. These are used to create graphical windows and manage
    /// window events such as controlling whether the application should close, etc.
    /// </summary>
    class IWindow
    {
        public:

            // Ensure destructor is virtual since this is an interface.
            virtual ~IWindow() { }


            /////////////////////////
            // Getters and setters //
            /////////////////////////

            /// <summary> Gets the width of the screen resolution. </summary>
            /// <returns> The width in pixels. </returns>
            virtual unsigned int getWidth() const = 0;

            /// <summary> Gets the height of the screen resolution. </summary>
            /// <returns> The height in pixels. </returns>
            virtual unsigned int getHeight() const = 0;

            /// <summary> Gets the position of the window of the screen. </summary>
            /// <returns> The position of the top-left point of the window in pixels. </returns>
            virtual const Vector2<int>& getPosition() const = 0;

            /// <summary> Sets the title of the window, this is displayed to the operating system. </summary>
            /// <param name="title"> The text to display to the OS. </param>
            virtual void setTitle (const std::string& title) = 0;

            /// <summary> Sets the icon to be displayed by the operating system. </summary>
            /// <param name="width"> The width of the icon in pixels. </param>
            /// <param name="height"> The height of the icon in pixels. </param>
            /// <param name="data"> A pointer to the image data. If the width or height values are invalid this could cause memory corruption. </param>
            virtual void setIcon (const unsigned int width, const unsigned int height, const byte* const data) = 0;


            /// <summary> Attempt to resize the window to the desired format. </summary>
            /// <param name="width"> The desired width of the window. </param>
            /// <param name="height"> The desired height of the window. </param>
            /// <param name="fullscreen"> Whether fullscreen mode should be used. </param>
            virtual void resize (const unsigned int width, const unsigned int height, const bool fullscreen) = 0;
    };
}

#endif // WATER_INTERFACE_WINDOW_INCLUDED
