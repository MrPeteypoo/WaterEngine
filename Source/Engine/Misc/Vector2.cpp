#include "Vector2.hpp"


// STL headers.
#include <complex>



#pragma region Instantiations

template Vector2<int>;
template Vector2<long>;
template Vector2<float>;
template Vector2<double>;

#pragma endregion


#pragma region Constructors

template <typename T> Vector2<T>::Vector2 (Vector2&& move)
{
    *this = std::move (move);
}


template <typename T> Vector2<T>& Vector2<T>::operator= (Vector2&& move)
{
    if (this != &move)
    {
        x = move.x;
        y = move.y;

        const T zero { (T) 0 };

        move.x = zero;
        move.y = zero;
    }

    return *this;
}

#pragma endregion


#pragma region Operators

template <typename T> bool Vector2<T>::operator== (const Vector2& rhs) const
{
    return (x == rhs.x && y == rhs.y);
}


template <typename T> bool Vector2<T>::operator!= (const Vector2& rhs) const
{
    return (x != rhs.x || y != rhs.y);
}


template <typename T> Vector2<T> Vector2<T>::operator+ (const Vector2& rhs) const
{
    return { x + rhs.x, y + rhs.y };
}


template <typename T> Vector2<T> Vector2<T>::operator- (const Vector2& rhs) const
{
    return { x - rhs.x, y - rhs.y };
}


template <typename T> Vector2<T> Vector2<T>::operator* (const Vector2& rhs) const
{
    return { x * rhs.x, y * rhs.y };
}


template <typename T> Vector2<T> Vector2<T>::operator/ (const Vector2& rhs) const
{
    return { x / rhs.x, y / rhs.y };
}


template <typename T> Vector2<T> Vector2<T>::operator+ (const T rhs) const
{
    return { x + rhs, y + rhs };
}


template <typename T> Vector2<T> Vector2<T>::operator- (const T rhs) const
{
    return { x - rhs, y - rhs };
}


template <typename T> Vector2<T> Vector2<T>::operator* (const T rhs) const
{
    return { x * rhs, y * rhs };
}


template <typename T> Vector2<T> Vector2<T>::operator/ (const T rhs) const
{
    return { x / rhs, y / rhs };
}


template <typename T> Vector2<T>& Vector2<T>::operator+= (const Vector2& rhs)
{
    x += rhs.x;
    y += rhs.y;

    return *this;
}


template <typename T> Vector2<T>& Vector2<T>::operator-= (const Vector2& rhs)
{
    x -= rhs.x;
    y -= rhs.y;

    return *this;
}


template <typename T> Vector2<T>& Vector2<T>::operator*= (const Vector2& rhs)
{
    x *= rhs.x;
    y *= rhs.y;

    return *this;
}


template <typename T> Vector2<T>& Vector2<T>::operator/= (const Vector2& rhs)
{
    x /= rhs.x;
    y /= rhs.y;

    return *this;
}


template <typename T> Vector2<T>& Vector2<T>::operator+= (const T rhs)
{
    x += rhs;
    y += rhs;

    return *this;
}


template <typename T> Vector2<T>& Vector2<T>::operator-= (const T rhs)
{
    x -= rhs;
    y -= rhs;

    return *this;
}


template <typename T> Vector2<T>& Vector2<T>::operator*= (const T rhs)
{
    x *= rhs;
    y *= rhs;

    return *this;
}


template <typename T> Vector2<T>& Vector2<T>::operator/= (const T rhs)
{
    x /= rhs;
    y /= rhs;

    return *this;
}

#pragma endregion


#pragma region Movement functionality

template <typename T> void Vector2<T>::setPosition (const T newX, const T newY)
{
    x = newX;
    y = newY;
}


template <typename T> void Vector2<T>::translate (const Vector2& translate)
{
    *this += translate;
}


template <typename T> void Vector2<T>::translate (const T moveX, const T moveY)
{
    x += moveX;
    y += moveY;
}

#pragma endregion


#pragma region Maths functions

template <typename T> T Vector2<T>::squareMagnitude() const
{
    return (x * x + y * y);
}


template <typename T> T Vector2<T>::magnitude() const
{
    return (T) std::sqrt (squareMagnitude());
}


template <typename T> Vector2<T> Vector2<T>::normalised() const
{
    return *this / magnitude();
}


template <typename T> void Vector2<T>::normalise()
{
    *this /= magnitude();
}


template <typename T> T Vector2<T>::dotProduct (const Vector2& lhs, const Vector2& rhs)
{
    // Calculate each component.
    const T x   { lhs.x * rhs.x },
            y   { lhs.y * rhs.y };
    
    // Return the calculated product.
    return ( x + y );
}

#pragma endregion