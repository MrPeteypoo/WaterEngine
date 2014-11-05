#if !defined VECTOR2D_INCLUDED
#define VECTOR2D_INCLUDED


// STL headers.
#include <complex>



/// <summary>
/// A basic mathematical vector struct for a 2D vector.
/// </summary>
template <typename T> struct Vector2D final
{
    #pragma region Constructors and destructor

    /// <summary> The default constructor, allows the initial values of each component to be set. </summary>
    Vector2D (const T newX, const T newY) : x (newX), y (newY) {  };

    Vector2D()                                  = default;
    Vector2D (const Vector2D& copy)             = default;
    Vector2D& operator= (const Vector2D& copy)  = default;

    Vector2D (Vector2D&& move);
    Vector2D& operator= (Vector2D&& move);

    ~Vector2D()                                 = default;

    #pragma endregion


    #pragma region Operators

    /// <summary> Allows the Vector2D to be cast between specialisations. </summary>
    template <typename U> operator Vector2D<U>() { return { (U) x, (U) y }; }

    /// <summary> Checks whether the vector is equal to another. </summary>
    bool operator== (const Vector2D& rhs) const;

    /// <summary> Checks whether the vector is not equal to another. </summary>
    bool operator!= (const Vector2D& rhs) const;

    /// <summary> Adds two vectors together. </summary>
    Vector2D operator+ (const Vector2D& rhs) const;

    /// <summary> Finds the difference between two vectors. </summary>
    Vector2D operator- (const Vector2D& rhs) const;

    /// <summary> Multiples each component of the vector by a value. </summary>
    Vector2D operator* (const T rhs) const;

    /// <summary> Divides each component of the vector by a value. </summary>
    Vector2D operator/ (const T rhs) const;

    /// <summary> Adds a vector onto the current vector. </summary>
    Vector2D& operator+= (const Vector2D& rhs);

    /// <summary> Subtracts a vector from the current vector. </summary>
    Vector2D& operator-= (const Vector2D& rhs);

    /// <summary> Multiples each component of the current vector by a value. </summary>
    Vector2D& operator*= (const T rhs);

    /// <summary> Divides each component of the current vector by a value. </summary>
    Vector2D& operator/= (const T rhs);

    #pragma endregion


    #pragma region Movement functionality

    /// <summary> Sets the position of the vector. </summary>
    void setPosition (const T newX, const T newY);

    /// <summary> Translate the vector by the values stored in the passed vector.
    void translate (const Vector2D& translate);

    /// <summary> Translates the each component by the values given. </summary>
    void translate (const T moveX, const T moveY);

    #pragma endregion


    #pragma region Maths functions

    /// <summary> Returns the squared magnitude of the vector, use for efficiency when comparing magnitudes. </summary>
    T squareMagnitude() const;

    /// <summary> Returns the magnitude of the vector. </summary>
    T magnitude() const;

    /// <summary> Returns a unit vector based on current values. </summary>
    Vector2D normalised() const;

    /// <summary> Converts the vector into a unit vector. </summary>
    void normalise();

    #pragma endregion


    #pragma region Member variables

    T x { (T) 0 };              //!< The x component of the vector.
    T y { (T) 0 };              //!< The y component of the vector.

    #pragma endregion
};


#pragma region Helper functions


/// <summary> Calculates the dot/scalar product of two given vectors. </summary>
template <typename T> T dotProduct (const Vector2D<T>& lhs, const Vector2D<T>& rhs)
{
    // Calculate each component.
    const T x   { lhs.x * rhs.x },
            y   { lhs.y * rhs.y };
    
    // Return the calculated product.
    return ( x + y );
}


#pragma endregion


#pragma region Implementations


template <typename T> Vector2D<T>::Vector2D (Vector2D<T>&& move)
{
    *this = std::move (move);
}


template <typename T> Vector2D<T>& Vector2D<T>::operator= (Vector2D&& move)
{
    if (this != &move)
    {
        x = move.x;
        y = move.y;

        move.x = (T) 0;
        move.y = (T) 0;
    }

    return *this;
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


template <typename T> T Vector2D<T>::squareMagnitude() const
{
    return (x * x + y * y);
}


template <typename T> T Vector2D<T>::magnitude() const
{
    return std::sqrt (squareMagnitude());
}


template <typename T> Vector2D<T> Vector2D<T>::normalised() const
{
    return *this / magnitude();
}


template <typename T> void Vector2D<T>::normalise()
{
    *this /= magnitude();
}


#pragma endregion


#endif // Vector2D_INCLUDED