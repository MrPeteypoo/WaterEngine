#include "WindowSFML.hpp"


// STL headers.
#include <stdexcept>


// Engine namespace.
namespace water
{
    ///////////////////////
    // System management //
    ///////////////////////

    void WindowSFML::initialise (const unsigned int width, const unsigned int height, const bool fullscreen, const std::string& title)
    {
        // Attempt to initialise the window.
        if (!resize (width, height, fullscreen))
        {
            // Give a helpful error message!
            if (fullscreen)
            {
                throw std::invalid_argument ("WindowSFML::initialise(), desired resolution is not a valid fullscreen mode. " +
                                             std::to_string (width) + "x" + std::to_string (height) + ".");
            }

            else
            {
                throw std::invalid_argument ("WindowSFML::initialise(), desired width or height value exceeds the desktop resolution. " +
                                             std::to_string (width) + "x" + std::to_string (height) + ".");
            }
        }

        // Set the correct title since we've successfully created a window!
        setTitle (title);
    }


    /////////////////////////
    // Getters and setters //
    /////////////////////////

    void WindowSFML::setTitle (const std::string& title)
    {
        // Update the local copy and the windows copy of the title.
        m_window.setTitle (sf::String (m_title = title));
    }


    bool WindowSFML::resize (const unsigned int width, const unsigned int height, const bool fullscreen)
    {
        // Create the video mode for the window, ready to test the validity of.
        const auto mode  = sf::VideoMode (width, height);
        auto       style = sf::Style::Titlebar | sf::Style::Close;

        // Ensure that the mode is valid if fullscreen mode is desired.
        if (fullscreen)
        {
            if (!mode.isValid())
            {
                return false;
            }

            // Enable fullscreen mode.
            style = style | sf::Style::Fullscreen;
        }

        else
        {
            // Check whether the width or height is larger than the desktop.
            const auto desktop = sf::VideoMode::getDesktopMode();

            if (mode.width > desktop.width || mode.height > desktop.height)
            {
                return false;
            }
        }

        // The video mode must be valid, recreate the window with these settings.
        m_window.create (mode, m_title, style);
        return true;
    }
}
