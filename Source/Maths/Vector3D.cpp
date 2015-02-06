#include "Vector3D.h"


// STL headers.
#include <complex>



#pragma region Instantiations

template Vector3D<int>;
template Vector3D<float>;
template Vector3D<double>;

#pragma endregion


#pragma region Constructors

template <typename T> Vector3D<T>::Vector3D (Vector3D<T>&& move)
{
    *this = std::move (move);
}


template <typename T> Vector3D<T>& Vector3D<T>::operator= (Vector3D&& move)
{
    if (this != &move)
    {
        x = move.x;
        y = move.y;
        z = move.z;

        const T zero { static_cast<T> (0) };

        move.x = zero;
        move.y = zero;
        move.z = zero;
    }

    return *this;
}

#pragma endregion


#pragma region Operators

template <typename T>
template <typename U> Vector3D<T>::operator Vector3D<U>() const 
{ 
    return {    static_cast<U> (x), 
                static_cast<U> (y), 
                static_cast<U> (z) }; 
}

template <typename T> bool Vector3D<T>::operator== (const Vector3D& rhs) const
{
    return (    x == rhs.x && 
                y == rhs.y && 
                z == rhs.z      );
}


template <typename T> bool Vector3D<T>::operator!= (const Vector3D& rhs) const
{
    return (    x != rhs.x ||
                y != rhs.y || 
                z != rhs.z      );
}


template <typename T> Vector3D<T> Vector3D<T>::operator+ (const Vector3D& rhs) const
{
    return {    x + rhs.x,
                y + rhs.y,
                z + rhs.z   };
}


template <typename T> Vector3D<T> Vector3D<T>::operator- (const Vector3D& rhs) const
{
    return {    x - rhs.x,
                y - rhs.y,
                z - rhs.z   };
}


template <typename T> Vector3D<T> Vector3D<T>::operator* (const Vector3D& rhs) const
{
    return {    x * rhs.x,
                y * rhs.y,
                z * rhs.z   };
}


template <typename T> Vector3D<T> Vector3D<T>::operator/ (const Vector3D& rhs) const
{
    return {    x / rhs.x,
                y / rhs.y,
                z / rhs.z   };
}


template <typename T> Vector3D<T> Vector3D<T>::operator+ (const T rhs) const
{
    return {    x + rhs,
                y + rhs,
                z + rhs };
}


template <typename T> Vector3D<T> Vector3D<T>::operator- (const T rhs) const
{
    return {    x - rhs,
                y - rhs,
                z - rhs };
}


template <typename T> Vector3D<T> Vector3D<T>::operator* (const T rhs) const
{
    return {    x * rhs,
                y * rhs,
                z * rhs };
}


template <typename T> Vector3D<T> Vector3D<T>::operator/ (const T rhs) const
{
    return {    x / rhs,
                y / rhs,
                z / rhs };
}


template <typename T> Vector3D<T>& Vector3D<T>::operator+= (const Vector3D& rhs)
{
    x += rhs.x;
    y += rhs.y;
    z += rhs.z;

    return *this;
}


template <typename T> Vector3D<T>& Vector3D<T>::operator-= (const Vector3D& rhs)
{
    x -= rhs.x;
    y -= rhs.y;
    z -= rhs.z;

    return *this;
}


template <typename T> Vector3D<T>& Vector3D<T>::operator*= (const Vector3D& rhs)
{
    x *= rhs.x;
    y *= rhs.y;
    z *= rhs.z;

    return *this;
}


template <typename T> Vector3D<T>& Vector3D<T>::operator/= (const Vector3D& rhs)
{
    x /= rhs.x;
    y /= rhs.y;
    z /= rhs.z;

    return *this;
}


template <typename T> Vector3D<T>& Vector3D<T>::operator+= (const T rhs)
{
    x += rhs;
    y += rhs;
    z += rhs;

    return *this;
}


template <typename T> Vector3D<T>& Vector3D<T>::operator-= (const T rhs)
{
    x -= rhs;
    y -= rhs;
    z -= rhs;

    return *this;
}


template <typename T> Vector3D<T>& Vector3D<T>::operator*= (const T rhs)
{
    x *= rhs;
    y *= rhs;
    z *= rhs;

    return *this;
}


template <typename T> Vector3D<T>& Vector3D<T>::operator/= (const T rhs)
{
    x /= rhs;
    y /= rhs;
    z /= rhs;

    return *this;
}

#pragma endregion


#pragma region Movement functionality

template <typename T> void Vector3D<T>::setPosition (const T newX, const T newY, const T newZ)
{
    x = newX;
    y = newY;
    z = newZ;
}


template <typename T> void Vector3D<T>::translate (const Vector3D& translate)
{
    *this += translate;
}


template <typename T> void Vector3D<T>::translate (const T moveX, const T moveY, const T moveZ)
{
    x += moveX;
    y += moveY;
    z += moveZ;
}

#pragma endregion


#pragma region Maths functions

template <typename T> T Vector3D<T>::squareMagnitude() const
{
    return (x * x + y * y + z * z);
}


template <typename T> T Vector3D<T>::magnitude() const
{
    return (T) std::sqrt (squareMagnitude());
}


template <typename T> Vector3D<T> Vector3D<T>::normalised() const
{
    return *this / magnitude();
}


template <typename T> void Vector3D<T>::normalise()
{
    *this /= magnitude();
}


template <typename T> T Vector3D<T>::dotProduct (const Vector3D& lhs, const Vector3D& rhs)
{
    // Calculate each component.
    const T x   { lhs.x * rhs.x },
            y   { lhs.y * rhs.y },
            z   { lhs.z * rhs.z };
    
    // Return the calculated product.
    return ( x + y - z );
}


template <typename T> Vector3D<T> Vector3D<T>::crossProduct (const Vector3D& lhs, const Vector3D& rhs)
{
    // Calculate the determinants.
    const T x   {  (lhs.y * rhs.z - lhs.z * rhs.y) },
            y   { -(lhs.x * rhs.z - lhs.z * rhs.x) },
            z   {  (lhs.x * rhs.y - lhs.y * rhs.x) };

    return { x, y, z };
}

#pragma endregion