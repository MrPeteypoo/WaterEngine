#include "TimeSTL.hpp"


// STL headers.
#include <chrono>
#include <exception>


// Engine headers.
#include <Utility/Maths.hpp>


// Aliases.
using namespace std::chrono;


// Engine namespace.
namespace water
{
    #pragma region Implementation data

    struct TimeSTL::Impl final
    {
        real                                targetPhysics   { 0 },  //!< The target physics delta value.
                                            targetUpdate    { 0 },  //!< The target update delta value.
                                            maxDelta        { 0 },  //!< The maximum delta value for both physics and other systems.
                                            timescale       { 1 };  //!< The scale applied to time values, this can create slow motion in the game.

        real                                physicsDelta    { 0 },  //!< The physics delta accumulator.
                                            updateDelta     { 0 };  //!< The update delta accumulator.

        high_resolution_clock::time_point   startTime       { },    //!< The initial time point since the start of the application.
                                            previousPhysics { },    //!< The previous physics time point.
                                            previousUpdate  { };    //!< The previous update time point.
    };

    #pragma endregion


    #pragma region Constructors and destructor
            
    TimeSTL::TimeSTL()
    {
        m_impl = new Impl();
    }

    
    TimeSTL::TimeSTL (TimeSTL&& move)
    {
        *this = std::move (move);
    }


    TimeSTL& TimeSTL::operator= (TimeSTL&& move)
    {
        if (this != &move)
        {
            if (m_impl)
            {
                delete m_impl;
            }

            m_impl = move.m_impl;
            m_currentDelta = move.m_currentDelta;

            move.m_impl = nullptr;
            move.m_currentDelta = 0.f;
        }

        return *this;
    }

    TimeSTL::~TimeSTL()
    {
        if (m_impl)
        {
            delete m_impl;
            m_impl = nullptr;
        }
    }

    #pragma endregion


    #pragma region System management

    void TimeSTL::initialise (const int physicsFPS, const int updateFPS, const int minFPS)
    {
        // Pre-condition: Physics FPS is higher than 0.
        if (physicsFPS <= 0)
        {
            throw std::invalid_argument ("TimeSTL::initialise(), invalid physics FPS value given, must be higher than zero. Physics-" + std::to_string (physicsFPS) + ".");
        }

        // Pre-condition: Min FPS is the lowest FPS value. Allowing updateFPS to be 0.
        if (minFPS == 0 || minFPS > physicsFPS || (updateFPS != 0 && minFPS > updateFPS))
        {
            throw std::invalid_argument ("TimeSTL::initialise(), minFPS can not be zero and must be higher than physics or update (update can be zero).");
        }

        // Update the target values.
        const real one = 1;

        m_impl->targetPhysics = one / physicsFPS;
        m_impl->targetUpdate = updateFPS > 0 ? one / updateFPS : 0;
        m_impl->maxDelta = one / minFPS;

        // Obtain the beautiful current time point.
        const auto& now = high_resolution_clock::now();
        m_impl->startTime = now;
        m_impl->previousPhysics = now;
        m_impl->previousUpdate = now;
    }


    bool TimeSTL::physicsUpdate()
    {
        // Cache the implementation data.
        auto& impl = *m_impl;

        // Obtain the current point in time and use it to calculate the delta time.
        const auto& now = high_resolution_clock::now();
        const auto time = duration<real> (now - impl.previousPhysics).count();

        // Set the physics delta and checks if a physics update should be performed.
        impl.physicsDelta += time;
        impl.previousPhysics = now;
        setCurrentDelta (impl.targetPhysics);

        return impl.physicsDelta >= impl.targetPhysics;
    }


    bool TimeSTL::update()
    {
        // Cache the implementation data.
        auto& impl = *m_impl;

        // Obtain the current point in time and use it to calculate the delta time.
        const auto& now = high_resolution_clock::now();
        const auto time = duration<real> (now - impl.previousUpdate).count();

        // Set the delta and checks if an update should be performed.
        impl.updateDelta += time;
        impl.previousUpdate = now;

        // Clamp the update delta to the max delta value.
        if (impl.updateDelta > impl.maxDelta)
        {
            setCurrentDelta (impl.maxDelta);
            return true;
        }
        
        // Only perform an update loop if we're higher than the target time.
        else if (impl.targetUpdate > 0)
        {
            setCurrentDelta (impl.targetUpdate);

            return impl.updateDelta >= impl.targetUpdate;
        }

        // Don't cap the update loop.
        setCurrentDelta (impl.updateDelta);
        return true;        
    }


    void TimeSTL::endFrame()
    {
        // Cache the implementation data.
        auto& impl = *m_impl;

        // Decrement the physics delta.
        if (impl.physicsDelta >= impl.maxDelta)
        {
            impl.physicsDelta = 0;
        }
        
        else if (impl.physicsDelta >= impl.targetPhysics)
        {
            impl.physicsDelta -= impl.targetPhysics;
        }

        // Decrement the update delta.
        if (impl.updateDelta > impl.maxDelta)
        {
            impl.updateDelta = 0;
        }

        else if (impl.targetUpdate > 0)
        {
            impl.updateDelta -= impl.targetUpdate;
        }

        else
        {
            impl.updateDelta = 0;
        }
    }


    void TimeSTL::resetTime()
    {
        // Cache the implementation data.
        auto& impl = *m_impl;
        
        m_currentDelta = 0;
        impl.physicsDelta = 0;
        impl.updateDelta = 0;
        impl.previousPhysics = high_resolution_clock::now();
        impl.previousUpdate = high_resolution_clock::now();
    }


    void TimeSTL::setCurrentDelta (const real delta)
    {
        m_currentDelta = (float) (delta * m_impl->timescale);
    }

    #pragma endregion


    #pragma region Time management

    float TimeSTL::timeSinceStart() const
    {
        const auto& now = high_resolution_clock::now();
        const auto time = duration<float> (now - m_impl->startTime).count();

        return time;
    }


    float TimeSTL::timescale() const
    {
        return (float) m_impl->timescale;
    }


    void TimeSTL::setTimescale (const real timescale)
    {
        m_impl->timescale = util::max (timescale, (real) 0);
    }

    #pragma endregion
}