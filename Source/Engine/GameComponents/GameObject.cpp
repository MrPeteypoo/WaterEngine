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
            m_velocity      = std::move (move.m_velocity);
            m_frame         = std::move (move.m_frame);
            m_baseTexture   = move.m_baseTexture;
            m_blendType     = move.m_blendType;
            m_name          = std::move (move.m_name);
            m_tag           = std::move (move.m_tag);

            // Reset primitives.
            move.m_baseTexture = 0;
            move.m_blendType = BlendType::Opaque;
        }

        return *this;
    }

    #pragma endregion


    #pragma region Commonly accessed systems

    IAudio& GameObject::audio()
    {
        return Systems::audio();
    }

    IInput& GameObject::input()
    {
        return Systems::input();
    }

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


    #pragma region Setters

    void GameObject::setBaseTextureID (const TextureID texture, const bool removeCurrent)
    {
        // Remove the texture we currently use if necessary.
        if (removeCurrent)
        {
            renderer().removeTexture (m_baseTexture);
        }

        // Enable the new texture.
        m_baseTexture = texture;
    }

    #pragma endregion
}