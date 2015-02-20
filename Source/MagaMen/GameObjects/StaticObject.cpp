#include "StaticObject.hpp"


// Game namespace.
namespace mm
{
    #pragma region Constructors and destructor

    StaticObject::StaticObject (StaticObject&& move)
    {
        *this = std::move (move);
    }


    StaticObject& StaticObject::operator= (StaticObject&& move)
    {
        if (this != &move)
        {
            GameObject::operator= (std::move (move));
        }

        return *this;
    }

    #pragma endregion


    #pragma region Game functionality

    void StaticObject::render()
    {
        renderer().drawToScreen (m_position, m_baseTexture, m_frame, m_blendType);
    }

    #pragma endregion


    #pragma region Texture handling

    void StaticObject::loadBaseTexture (const std::string& textureName, const bool removeCurrent)
    {
        // Use the functionality already available.
        const auto id = renderer().loadTexture (textureName);
        setBaseTextureID (id, removeCurrent);
    }

    #pragma endregion
}