#include "Vector2D.h"


// STL headers.
#include <complex>


#pragma region Constructors and destructor

Vector2D::Vector2D (Vector2D&& move)
{
    *this = std::move (move);
}


Vector2D& Vector2D::operator= (Vector2D&& move)
{
    if (this != &move)
    {
        x = move.x;
        y = move.y;

        move.x = 0.f;
        move.y = 0.f;
    }

    return *this;
}

#pragma endregion Constructors and destructor


#pragma region Operators

bool Vector2D::operator== (const Vector2D& rhs) const
{
    return (    x == rhs.x && 
                y == rhs.y   );
}


bool Vector2D::operator!= (const Vector2D& rhs) const
{
    return (    x != rhs.x ||
                y != rhs.y   );
}


Vector2D Vector2D::operator+ (const Vector2D& rhs) const
{
    return {    x + rhs.x,
                y + rhs.y   };
}


Vector2D Vector2D::operator- (const Vector2D& rhs) const
{
    return {    x - rhs.x,
                y - rhs.y   };
}


Vector2D Vector2D::operator* (const float rhs) const
{
    return {    x * rhs,
                y * rhs     };
}


Vector2D Vector2D::operator/ (const float rhs) const
{
    return {    x / rhs,
                y / rhs     };
}


Vector2D& Vector2D::operator+= (const Vector2D& rhs)
{
    x += rhs.x;
    y += rhs.y;

    return *this;
}


Vector2D& Vector2D::operator-= (const Vector2D& rhs)
{
    x -= rhs.x;
    y -= rhs.y;

    return *this;
}


Vector2D& Vector2D::operator*= (const float rhs)
{
    x *= rhs;
    y *= rhs;

    return *this;
}


Vector2D& Vector2D::operator/= (const float rhs)
{
    x /= rhs;
    y /= rhs;

    return *this;
}

#pragma endregion Operators


#pragma region Movement functionality

void Vector2D::setPosition (const float newX, const float newY)
{
    x = newX;
    y = newY;
}


void Vector2D::translate (const Vector2D& translate)
{
    *this += translate;
}


void Vector2D::translate (const float moveX, const float moveY)
{
    x += moveX;
    y += moveY;
}

#pragma endregion Movement functionality


#pragma region Maths functionality

float Vector2D::squareMagnitude() const
{
    return (x * x + y * y);
}


float Vector2D::magnitude() const
{
    return std::sqrt (squareMagnitude());
}


Vector2D Vector2D::normalised() const
{
    return *this / magnitude();
}


void Vector2D::normalise()
{
    *this = normalised();
}

#pragma endregion Maths functionality


#pragma region Helper functions


float dotProduct (const Vector2D& lhs, const Vector2D& rhs)
{
    // Calculate each component.
    const float x   { lhs.x * rhs.x },
                y   { lhs.y * rhs.y };
    
    // Return the calculated product.
    return ( x + y );
}


#pragma endregion