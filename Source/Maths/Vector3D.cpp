#include "Vector3D.h"


// STL headers.
#include <complex>


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

bool Vector3D::operator== (const Vector3D& rhs) const
{
    return (    x == rhs.x && 
                y == rhs.y && 
                z == rhs.z      );
}


bool Vector3D::operator!= (const Vector3D& rhs) const
{
    return (    x != rhs.x ||
                y != rhs.y || 
                z != rhs.z      );
}


Vector3D Vector3D::operator+ (const Vector3D& rhs) const
{
    return {    x + rhs.x,
                y + rhs.y,
                z + rhs.z   };
}


Vector3D Vector3D::operator- (const Vector3D& rhs) const
{
    return {    x - rhs.x,
                y - rhs.y,
                z - rhs.z   };
}


Vector3D Vector3D::operator* (const float rhs) const
{
    return {    x * rhs,
                y * rhs,
                z * rhs };
}


Vector3D Vector3D::operator/ (const float rhs) const
{
    return {    x / rhs,
                y / rhs,
                z / rhs };
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


void Vector3D::translate (const float moveX, const float moveY, const float moveZ)
{
    x += moveX;
    y += moveY;
    z += moveZ;
}

#pragma endregion Movement functionality
        

#pragma region Maths functionality

float Vector3D::squareMagnitude() const
{
    return (x * x + y * y + z * z);
}


float Vector3D::magnitude() const
{
    return std::sqrt (squareMagnitude());
}


Vector3D Vector3D::normalised() const
{
    return *this / magnitude();
}


void Vector3D::normalise()
{
    *this = normalised();
}

#pragma endregion Maths functionality


#pragma region Helper functions


float dotProduct (const Vector3D& lhs, const Vector3D& rhs)
{
    // Calculate each component.
    const float x   { lhs.x * rhs.x },
                y   { lhs.y * rhs.y },
                z   { lhs.z * rhs.z };
    
    // Return the calculated product.
    return ( x + y - z );
}


Vector3D crossProduct (const Vector3D& lhs, const Vector3D& rhs)
{
    // Calculate the determinants.
    const float x   {  (lhs.y * rhs.z - lhs.z * rhs.y) },
                y   { -(lhs.x * rhs.z - lhs.z * rhs.x) },
                z   {  (lhs.x * rhs.y - lhs.y * rhs.x) };

    return { x, y, z };
}


#pragma endregion