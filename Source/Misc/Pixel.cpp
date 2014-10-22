#include "Pixel.h"


Pixel::Pixel (Pixel&& move)
{
    *this = std::move (move);
}


Pixel& Pixel::operator= (Pixel&& move)
{
    if (this != &move)
    {
        x = move.x;
        y = move.y;
        colour = move.colour;

        move.x = 0;
        move.y = 0;
        move.colour = HAPI_TColour();
    }

    return *this;
}