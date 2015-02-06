#include "Vector2D.h"


// STL headers.
#include <complex>



#pragma region Instantiations

template Vector2D<int>;
template Vector2D<float>;
template Vector2D<double>;

#pragma endregion


#pragma region Constructors

template <typename T> Vector2D<T>::Vector2D (Vector2D&& move)
{
    *this = std::move (move);
}


template <typename T> Vector2D<T>& Vector2D<T>::operator= (Vector2D&& move)
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

template <typename T>
template <typename U> Vector2D<T>::operator Vector2D<U>() const
{ 
    return { (U) x, (U) y }; 
}


template <typename T> bool Vector2D<T>::operator== (const Vector2D& rhs) const
{
    return (x == rhs.x && y == rhs.y);
}


template <typename T> bool Vector2D<T>::operator!= (const Vector2D& rhs) const
{
    return (x != rhs.x || y != rhs.y);
}


template <typename T> Vector2D<T> Vector2D<T>::operator+ (const Vector2D& rhs) const
{
    return { x + rhs.x, y + rhs.y };
}


template <typename T> Vector2D<T> Vector2D<T>::operator- (const Vector2D& rhs) const
{
    return { x - rhs.x, y - rhs.y };
}


template <typename T> Vector2D<T> Vector2D<T>::operator* (const Vector2D& rhs) const
{
    return { x * rhs.x, y * rhs.y };
}


template <typename T> Vector2D<T> Vector2D<T>::operator/ (const Vector2D& rhs) const
{
    return { x / rhs.x, y / rhs.y };
}


template <typename T> Vector2D<T> Vector2D<T>::operator+ (const T rhs) const
{
    return { x + rhs, y + rhs };
}


template <typename T> Vector2D<T> Vector2D<T>::operator- (const T rhs) const
{
    return { x - rhs, y - rhs };
}


template <typename T> Vector2D<T> Vector2D<T>::operator* (const T rhs) const
{
    return { x * rhs, y * rhs };
}


template <typename T> Vector2D<T> Vector2D<T>::operator/ (const T rhs) const
{
    return { x / rhs, y / rhs };
}


template <typename T> Vector2D<T>& Vector2D<T>::operator+= (const Vector2D& rhs)
{
    x += rhs.x;
    y += rhs.y;

    return *this;
}


template <typename T> Vector2D<T>& Vector2D<T>::operator-= (const Vector2D& rhs)
{
    x -= rhs.x;
    y -= rhs.y;

    return *this;
}


template <typename T> Vector2D<T>& Vector2D<T>::operator*= (const Vector2D& rhs)
{
    x *= rhs.x;
    y *= rhs.y;

    return *this;
}


template <typename T> Vector2D<T>& Vector2D<T>::operator/= (const Vector2D& rhs)
{
    x /= rhs.x;
    y /= rhs.y;

    return *this;
}


template <typename T> Vector2D<T>& Vector2D<T>::operator+= (const T rhs)
{
    x += rhs;
    y += rhs;

    return *this;
}


template <typename T> Vector2D<T>& Vector2D<T>::operator-= (const T rhs)
{
    x -= rhs;
    y -= rhs;

    return *this;
}


template <typename T> Vector2D<T>& Vector2D<T>::operator*= (const T rhs)
{
    x *= rhs;
    y *= rhs;

    return *this;
}


template <typename T> Vector2D<T>& Vector2D<T>::operator/= (const T rhs)
{
    x /= rhs;
    y /= rhs;

    return *this;
}

#pragma endregion


#pragma region Movement functionality

template <typename T> void Vector2D<T>::setPosition (const T newX, const T newY)
{
    x = newX;
    y = newY;
}


template <typename T> void Vector2D<T>::translate (const Vector2D& translate)
{
    *this += translate;
}


template <typename T> void Vector2D<T>::translate (const T moveX, const T moveY)
{
    x += moveX;
    y += moveY;
}

#pragma endregion


#pragma region Maths functions

template <typename T> T Vector2D<T>::squareMagnitude() const
{
    return (x * x + y * y);
}


template <typename T> T Vector2D<T>::magnitude() const
{
    return (T) std::sqrt (squareMagnitude());
}


template <typename T> Vector2D<T> Vector2D<T>::normalised() const
{
    return *this / magnitude();
}


template <typename T> void Vector2D<T>::normalise()
{
    *this /= magnitude();
}


template <typename T> T Vector2D<T>::dotProduct (const Vector2D& lhs, const Vector2D& rhs)
{
    // Calculate each component.
    const T x   { lhs.x * rhs.x },
            y   { lhs.y * rhs.y };
    
    // Return the calculated product.
    return ( x + y );
}

#pragma endregion