#if !defined STAR_INCLUDED
#define STAR_INCLUDED


#include <Misc/Pixel.h>
#include <Vectors/Vector3D.h>


/// <summary>
/// A star with 3D perspective to be displayed in a 2D game engine.
/// </summary>
class Star final
{
    public:
        
        #pragma region Constructors and destructor

        /// <summary> The default constructor, allows the initial values of each component to be set. </summary>
        Star (const Vector3D& position = { }, const HAPI_TColour& colour = { }, const Vector3D& direction = { 0.f, 0.f, 1.f }, const float speed = 0.f)
            :   m_position (position), m_colour (colour), m_direction (direction), m_speed (speed) { }

        Star (const Star& copy)             = default;
        Star& operator= (const Star& copy)  = default;

        Star (Star&& move);
        Star& operator= (Star&& move);

        ~Star()                             = default;

        #pragma endregion Constructors and destructor
        

        #pragma region Movement functionality

        /// <summary> Returns the position of the star in Vector3D format. </summary>
        const Vector3D& getPosition() const         { return m_position; }

        /// <summary> Returns the Pixel data required for the star to be displayed on the screen. </summary>
        const Pixel getDisplayData (const int screenWidth, const int screenHeight, const float eyeDistance = 100.f);

        /// <summary> Sets the colour of the star. </summary>
        void setColour (const HAPI_TColour& colour) { m_colour = colour; }

        /// <summary> Sets the position of the star. </summary>
        void setPosition (const Vector3D& position) { m_position = position; }

        /// <summary> Sets the direction which the Star should move in. </summary>
        void setDirection (const Vector3D& dir)     { m_direction = dir; }

        /// <summary> Sets the speed of the stars movement. </summary>
        void setSpeed (const float speed)           { m_speed = speed; }

        /// <summary> Moves the star. </summary>
        void update();

        #pragma endregion Movement functionality


    private:

        #pragma region Member variables

        Vector3D        m_position  { };                //!< The position of the star.
        Vector3D        m_direction { 0.f, 0.f, 1.f };  //!< The direction which the star should travel.
        
        float           m_speed     { 0.f };            //!< The speed at which the star moves.

        HAPI_TColour    m_colour    { };                //!< The point at which the star should be displayed on the screen.

        #pragma endregion Member variables
    
};


#endif // STAR_INCLUDED