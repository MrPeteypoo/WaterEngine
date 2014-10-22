#include "Star.h"


#pragma region Constructors and destructor


Star::Star (Star&& move)
{
    *this = std::move (move);
}


Star& Star::operator= (Star&& move)
{
    if (this != &move)
    {
        m_position = move.m_position;
        m_direction = move.m_direction;
        m_speed = move.m_speed;
        m_colour = move.m_colour;

        move.m_position = { };
        move.m_direction = { };
        move.m_speed = 0.f;
        move.m_colour = { };
    }

    return *this;
}


#pragma endregion Constructors and destructor


#pragma region Movement functionality


const Pixel Star::getDisplayData (const int screenWidth, const int screenHeight, const float eyeDistance)
{
    // We need to convert from 3D to 2D space taking into account.
    // Start by calculating the centre of the screen.
    const int   centreX = screenWidth / 2,
                centreY = screenHeight / 2;

    // Pxyz = position, Cxy = Centre X/Y.
    // Formula: sX = ((eyeDist * (Px - Cx)) / (eyeDist + Pz)) + Cx.
    const float division = eyeDistance + m_position.z;

    const float sX = division == 0.f ? 
                                    (eyeDistance * (m_position.x - centreX)) / 0.001f + centreX :
                                    (eyeDistance * (m_position.x - centreX)) / division + centreX;

    // Formula: sY = ((eyeDist * (Py - Cy)) / (eyeDist + Pz)) + Cy.
    const float sY = division == 0.f ? 
                                    (eyeDistance * (m_position.y - centreY)) / 0.001f + centreY :
                                    (eyeDistance * (m_position.y - centreY)) / division + centreY;

    // Finally construct the pixel.
    return Pixel ((int) sX, (int) sY, m_colour);
}

void Star::update()
{
    m_position += m_direction * m_speed;
}

#pragma endregion Movement functionality