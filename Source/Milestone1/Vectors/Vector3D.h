#if !defined VECTOR3_INCLUDED
#define VECTOR3_INCLUDED


/// <summary>
/// A basic mathematical vector class for a 3D vector.
/// </summary>
class Vector3D final
{
    public:
    
        #pragma region Constructors and destructor

        /// <summary> The default constructor, allows the initial values of each component to be set. </summary>
        Vector3D (const float X = 0.f, const float Y = 0.f, const float Z = 0.f) : x (X), y (Y), z (Z) {};

        Vector3D (const Vector3D& copy)             = default;
        Vector3D& operator= (const Vector3D& copy)  = default;

        Vector3D (Vector3D&& move);
        Vector3D& operator= (Vector3D&& move);

        ~Vector3D()                                 = default;

        #pragma endregion Constructors and destructor


        #pragma region Movement functionality

        /// <summary> Sets the position of the vector. </summary>
        void setPosition (const float newX = 0.f, const float newY = 0.f, const float newZ = 0.f);

        /// <summary> Translates the each component by the values given. </summary>
        void translate (const float x = 0.f, const float y = 0.f, const float z = 0.f);

        #pragma endregion Movement functionality

    public:

        #pragma region Member variables

        float x = 0.f;              //!< The x co-ordinate of the vector.
        float y = 0.f;              //!< The y co-ordinate of the vector.
        float z = 0.f;              //!< The z co-ordinate of the vector.

        #pragma endregion Member variables
};


#endif // VECTOR3_INCLUDED