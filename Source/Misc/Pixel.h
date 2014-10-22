#if !defined PIXEL_INCLUDED
#define PIXEL_INCLUDED


#include <HAPI_lib.h>


/// <summary>
/// A pixel which is used to display a point of a particular colour on the screen.
/// </summary>
struct Pixel final
{
    /// <summary> The default constructor of the Pixel struct. </summary>
    Pixel (const int pixelX = 0, const int pixelY = 0, const HAPI_TColour pixelColour = { }) 
        : x (pixelX), y (pixelY), colour (pixelColour) { }

    Pixel (const Pixel& copy)               = default;
    Pixel& operator= (const Pixel& copy)    = default;

    Pixel (Pixel&& move);
    Pixel& operator= (Pixel&& move);

    ~Pixel()                                = default;



    int             x       { 0 },  //!< The x co-ordinate of the pixel.
                    y       { 0 };  //!< The y co-ordinate of the pixel.

    HAPI_TColour    colour  { };    //!< The colour which the pixel should be displayed as.
};


#endif // PIXEL_INCLUDED