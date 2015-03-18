#ifndef WATER_INTERFACE_WINDOW_ENGINE_INCLUDED
#define WATER_INTERFACE_WINDOW_ENGINE_INCLUDED


// Engine headers.
#include <Interfaces/IWindow.hpp>


// Engine namespace.
namespace water
{
    /// <summary>
    /// An interface to IWindow systems which allows critical functions to be performed by the Engine whilst being hidden from the user.
    /// </summary>
    class IEngineWindow : public IWindow
    {
        public:

            // Ensure destructor is virtual since this is an interface.
            virtual ~IEngineWindow() override { }


            ///////////////////////
            // System management //
            ///////////////////////

            /// <summary> Initialise the system, opening the window with the specified settings. </summary>
            /// <param name="width"> The desired width of the window in pixels. </param>
            /// <param name="height"> The desired height of the window in pixels. </param>
            /// <param name="fullscreen"> Whether the window should be fullscreened or not. </param>
            /// <param name="title"> The title text to display to the operating system. </param>
            virtual void initialise (const unsigned int width, const unsigned int height, const bool fullscreen, const std::string& title) = 0;

            /// <summary> Update the system, ensuring all window events are handled and such. </summary>
            /// <returns> Whether an error occurred, normally this should close the application. </returns>
            virtual bool update() = 0;

            /// <summary> Perform any necessary end-of-frame actions. </summary>
            virtual void endFrame() = 0;
    };
}

#endif // WATER_INTERFACE_WINDOW_ENGINE_INCLUDED
