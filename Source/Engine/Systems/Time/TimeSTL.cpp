#include "TimeSTL.hpp"


// STL headers.
#include <exception>


// Engine headers.
#include <Utility/Maths.hpp>


// Engine namespace.
namespace water
{
    #pragma region Constructors and destructor
    
    TimeSTL::TimeSTL (TimeSTL&& move)
    {
        *this = std::move (move);
    }


    TimeSTL& TimeSTL::operator= (TimeSTL&& move)
    {
        if (this != &move)
        {
            m_targetPhysics     = move.m_targetPhysics;
            m_targetUpdate      = move.m_targetUpdate;
            m_maxDelta          = move.m_maxDelta;
            m_timescale         = move.m_timescale;

            m_physicsDelta      = move.m_physicsDelta;
            m_updateDelta       = move.m_updateDelta;
            m_currentDelta      = move.m_currentDelta;
            m_physicsStep       = move.m_physicsStep;

            m_startTime         = std::move (move.m_startTime);
            m_previousPhysics   = std::move (move.m_previousPhysics);
            m_previousUpdate    = std::move (move.m_previousUpdate);

            // Reset primitives.
            move.m_targetPhysics    = 0;
            move.m_targetUpdate     = 0;
            move.m_maxDelta         = 0;
            move.m_timescale        = 0;
            
            move.m_physicsDelta     = 0;
            move.m_updateDelta      = 0;
            move.m_currentDelta     = 0;
            move.m_physicsStep      = 0;
        }

        return *this;
    }

    #pragma endregion


    #pragma region System management

    void TimeSTL::initialise (const unsigned int physicsFPS, const unsigned int updateFPS, const unsigned int minFPS)
    {
        // Pre-condition: Physics FPS is higher than 0.
        if (physicsFPS == 0)
        {
            throw std::invalid_argument ("TimeSTL::initialise(), physics FPS value must be higher than zero.");
        }

        // Pre-condition: Min FPS is the lowest FPS value. Allowing updateFPS to be 0.
        if (minFPS == 0 || minFPS > physicsFPS || (updateFPS != 0 && minFPS > updateFPS))
        {
            throw std::invalid_argument ("TimeSTL::initialise(), minFPS can not be zero and must be higher than physics and update (update can be zero).");
        }

        // Update the target values.
        const real one = 1;

        m_targetPhysics = one / physicsFPS;
        m_targetUpdate = updateFPS > 0 ? one / updateFPS : 0;
        m_maxDelta = one / minFPS;

        // Obtain the beautiful current time point.
        const auto& now = high_resolution_clock::now();
        m_startTime = now;
        m_previousPhysics = now;
        m_previousUpdate = now;
    }


    bool TimeSTL::updatePhysics()
    {
        // Obtain the current point in time and use it to calculate the delta time.
        const auto& now = high_resolution_clock::now();
        const auto time = duration<real> (now - m_previousPhysics).count();

        // Set the physics delta and checks if a physics update should be performed.
        m_physicsDelta += time;
        m_previousPhysics = now;
        m_physicsStep = (float) util::max (m_physicsDelta / m_targetPhysics, 1.0);
        setCurrentDelta (m_targetPhysics);

        return m_physicsDelta >= m_targetPhysics;
    }


    bool TimeSTL::update()
    {

        // Obtain the current point in time and use it to calculate the delta time.
        const auto& now = high_resolution_clock::now();
        const auto time = duration<real> (now - m_previousUpdate).count();

        // Set the delta and checks if an update should be performed.
        m_updateDelta += time;
        m_previousUpdate = now;

        // Clamp the update delta to the max delta value.
        if (m_updateDelta > m_maxDelta)
        {
            setCurrentDelta (m_maxDelta);
            return true;
        }
        
        // Only perform an update loop if we're higher than the target time.
        else if (m_targetUpdate > 0)
        {
            setCurrentDelta (m_targetUpdate);

            return m_updateDelta >= m_targetUpdate;
        }

        // Don't cap the update loop.
        setCurrentDelta (m_updateDelta);
        return true;        
    }


    void TimeSTL::endFrame()
    {
        // Decrement the physics delta. 
        if (m_physicsDelta > m_maxDelta)
        {
            m_physicsDelta = 0;
        }
        
        else if (m_physicsDelta >= m_targetPhysics)
        {
            m_physicsDelta -= m_targetPhysics;
        }

        // Decrement the update delta.
        if (m_updateDelta > m_maxDelta)
        {
            m_updateDelta = 0;
        }

        else if (m_targetUpdate > 0)
        {
            if (m_updateDelta > m_targetUpdate)
            {
                m_updateDelta -= m_targetUpdate;
            }
        }

        else
        {
            m_updateDelta = 0;
        }
    }


    void TimeSTL::resetTime()
    {        
        m_currentDelta = 0;
        m_physicsDelta = 0;
        m_updateDelta = 0;
        m_previousPhysics = high_resolution_clock::now();
        m_previousUpdate = high_resolution_clock::now();
    }


    void TimeSTL::setCurrentDelta (const real delta)
    {
        m_currentDelta = (float) (delta * m_timescale);
    }

    #pragma endregion


    #pragma region Time management

    float TimeSTL::timeSinceStart() const
    {
        const auto& now = high_resolution_clock::now();
        const auto time = duration<float> (now - m_startTime).count();

        return time;
    }


    void TimeSTL::setTimescale (const real timescale)
    {
        m_timescale = util::max (timescale, (real) 0);
    }

    #pragma endregion
}