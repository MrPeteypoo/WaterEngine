#include "Rectangle.h"


/// STL headers.
#include <stdexcept>
#include <string>


// Engine headers.
#include <Utility/Maths.h>



#pragma region Instantiations

template Rectangle<int>;
template Rectangle<unsigned int>;
template Rectangle<float>;
template Rectangle<double>;

#pragma endregion


#pragma region Constructors and destructor

template <typename T> Rectangle<T>::Rectangle (const T left, const T top, const T right, const T bottom)
{
    // Test each component to check if it's valid.
    if (left > right || right < left || top > bottom || bottom < top)
    {
        throw std::runtime_error ("Rectangle::Rectangle(): Invalid parameters given (" +    std::to_string (left) + ", " + 
                                                                                            std::to_string (top) + ", " +
                                                                                            std::to_string (right) + ", " + 
                                                                                            std::to_string (bottom) + ")."  );
    }

    m_left = left;
    m_top = top;
    m_right = right;
    m_bottom = bottom;
}


template <typename T> Rectangle<T>::Rectangle (Rectangle&& move)
{
    *this = std::move (move);
}


template <typename T> Rectangle<T>& Rectangle<T>::operator= (Rectangle&& move)
{
    if (this != &move)
    {
        m_left          = move.m_left;
        m_top           = move.m_top;
        m_right         = move.m_right;
        m_bottom        = move.m_bottom;

        // Reset primitives.
        const T zero    = (T) 0;
        move.m_left     = zero;
        move.m_top      = zero;
        move.m_right    = zero;
        move.m_bottom   = zero;
    }

    return *this;
}

#pragma endregion


#pragma region Operators

template <typename T>
template <typename U> Rectangle<T>::operator Rectangle<U>() const
{
    return { (U) m_left, (U) m_top, (U) m_right, (U) m_bottom };
}


template <typename T> bool Rectangle<T>::operator== (const Rectangle& rhs) const
{
    return (m_left == rhs.m_left &&
            m_top == rhs.m_top &&
            m_right == rhs.m_right &&
            m_bottom == rhs.m_bottom    );
}


template <typename T> bool Rectangle<T>::operator!= (const Rectangle& rhs) const
{
    return (m_left != rhs.m_left ||
            m_top != rhs.m_top ||
            m_right != rhs.m_right ||
            m_bottom != rhs.m_bottom    );
}

#pragma endregion


#pragma region Getters and setters

template <typename T> void Rectangle<T>::setLeft (const T left)
{
    if (left > m_right)
    {
        throw std::runtime_error ("Rectangle::setLeft(): Attempt to set the left co-ordinate with a value larger than m_right.");
    }

    m_left = left;
}


template <typename T> void Rectangle<T>::setTop (const T top)
{
    if (top > m_bottom)
    {
        throw std::runtime_error ("Rectangle::setTop(): Attempt to set the top co-ordinate with a value larger than m_bottom.");
    }

    m_top = top;
}


template <typename T> void Rectangle<T>::setRight (const T right)
{
    if (m_right < m_left)
    {
        throw std::runtime_error ("Rectangle::setRight(): Attempt to set the right co-ordinate with a value smaller than m_left.");
    }

    m_right = right;
}


template <typename T> void Rectangle<T>::setBottom (const T bottom)
{
    if (bottom < m_top)
    {
        throw std::runtime_error ("Rectangle::setBottom(): Attempt to set the bottom co-ordinate with a value smaller than m_top.");
    }

    m_bottom = bottom;
}

#pragma endregion 


#pragma region Testing functionality

template <typename T> bool Rectangle<T>::contains (const Rectangle& other) const
{
    return (m_left <= other.m_left &&
            m_top <= other.m_top &&
            m_right >= other.m_right &&
            m_bottom >= other.m_bottom  );
}


template <typename T> bool Rectangle<T>::intersects (const Rectangle& other) const
{
    return (m_left <= other.m_right &&      // A's left to B's right
            m_top <= other.m_bottom &&      // A's top to B's bottom
            m_right >= other.m_left &&      // A's right to B's left
            m_bottom >= other.m_top     );  // A's bottom to B's top
}

#pragma endregion


#pragma region Manipulation functionality

template <typename T> Rectangle<T> Rectangle<T>::clipped (const Rectangle& clip) const
{
    // Clip each element, ensuring we maintain valid values.
    const T left    = clip.m_right < m_left ? clip.m_right  : util::max (m_left, clip.m_left),

            top     = clip.m_bottom < m_top ? clip.m_bottom : util::max (m_top, clip.m_top),
                
            right   = left > m_right        ? left          : util::min (m_right, clip.m_right),

            bottom  = top > m_bottom        ? top           : util::min (m_bottom, clip.m_bottom);

    return { left, top, right, bottom };
}


template <typename T> void Rectangle<T>::clipTo (const Rectangle& clip)
{
    // Clip each element, ensuring we maintain valid values.
    m_left      = clip.m_right < m_left ? clip.m_right  : util::max (m_left, clip.m_left);

    m_top       = clip.m_bottom < m_top ? clip.m_bottom : util::max (m_top, clip.m_top);
    
    m_right     = m_left > m_right      ? m_left        : util::min (m_right, clip.m_right);

    m_bottom    = m_top > m_bottom      ? m_top         : util::min (m_bottom, clip.m_bottom);
}


template <typename T> void Rectangle<T>::translate (const T moveX, const T moveY)
{
    m_left += moveX;
    m_top += moveY;
    m_right += moveX;
    m_bottom += moveY;
}

#pragma endregion