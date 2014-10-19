#include "Vector3D.h"

#include <complex>
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


#pragma region Operators

const bool Vector3D::operator== (const Vector3D& rhs) const
{
    return (    x == rhs.x && 
                y == rhs.y && 
                z == rhs.z      );
}


const bool Vector3D::operator!= (const Vector3D& rhs) const
{
    return (    x != rhs.x ||
                y != rhs.y || 
                z != rhs.z      );
}


const Vector3D Vector3D::operator+ (const Vector3D& rhs) const
{
    return Vector3D (   x + rhs.x,
                        y + rhs.y,
                        z + rhs.z   );
}


const Vector3D Vector3D::operator- (const Vector3D& rhs) const
{
    return Vector3D (   x - rhs.x,
                        y - rhs.y,
                        z - rhs.z   );
}


const Vector3D Vector3D::operator+ (const float rhs) const
{
    return Vector3D (   x + rhs,
                        y + rhs,
                        z + rhs     );
}


const Vector3D Vector3D::operator- (const float rhs) const
{
    return Vector3D (   x - rhs,
                        y - rhs,
                        z - rhs     );
}


const Vector3D Vector3D::operator* (const float rhs) const
{
    return Vector3D (   x * rhs,
                        y * rhs,
                        z * rhs     );
}


const Vector3D Vector3D::operator/ (const float rhs) const
{
    return Vector3D (   x / rhs,
                        y / rhs,
                        z / rhs     );
}


Vector3D& Vector3D::operator+= (const Vector3D& rhs)
{
    x += rhs.x;
    y += rhs.y;
    z += rhs.z;

    return *this;
}


Vector3D& Vector3D::operator-= (const Vector3D& rhs)
{
    x -= rhs.x;
    y -= rhs.y;
    z -= rhs.z;

    return *this;
}


Vector3D& Vector3D::operator+= (const float rhs)
{
    x += rhs;
    y += rhs;
    z += rhs;

    return *this;
}


Vector3D& Vector3D::operator-= (const float rhs)
{
    x -= rhs;
    y -= rhs;
    z -= rhs;

    return *this;
}


Vector3D& Vector3D::operator*= (const float rhs)
{
    x *= rhs;
    y *= rhs;
    z *= rhs;

    return *this;
}


Vector3D& Vector3D::operator/= (const float rhs)
{
    x /= rhs;
    y /= rhs;
    z /= rhs;

    return *this;
}

#pragma endregion Operators


#pragma region Movement functionality

void Vector3D::setPosition (const float newX, const float newY, const float newZ)
{
    x = newX;
    y = newY;
    z = newZ;
}


void Vector3D::translate (const Vector3D& translate)
{
    *this += translate;
}


void Vector3D::translate (const float xInc, const float yInc, const float zInc)
{
    x += xInc;
    y += yInc;
    z += zInc;
}

#pragma endregion Movement functionality
        

#pragma region Maths functionality

const float Vector3D::squareMagnitude() const
{
    return (x * x + y * y + z * z);
}


const float Vector3D::magnitude() const
{
    return std::sqrtf (squareMagnitude());
}


const Vector3D Vector3D::normalised() const
{
    return *this / magnitude();
}


void Vector3D::normalise()
{
    *this = normalised();
}

#pragma endregion Maths functionality