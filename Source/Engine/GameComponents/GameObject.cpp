#include "GameObject.hpp"


// Engine headers.
#include <Systems.hpp>


// Engine namespace.
namespace water
{
    #pragma region Commonly accessed systems

    IAudio& GameObject::audio()
    {
        return Systems::audio();
    }

    /*IInput& GameObject::input()
    {
        return Systems::input();
    }*/

    ILogger& GameObject::logger()
    {
        return Systems::logger();
    }

    IRenderer& GameObject::renderer()
    {
        return Systems::renderer();
    }

    ITime& GameObject::time()
    {
        return Systems::time();
    }

    #pragma endregion
}