#if !defined WATER_TIME_STL_INCLUDED
#define WATER_TIME_STL_INCLUDED


// STL headers.
#include <chrono>


// Engine headers.
#include <Systems/IEngineTime.hpp>


// Engine namespace.
namespace water
{
    /// <summary>
    /// A time keeping engine which uses the chrono library to track time.
    /// </summary>
    class TimeSTL final : public IEngineTime
    {
        public:

            /////////////////////////////////
            // Constructors and destructor //
            /////////////////////////////////

            TimeSTL()                                   = default;
            TimeSTL (TimeSTL&& move);
            TimeSTL& operator= (TimeSTL&& move);

            ~TimeSTL() override final { }

            TimeSTL (const TimeSTL& copy)               = delete;
            TimeSTL& operator= (const TimeSTL& copy)    = delete;


            ///////////////////////
            // System management //
            ///////////////////////

            /// <summary> Initialise the time system, preparing it for usage. </summary>
            /// <param name="physicsFPS"> The FPS which physics updates should be capped at. </param>
            /// <param name="updateFPS"> The FPS other systems should update at. 0 means unlimited. </param>
            /// <param name="minFPS"> The minimum FPS before slowdown occurs. This effectively caps the max tick rate of the engine. </param>
            /// <returns> Whether the initialisation was successful. </returns>
            void initialise (const unsigned int physicsFPS, const unsigned int updateFPS, const unsigned int minFPS) override final;

            /// <summary> Causes physics update to become the active context and updates the physics delta time. </summary>
            bool updatePhysics() override final;

            /// <summary> Causes update to become the active context and updates the standard delta time. </summary>
            bool update() override final;

            /// <summary> Causes the time system to perform end-of-frame actions such as decrementing accumulators. </summary>
            void endFrame() override final;

            /// <summary> Force the time class to reset the time values, this does not reset the start time of the application. </summary>
            void resetTime() override final;


            /////////////////////
            // Time management //
            /////////////////////

            /// <summary> Get the delta time value of the current update loop in seconds. </summary>
            /// <returns> The physics update time during updatePhysics() or the update time during update(). </returns>
            float getDelta() const override final           { return m_currentDelta; }

            /// <summary> Obtains a normalised value of the current point in time between the previous physics update and the next physics update. </summary>
            float getPhysicsStep() const override final     { return m_physicsStep; }

            /// <summary> Obtains the time in seconds since the game start. </summary>
            float timeSinceStart() const override final;

            /// <summary> Obtains the time scale currently being applied each frame. </summary>
            float timescale() const override final          { return static_cast<float> (m_timescale); }

            /// <summary> This sets the time scale applied to the real world frame times. This will not go below zero. </summary>
            void setTimescale (const real timescale) override final;

        private:

            /// <summary> Sets the current delta value, applying the timescale value. </summary>
            void setCurrentDelta (const real delta);


            /////////////////////////
            // Implementation data //
            /////////////////////////

            using high_resolution_clock = std::chrono::high_resolution_clock;
            using time_point = high_resolution_clock::time_point;

            real        m_targetPhysics     { 0 },  //!< The target physics delta value.
                        m_targetUpdate      { 0 },  //!< The target update delta value.
                        m_maxDelta          { 0 },  //!< The maximum delta value for both physics and other systems.
                        m_timescale         { 1 };  //!< The scale applied to time values, this can create slow motion in the game.

            real        m_physicsDelta      { 0 },  //!< The physics delta accumulator.
                        m_updateDelta       { 0 };  //!< The update delta accumulator.
            float       m_currentDelta      { 0 },  //!< The current delta time value.
                        m_physicsStep       { 0 };  //!< The step value for the current point between the previous physics update and the next.

            time_point  m_startTime         { },    //!< The initial time point since the start of the application.
                        m_previousPhysics   { },    //!< The previous physics time point.
                        m_previousUpdate    { };    //!< The previous update time point.
    };
}

#endif
