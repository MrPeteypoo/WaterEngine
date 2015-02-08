#if !defined VECTOR2_INCLUDED
#define VECTOR2_INCLUDED



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

#endif // VECTOR2_INCLUDED