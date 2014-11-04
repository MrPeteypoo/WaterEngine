#if !defined VECTOR2D_INCLUDED
#define VECTOR2D_INCLUDED


/// <summary>
/// A basic mathematical vector struct for a 2D vector.
/// </summary>
struct Vector2D final
{
    #pragma region Constructors and destructor

    /// <summary> The default constructor, allows the initial values of each component to be set. </summary>
    Vector2D (const float newX = 0.f, const float newY = 0.f) : x (newX), y (newY) {};

    Vector2D (const Vector2D& copy)             = default;
    Vector2D& operator= (const Vector2D& copy)  = default;

    Vector2D (Vector2D&& move);
    Vector2D& operator= (Vector2D&& move);

    ~Vector2D()                                 = default;

    #pragma endregion


    #pragma region Operators

    /// <summary> Checks whether the vector is equal to another. </summary>
    bool operator== (const Vector2D& rhs) const;

    /// <summary> Checks whether the vector is not equal to another. </summary>
    bool operator!= (const Vector2D& rhs) const;

    /// <summary> Adds two vectors together. </summary>
    Vector2D operator+ (const Vector2D& rhs) const;

    /// <summary> Finds the difference between two vectors. </summary>
    Vector2D operator- (const Vector2D& rhs) const;

    /// <summary> Multiples each component of the vector by a value. </summary>
    Vector2D operator* (const float rhs) const;

    /// <summary> Divides each component of the vector by a value. </summary>
    Vector2D operator/ (const float rhs) const;

    /// <summary> Adds a vector onto the current vector. </summary>
    Vector2D& operator+= (const Vector2D& rhs);

    /// <summary> Subtracts a vector from the current vector. </summary>
    Vector2D& operator-= (const Vector2D& rhs);

    /// <summary> Multiples each component of the current vector by a value. </summary>
    Vector2D& operator*= (const float rhs);

    /// <summary> Divides each component of the current vector by a value. </summary>
    Vector2D& operator/= (const float rhs);

    #pragma endregion


    #pragma region Movement functionality

    /// <summary> Sets the position of the vector. </summary>
    void setPosition (const float newX = 0.f, const float newY = 0.f);

    /// <summary> Translate the vector by the values stored in the passed vector.
    void translate (const Vector2D& translate);

    /// <summary> Translates the each component by the values given. </summary>
    void translate (const float moveX = 0.f, const float moveY = 0.f);

    #pragma endregion


    #pragma region Maths functions

    /// <summary> Returns the squared magnitude of the vector, use for efficiency when comparing magnitudes. </summary>
    float squareMagnitude() const;

    /// <summary> Returns the magnitude of the vector. </summary>
    float magnitude() const;

    /// <summary> Returns a unit vector based on current values. </summary>
    Vector2D normalised() const;

    /// <summary> Converts the vector into a unit vector. </summary>
    void normalise();

    #pragma endregion


    #pragma region Member variables

    float x { 0.f };              //!< The x component of the vector.
    float y { 0.f };              //!< The y component of the vector.

    #pragma endregion
};


#pragma region Helper functions

/// <summary> Calculates the dot/scalar product of two given vectors. </summary>
float dotProduct (const Vector2D& lhs, const Vector2D& rhs);

#pragma endregion

#endif // Vector2D_INCLUDED