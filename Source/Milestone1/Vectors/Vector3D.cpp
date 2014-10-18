#include "Vector3D.h"

#include <memory>


#pragma region Constructors and destructor

Vector3D::Vector3D (Vector3D&& move)
{
    *this = std::move (move);
}


Vector3D& Vector3D::operator= (Vector3D&& move)
{
    if (this != &move)
    {
        x = move.x;
        y = move.y;
        z = move.z;

        move.x = 0.f;
        move.y = 0.f;
        move.z = 0.f;
    }

    return *this;
}

#pragma endregion Constructors and destructor


#pragma region Movement functionality

void Vector3D::setPosition (const float newX, const float newY, const float newZ)
{
    x = newX;
    y = newY;
    z = newZ;
}


void Vector3D::translate (const float xInc, const float yInc, const float zInc)
{
    x += xInc;
    y += yInc;
    z += zInc;
}

#pragma endregion Movement functionality