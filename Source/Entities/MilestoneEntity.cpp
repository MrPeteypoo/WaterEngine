#include <Entities/MilestoneEntity.h>


// STL headers.
#include <memory>



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

        const float fifteenFPS = 1.f / 15.f;

        if (m_time >= fifteenFPS)
        {
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


void MilestoneEntity::render (const std::shared_ptr<IRenderer2D>& renderer)
{
    if (renderer)
    {
        renderer->drawTexture (static_cast<Vector2D<int>> (m_position), m_texture, m_blend, m_frame);
    }

    else
    {
        throw std::invalid_argument ("MilestoneEntity::render(): Invalid pointer to the renderering engine given.");
    }
}