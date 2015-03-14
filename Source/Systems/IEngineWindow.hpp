#ifndef WATER_INTERFACE_WINDOW_ENGINE_INCLUDED
#define WATER_INTERFACE_WINDOW_ENGINE_INCLUDED

#include <Interfaces/IWindow.hpp>

namespace water
{
    class IEngineWindow : public IWindow
    {
        public:
            virtual ~IEngineWindow() {}
    };
}

#endif // WATER_INTERFACE_WINDOW_ENGINE_INCLUDED
