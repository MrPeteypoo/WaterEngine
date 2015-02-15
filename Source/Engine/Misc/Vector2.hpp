#if !defined WATER_VECTOR2_INCLUDED
#define WATER_VECTOR2_INCLUDED


// STL headers.
#include <complex>


/// <summary>
/// A basic mathematical vector struct for a 2D vector.
/// </summary>
template <typename T = float> struct Vector2 final
{
    #pragma region Member variables

    T   x   { (T) 0 },  //!< The x component of the vector.
        y   { (T) 0 };  //!< The y component of the vector.

    #pragma endregion


    #pragma region Constructors and destructor

    /// <summary> The default constructor, allows the initial values of each component to be set. </summary>
    Vector2 (const T newX, const T newY) : x (newX), y (newY) {  };

    Vector2()                                   = default;
    Vector2 (const Vector2& copy)               = default;
    Vector2& operator= (const Vector2& copy)    = default;

    Vector2 (Vector2&& move);
    Vector2& operator= (Vector2&& move);

    ~Vector2()                                  = default;

    #pragma endregion


    #pragma region Operators

    /// <summary> Allows the Vector2 to be cast between specialisations. </summary>
    template <typename U> operator Vector2<U>() const   { return { (U) x, (U) y }; }

    /// <summary> Checks whether the vector is equal to another. </summary>
    bool operator== (const Vector2& rhs) const;

    /// <summary> Checks whether the vector is not equal to another. </summary>
    bool operator!= (const Vector2& rhs) const;

    /// <summary> Adds two vectors together. </summary>
    Vector2 operator+ (const Vector2& rhs) const;

    /// <summary> Finds the difference between two vectors. </summary>
    Vector2 operator- (const Vector2& rhs) const;

    /// <summary> Multiples each component of two vectors. </summary>
    Vector2 operator* (const Vector2& rhs) const;

    /// <summary> Divides each component of two vectors. </summary>
    Vector2 operator/ (const Vector2& rhs) const;

    /// <summary> Translates each component of the vector by a value. </summary>
    Vector2 operator+ (const T rhs) const;

    /// <summary> Negatively translates each component of the vector by a value. </summary>
    Vector2 operator- (const T rhs) const;

    /// <summary> Multiples each component of the vector by a value. </summary>
    Vector2 operator* (const T rhs) const;

    /// <summary> Divides each component of the vector by a value. </summary>
    Vector2 operator/ (const T rhs) const;

    /// <summary> Adds a vector onto the current vector. </summary>
    Vector2& operator+= (const Vector2& rhs);

    /// <summary> Subtracts a vector from the current vector. </summary>
    Vector2& operator-= (const Vector2& rhs);

    /// <summary> Multiples each component of the current vector by the given vector. </summary>
    Vector2& operator*= (const Vector2& rhs);

    /// <summary> Divides each component of the current vector by the given vector. </summary>
    Vector2& operator/= (const Vector2& rhs);

    /// <summary> Adds a value onto each component of the current vector. </summary>
    Vector2& operator+= (const T rhs);

    /// <summary> Subtracts a value from each component of the current vector. </summary>
    Vector2& operator-= (const T rhs);

    /// <summary> Multiples each component of the current vector by a value. </summary>
    Vector2& operator*= (const T rhs);

    /// <summary> Divides each component of the current vector by a value. </summary>
    Vector2& operator/= (const T rhs);

    /// <summary> Return a negative version of the vector. </summary>
    Vector2 operator-() const;

    #pragma endregion


    #pragma region Movement functionality

    /// <summary> Sets the position of the vector. </summary>
    void setPosition (const T newX, const T newY);

    /// <summary> Translate the vector by the values stored in the passed vector.
    void translate (const Vector2& translate);

    /// <summary> Translates the each component by the values given. </summary>
    void translate (const T moveX, const T moveY);

    #pragma endregion


    #pragma region Maths functions

    /// <summary> Returns the squared magnitude of the vector, use for efficiency when comparing magnitudes. </summary>
    T squareMagnitude() const;

    /// <summary> Returns the magnitude of the vector. </summary>
    T magnitude() const;

    /// <summary> Returns a unit vector based on current values. </summary>
    Vector2 normalised() const;

    /// <summary> Converts the vector into a unit vector. </summary>
    void normalise();

    /// <summary> Short hand for Vector2::dotProduct(). </summary>
    T dotProduct (const Vector2& rhs) const { return dotProduct (*this, rhs); }

    /// <summary> Calculates the dot/scalar product of two given vectors. </summary>
    static T dotProduct (const Vector2& lhs, const Vector2& rhs);

    #pragma endregion
};


// Aliases.
using Point = Vector2<int>;


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


template <typename T> Vector2<T> Vector2<T>::operator-() const
{
    return { -x, -y };
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


#endif // WATER_VECTOR2_INCLUDED