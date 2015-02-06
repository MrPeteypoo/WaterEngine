#if !defined VECTOR2D_INCLUDED
#define VECTOR2D_INCLUDED



/// <summary>
/// A basic mathematical vector struct for a 2D vector.
/// </summary>
template <typename T = float> struct Vector2D final
{
    #pragma region Member variables

    T   x   { (T) 0 },  //!< The x component of the vector.
        y   { (T) 0 };  //!< The y component of the vector.

    #pragma endregion


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
    template <typename U> operator Vector2D<U>() const;

    /// <summary> Checks whether the vector is equal to another. </summary>
    bool operator== (const Vector2D& rhs) const;

    /// <summary> Checks whether the vector is not equal to another. </summary>
    bool operator!= (const Vector2D& rhs) const;

    /// <summary> Adds two vectors together. </summary>
    Vector2D operator+ (const Vector2D& rhs) const;

    /// <summary> Finds the difference between two vectors. </summary>
    Vector2D operator- (const Vector2D& rhs) const;

    /// <summary> Multiples each component of two vectors. </summary>
    Vector2D operator* (const Vector2D& rhs) const;

    /// <summary> Divides each component of two vectors. </summary>
    Vector2D operator/ (const Vector2D& rhs) const;

    /// <summary> Translates each component of the vector by a value. </summary>
    Vector2D operator+ (const T rhs) const;

    /// <summary> Negatively translates each component of the vector by a value. </summary>
    Vector2D operator- (const T rhs) const;

    /// <summary> Multiples each component of the vector by a value. </summary>
    Vector2D operator* (const T rhs) const;

    /// <summary> Divides each component of the vector by a value. </summary>
    Vector2D operator/ (const T rhs) const;

    /// <summary> Adds a vector onto the current vector. </summary>
    Vector2D& operator+= (const Vector2D& rhs);

    /// <summary> Subtracts a vector from the current vector. </summary>
    Vector2D& operator-= (const Vector2D& rhs);

    /// <summary> Multiples each component of the current vector by the given vector. </summary>
    Vector2D& operator*= (const Vector2D& rhs);

    /// <summary> Divides each component of the current vector by the given vector. </summary>
    Vector2D& operator/= (const Vector2D& rhs);

    /// <summary> Adds a value onto each component of the current vector. </summary>
    Vector2D& operator+= (const T rhs);

    /// <summary> Subtracts a value from each component of the current vector. </summary>
    Vector2D& operator-= (const T rhs);

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

    /// <summary> Short hand for Vector2D::dotProduct(). </summary>
    T dotProduct (const Vector2D& rhs) const    { return dotProduct (*this, rhs); }

    /// <summary> Calculates the dot/scalar product of two given vectors. </summary>
    static T dotProduct (const Vector2D& lhs, const Vector2D& rhs);

    #pragma endregion
};


// Aliases
using Point = Vector2D<int>;


#endif // VECTOR2D_INCLUDED