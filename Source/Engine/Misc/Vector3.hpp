#if !defined WATER_VECTOR3_INCLUDED
#define WATER_VECTOR3_INCLUDED


// STL headers.
#include <complex>



/// <summary>
/// A basic mathematical vector struct for a 3D vector.
/// </summary>
template <typename T = float> struct Vector3 final
{
    #pragma region Member variables

    T   x   { (T) 0 },  //!< The x component of the vector.
        y   { (T) 0 },  //!< The y component of the vector.
        z   { (T) 0 };  //!< The z component of the vector.

    #pragma endregion


    #pragma region Constructors and destructor

    /// <summary> The default constructor, allows the initial values of each component to be set. </summary>
    Vector3 (const T newX, const T newY, const T newZ) : x (newX), y (newY), z (newZ) {};

    Vector3()                                   = default;
    Vector3 (const Vector3& copy)               = default;
    Vector3& operator= (const Vector3& copy)    = default;

    Vector3 (Vector3&& move);
    Vector3& operator= (Vector3&& move);

    ~Vector3()                                  = default;

    #pragma endregion


    #pragma region Operators

    /// <summary> Allows the Vector3 to be cast between specialisations. </summary>
    template <typename U> operator Vector3<U>() const   { return { (U) x, (U) y, (U) z }; }

    /// <summary> Checks whether the vector is equal to another. </summary>
    bool operator== (const Vector3& rhs) const;

    /// <summary> Checks whether the vector is not equal to another. </summary>
    bool operator!= (const Vector3& rhs) const;

    /// <summary> Adds two vectors together. </summary>
    Vector3 operator+ (const Vector3& rhs) const;

    /// <summary> Calculates the difference between two vectors. </summary>
    Vector3 operator- (const Vector3& rhs) const;

    /// <summary> Multiples each component of two vectors. </summary>
    Vector3 operator* (const Vector3& rhs) const;

    /// <summary> Divides each component of two vectors. </summary>
    Vector3 operator/ (const Vector3& rhs) const;

    /// <summary> Translates each component of the vector by a value. </summary>
    Vector3 operator+ (const T rhs) const;

    /// <summary> Negatively translates each component of the vector by a value. </summary>
    Vector3 operator- (const T rhs) const;

    /// <summary> Multiples each component of the vector by a value. </summary>
    Vector3 operator* (const T rhs) const;

    /// <summary> Divides each component of the vector by a value. </summary>
    Vector3 operator/ (const T rhs) const;

    /// <summary> Adds a vector onto the current vector. </summary>
    Vector3& operator+= (const Vector3& rhs);

    /// <summary> Subtracts a vector from the current vector. </summary>
    Vector3& operator-= (const Vector3& rhs);

    /// <summary> Multiples each component of the current vector by the given vector. </summary>
    Vector3& operator*= (const Vector3& rhs);

    /// <summary> Divides each component of the current vector by the given vector. </summary>
    Vector3& operator/= (const Vector3& rhs);

    /// <summary> Adds a value onto each component of the current vector. </summary>
    Vector3& operator+= (const T rhs);

    /// <summary> Subtracts a value from each component of the current vector. </summary>
    Vector3& operator-= (const T rhs);

    /// <summary> Multiples each component of the current vector by a value. </summary>
    Vector3& operator*= (const T rhs);

    /// <summary> Divides each component of the current vector by a value. </summary>
    Vector3& operator/= (const T rhs);

    #pragma endregion


    #pragma region Movement functionality

    /// <summary> Sets the position of the vector. </summary>
    void setPosition (const T newX, const T newY, const T newZ);

    /// <summary> Translate the vector by the values stored in the passed vector.
    void translate (const Vector3& translate);

    /// <summary> Translates the each component by the values given. </summary>
    void translate (const T moveX, const T moveY, const T moveZ);

    #pragma endregion


    #pragma region Maths functions

    /// <summary> Returns the squared magnitude of the vector, use for efficiency when comparing magnitudes. </summary>
    T squareMagnitude() const;

    /// <summary> Returns the magnitude of the vector. </summary>
    T magnitude() const;

    /// <summary> Returns a unit vector based on current values. </summary>
    Vector3 normalised() const;

    /// <summary> Converts the vector into a unit vector. </summary>
    void normalise();

    /// <summary> Short hand for Vector3::dotProduct. </summary>
    T dotProduct (const Vector3& rhs) const         { return dotProduct (*this, rhs); }

    /// <summary> Short hand for Vector3::crossProduct. </summary>
    Vector3 crossProduct (const Vector3& rhs) const { return crossProduct (*this, rhs); }

    /// <summary> Calculates the dot/scalar product of two given vectors. </summary>
    static T dotProduct (const Vector3& lhs, const Vector3& rhs);
    
    /// <summary> Calculates the cross/vector product of two given vectors. </summary>
    static Vector3 crossProduct (const Vector3& lhs, const Vector3& rhs);

    #pragma endregion
};


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


#endif