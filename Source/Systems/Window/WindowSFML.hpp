#ifndef WATER_WINDOW_SFML_INCLUDED
#define WATER_WINDOW_SFML_INCLUDED

#include <Systems/IEngineWindow.hpp>
#include <SFML/Window.hpp>

namespace water
{
    class WindowSFML : public IEngineWindow
    {
        public:
            WindowSFML();
            ~WindowSFML();
            WindowSFML(const WindowSFML& other);
            WindowSFML& operator=(const WindowSFML& other);
        protected:
        private:
            sf::Window m_window;
    };
}

#endif // WATER_WINDOW_SFML_INCLUDED
