#if !defined STAR_INCLUDED
#define STAR_INCLUDED


#include "../Vectors/Vector3D.h"


/// <summary>
/// A star with 3D perspective to be displayed in a 2D game engine.
/// </summary>
class Star final
{
    public:
        
        #pragma region Constructors and destructor

        /// <summary> The default constructor, allows the initial values of each component to be set. </summary>
        Star (const Vector3D& position = Vector3D()) {};

        Star (const Star& copy)             = default;
        Star & operator= (const Star& copy) = default;

        Star (Star&& move);
        Star& operator= (Star&& move);

        ~Star()                             = default;

        #pragma endregion Constructors and destructor
        
        /// <summary> Updates the star
        void update();

    private:
        
};


#endif // STAR_INCLUDED