#if !defined VECTOR3D_INCLUDED
#define VECTOR3D_INCLUDED


/// <summary>
/// A basic mathematical vector struct for a 3D vector.
/// </summary>
struct Vector3D final
{
    #pragma region Constructors and destructor

    /// <summary> The default constructor, allows the initial values of each component to be set. </summary>
    Vector3D (const float newX = 0.f, const float newY = 0.f, const float newZ = 0.f) : x (newX), y (newY), z (newZ) {};

    Vector3D (const Vector3D& copy)             = default;
    Vector3D& operator= (const Vector3D& copy)  = default;

    Vector3D (Vector3D&& move);
    Vector3D& operator= (Vector3D&& move);

    ~Vector3D()                                 = default;

    #pragma endregion


    #pragma region Operators

    /// <summary> Checks whether the vector is equal to another. </summary>
    const bool operator== (const Vector3D& rhs) const;

    /// <summary> Checks whether the vector is not equal to another. </summary>
    const bool operator!= (const Vector3D& rhs) const;

    /// <summary> Adds two vectors together. </summary>
    const Vector3D operator+ (const Vector3D& rhs) const;

    /// <summary> Finds the difference between two vectors. </summary>
    const Vector3D operator- (const Vector3D& rhs) const;

    /// <summary> Translates a vector positively by the specified amount. </summary>
    const Vector3D operator+ (const float rhs) const;

    /// <summary> Translates a vector negatively by the specified amount. </summary>
    const Vector3D operator- (const float rhs) const;

    /// <summary> Multiples each component of the vector by a value. </summary>
    const Vector3D operator* (const float rhs) const;

    /// <summary> Divides each component of the vector by a value. </summary>
    const Vector3D operator/ (const float rhs) const;

    /// <summary> Adds a vector onto the current vector. </summary>
    Vector3D& operator+= (const Vector3D& rhs);

    /// <summary> Subtracts a vector from the current vector. </summary>
    Vector3D& operator-= (const Vector3D& rhs);

    /// <summary> Translates the current vector positively by the specified amount. </summary>
    Vector3D& operator+= (const float rhs);

    /// <summary> Translates the current vector negatively by the specified amount. </summary>
    Vector3D& operator-= (const float rhs);

    /// <summary> Multiples each component of the current vector by a value. </summary>
    Vector3D& operator*= (const float rhs);

    /// <summary> Divides each component of the current vector by a value. </summary>
    Vector3D& operator/= (const float rhs);

    #pragma endregion


    #pragma region Movement functionality

    /// <summary> Sets the position of the vector. </summary>
    void setPosition (const float newX = 0.f, const float newY = 0.f, const float newZ = 0.f);

    /// <summary> Translate the vector by the values stored in the passed vector.
    void translate (const Vector3D& translate);

    /// <summary> Translates the each component by the values given. </summary>
    void translate (const float x = 0.f, const float y = 0.f, const float z = 0.f);

    #pragma endregion


    #pragma region Maths functions

    /// <summary> Returns the squared magnitude of the vector, use for efficiency when comparing magnitudes. </summary>
    const float squareMagnitude() const;

    /// <summary> Returns the magnitude of the vector. </summary>
    const float magnitude() const;

    /// <summary> Returns a unit vector based on current values. </summary>
    const Vector3D normalised() const;

    /// <summary> Converts the vector into a unit vector. </summary>
    void normalise();


    #pragma endregion


    #pragma region Member variables

    float x { 0.f };              //!< The x component of the vector.
    float y { 0.f };              //!< The y component of the vector.
    float z { 0.f };              //!< The z component of the vector.

    #pragma endregion
};


#endif // VECTOR3D_INCLUDED