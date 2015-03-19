#ifndef WATER_WINDOW_SFML_INCLUDED
#define WATER_WINDOW_SFML_INCLUDED


// STL headers.
#include <memory>


// Engine headers.
#include <Systems/IEngineWindow.hpp>
#include <Misc/Vector2.hpp>


// Third party headers.
#include <SFML/Graphics/RenderWindow.hpp>


// Engine namespace.
namespace water
{
    /// <summary>
    /// A window management system which acts as a wrapper for the sf::RenderWindow class.
    /// </summary>
    class WindowSFML final : public IEngineWindow
    {
        public:

            /////////////////////////////////
            // Constructors and destructor //
            /////////////////////////////////

            WindowSFML()                                    = default;
            ~WindowSFML() override final                    = default;


            WindowSFML (const WindowSFML& other)            = delete;
            WindowSFML (WindowSFML&& move)                  = delete;
            WindowSFML& operator= (const WindowSFML& other) = delete;
            WindowSFML& operator= (WindowSFML&& move)       = delete;


            ///////////////////////
            // System management //
            ///////////////////////

            /// <summary> Initialise the system, opening the window with the specified settings. </summary>
            /// <param name="width"> The desired width of the window in pixels. </param>
            /// <param name="height"> The desired height of the window in pixels. </param>
            /// <param name="fullscreen"> Whether the window should be fullscreened or not. </param>
            /// <param name="title"> The title text to display to the operating system. </param>
            void initialise (const unsigned int width, const unsigned int height, const bool fullscreen, const std::string& title) override final;

            /// <summary> Pool all operating system events. </summary>
            /// <returns> Whether the application should close or not. </returns>
            bool update() override final;

            /// <summary> Calls the display command to render the screen. </summary>
            void endFrame() override final;


            /////////////////////////
            // Getters and setters //
            /////////////////////////

            /// <summary> Gets the width of the screen resolution. </summary>
            /// <returns> The width in pixels. </returns>
            unsigned int getWidth() const override final { return m_width; }

            /// <summary> Gets the height of the screen resolution. </summary>
            /// <returns> The height in pixels. </returns>
            unsigned int getHeight() const override final { return m_height; }

            /// <summary> Gets the position of the window of the screen. </summary>
            /// <returns> The position of the top-left point of the window in pixels. </returns>
            const Vector2<int>& getPosition() const override final { return m_position; }

            /// <summary> Gets the title of the window. </summary>
            /// <returns> The text being displayed to the operating system. </returns>
            const std::string& getTitle() const override final { return m_title; }

            /// <summary> Sets the title of the window, this is displayed to the operating system. </summary>
            /// <param name="title"> The text to display to the OS. </param>
            void setTitle (const std::string& title) override final;

            /// <summary> Sets the icon to be displayed by the operating system. </summary>
            /// <param name="width"> The width of the icon in pixels. </param>
            /// <param name="height"> The height of the icon in pixels. </param>
            /// <param name="data"> A pointer to the image data. If the width or height values are invalid this could cause memory corruption. </param>
            void setIcon (const unsigned int width, const unsigned int height, const byte* const data) override final;

            /// <summary> Attempt to resize the window to the desired format. </summary>
            /// <param name="width"> The desired width of the window. </param>
            /// <param name="height"> The desired height of the window. </param>
            /// <param name="fullscreen"> Whether fullscreen mode should be used. </param>
            /// <returns> Whether the window was successfully resized. </returns>
            bool resize (const unsigned int width, const unsigned int height, const bool fullscreen) override final;

        private:

            ////////////////////
            // Implementation //
            ////////////////////

            /// <summary> Processes each operating system event. </summary>
            /// <returns> Whether the application should close or not. </returns>
            bool pollEvents();


            ///////////////////
            // Internal data //
            ///////////////////

            /// Allow the SFML renderer to have direct access to the WindowSFML contents.
            friend class RendererSFML;

            sf::RenderWindow                    m_window    { };    //!< The window used for displaying the game.
            unsigned int                        m_width     { 0 };  //!< The width of the window.
            unsigned int                        m_height    { 0 };  //!< The height of the window.
            Vector2<int>                        m_position  { };    //!< A cache of the window position, updated every frame.
            std::string                         m_title     = "";   //!< The title of the window, this gets displayed to the OS.
    };
}

#endif // WATER_WINDOW_SFML_INCLUDED
