#include "Collider.hpp"


// STL headers.
#include <utility>


// Engine namespace.
namespace water
{
    #pragma region Constructors

    Collider::Collider (Collider&& move)
    {
        *this = std::move (move);
    }


    Collider& Collider::operator= (Collider&& move)
    {
        if (this != &move)
        {
            m_box = std::move (move.m_box);
            m_layer = move.m_layer;
            m_isTrigger = move.m_isTrigger;

            // Reset primitives
            move.m_layer = false;
            move.m_isTrigger = false;
        }

        return *this;
    }

    #pragma endregion


    #pragma region Setters

    void Collider::setBox (const Rectangle<float>& box)
    {
        if (box.isValid())
        {
            m_box = box;
        }
    }


    void Collider::setLayer (const unsigned int layer)
    {
        if (layer < 32)
        {
            m_layer = layer;
        }
    }

    #pragma endregion
}