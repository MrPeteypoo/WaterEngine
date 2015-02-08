#include "MilestoneEntity.h"


// STL headers.
#include <memory>


const float fifteenFPS = static_cast<float> (1.0 / 15.0);



bool MilestoneEntity::initialise()
{
    return true;
}


void MilestoneEntity::update (const float deltaTime)
{
    // Basic sprite sheet cycling.
    if (m_frameSize.x != 0 && m_frameSize.y != 0)
    {
        m_time += deltaTime;

        if (m_time >= fifteenFPS)
        {
            m_time -= fifteenFPS;

            if (++m_frame.x >= m_frameSize.x)
            {
                m_frame.x = 0;

                if (++m_frame.y >= m_frameSize.y)
                {
                    m_frame.y = 0;
                }
            }
        }
    }
}


void MilestoneEntity::render (const std::shared_ptr<IRenderer>& renderer)
{
    if (renderer)
    {
        renderer->drawToScreen (m_position, m_texture, m_blend, m_frame);
    }

    else
    {
        throw std::invalid_argument ("MilestoneEntity::render(): Invalid pointer to the renderering engine given.");
    }
}