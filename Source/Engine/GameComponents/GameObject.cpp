#include "GameObject.hpp"


// Engine headers.
#include <Systems.hpp>


// Engine namespace.
namespace water
{
    #pragma region Constructors and destructor

    GameObject::GameObject (GameObject&& move)
    {
        *this = std::move (move);
    }


    GameObject& GameObject::operator= (GameObject&& move)
    {
        if (this != &move)
        {
            m_position      = std::move (move.m_position);
            m_baseTexture   = move.m_baseTexture;
            m_name          = std::move (move.m_name);
            m_tag           = std::move (move.m_tag);

            // Reset primitives.
            move.m_baseTexture = 0;
        }

        return *this;
    }

    #pragma endregion


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