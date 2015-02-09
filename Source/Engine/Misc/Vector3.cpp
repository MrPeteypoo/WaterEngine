#include "Vector3.h"


// STL headers.
#include <complex>



#pragma region Instantiations

template Vector3<int>;
template Vector3<float>;
template Vector3<double>;

#pragma endregion


#pragma region Constructors

template <typename T> Vector3<T>::Vector3 (Vector3<T>&& move)
{
    *this = std::move (move);
}


template <typename T> Vector3<T>& Vector3<T>::operator= (Vector3&& move)
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

template <typename T> bool Vector3<T>::operator== (const Vector3& rhs) const
{
    return (    x == rhs.x && 
                y == rhs.y && 
                z == rhs.z      );
}


template <typename T> bool Vector3<T>::operator!= (const Vector3& rhs) const
{
    return (    x != rhs.x ||
                y != rhs.y || 
                z != rhs.z      );
}


template <typename T> Vector3<T> Vector3<T>::operator+ (const Vector3& rhs) const
{
    return {    x + rhs.x,
                y + rhs.y,
                z + rhs.z   };
}


template <typename T> Vector3<T> Vector3<T>::operator- (const Vector3& rhs) const
{
    return {    x - rhs.x,
                y - rhs.y,
                z - rhs.z   };
}


template <typename T> Vector3<T> Vector3<T>::operator* (const Vector3& rhs) const
{
    return {    x * rhs.x,
                y * rhs.y,
                z * rhs.z   };
}


template <typename T> Vector3<T> Vector3<T>::operator/ (const Vector3& rhs) const
{
    return {    x / rhs.x,
                y / rhs.y,
                z / rhs.z   };
}


template <typename T> Vector3<T> Vector3<T>::operator+ (const T rhs) const
{
    return {    x + rhs,
                y + rhs,
                z + rhs };
}


template <typename T> Vector3<T> Vector3<T>::operator- (const T rhs) const
{
    return {    x - rhs,
                y - rhs,
                z - rhs };
}


template <typename T> Vector3<T> Vector3<T>::operator* (const T rhs) const
{
    return {    x * rhs,
                y * rhs,
                z * rhs };
}


template <typename T> Vector3<T> Vector3<T>::operator/ (const T rhs) const
{
    return {    x / rhs,
                y / rhs,
                z / rhs };
}


template <typename T> Vector3<T>& Vector3<T>::operator+= (const Vector3& rhs)
{
    x += rhs.x;
    y += rhs.y;
    z += rhs.z;

    return *this;
}


template <typename T> Vector3<T>& Vector3<T>::operator-= (const Vector3& rhs)
{
    x -= rhs.x;
    y -= rhs.y;
    z -= rhs.z;

    return *this;
}


template <typename T> Vector3<T>& Vector3<T>::operator*= (const Vector3& rhs)
{
    x *= rhs.x;
    y *= rhs.y;
    z *= rhs.z;

    return *this;
}


template <typename T> Vector3<T>& Vector3<T>::operator/= (const Vector3& rhs)
{
    x /= rhs.x;
    y /= rhs.y;
    z /= rhs.z;

    return *this;
}


template <typename T> Vector3<T>& Vector3<T>::operator+= (const T rhs)
{
    x += rhs;
    y += rhs;
    z += rhs;

    return *this;
}


template <typename T> Vector3<T>& Vector3<T>::operator-= (const T rhs)
{
    x -= rhs;
    y -= rhs;
    z -= rhs;

    return *this;
}


template <typename T> Vector3<T>& Vector3<T>::operator*= (const T rhs)
{
    x *= rhs;
    y *= rhs;
    z *= rhs;

    return *this;
}


template <typename T> Vector3<T>& Vector3<T>::operator/= (const T rhs)
{
    x /= rhs;
    y /= rhs;
    z /= rhs;

    return *this;
}

#pragma endregion


#pragma region Movement functionality

template <typename T> void Vector3<T>::setPosition (const T newX, const T newY, const T newZ)
{
    x = newX;
    y = newY;
    z = newZ;
}


template <typename T> void Vector3<T>::translate (const Vector3& translate)
{
    *this += translate;
}


template <typename T> void Vector3<T>::translate (const T moveX, const T moveY, const T moveZ)
{
    x += moveX;
    y += moveY;
    z += moveZ;
}

#pragma endregion


#pragma region Maths functions

template <typename T> T Vector3<T>::squareMagnitude() const
{
    return (x * x + y * y + z * z);
}


template <typename T> T Vector3<T>::magnitude() const
{
    return (T) std::sqrt (squareMagnitude());
}


template <typename T> Vector3<T> Vector3<T>::normalised() const
{
    return *this / magnitude();
}


template <typename T> void Vector3<T>::normalise()
{
    *this /= magnitude();
}


template <typename T> T Vector3<T>::dotProduct (const Vector3& lhs, const Vector3& rhs)
{
    // Calculate each component.
    const T x   { lhs.x * rhs.x },
            y   { lhs.y * rhs.y },
            z   { lhs.z * rhs.z };
    
    // Return the calculated product.
    return ( x + y - z );
}


template <typename T> Vector3<T> Vector3<T>::crossProduct (const Vector3& lhs, const Vector3& rhs)
{
    // Calculate the determinants.
    const T x   {  (lhs.y * rhs.z - lhs.z * rhs.y) },
            y   { -(lhs.x * rhs.z - lhs.z * rhs.x) },
            z   {  (lhs.x * rhs.y - lhs.y * rhs.x) };

    return { x, y, z };
}

#pragma endregion