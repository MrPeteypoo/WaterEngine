#if !defined WATER_TIME_STL_INCLUDED
#define WATER_TIME_STL_INCLUDED


// Engine headers.
#include <Systems/IEngineTime.hpp>


// Engine namespace.
namespace water
{
    /// <summary>
    /// A time keeping engine which uses the chrono library to track time.
    /// </summary>
    class TimeSTL final : public ITimeEngine
    {
        public:

            #pragma region Constructors and destructor
            
            TimeSTL();
            TimeSTL (TimeSTL&& move);
            TimeSTL& operator= (TimeSTL&& move);

            ~TimeSTL() override final;
        
            TimeSTL (const TimeSTL& copy)               = delete;
            TimeSTL& operator= (const TimeSTL& copy)    = delete;

            #pragma endregion


            #pragma region System management
            
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

            #pragma endregion


            #pragma region Time management

            /// <summary> Get the delta time value of the current update loop in seconds. </summary>
            /// <returns> The physics update time during updatePhysics() or the update time during update(). </returns>
            float getDelta() const override final { return m_currentDelta; }

            /// <summary> Obtains the time in seconds since the game start. </summary>
            float timeSinceStart() const override final;

            /// <summary> Obtains the time scale currently being applied each frame. </summary>
            float timescale() const override final;

            /// <summary> This sets the time scale applied to the real world frame times. This will not go below zero. </summary>
            void setTimescale (const real timescale) override final;

            #pragma endregion

        private:

            /// <summary> Sets the current delta value, applying the timescale value. </summary>
            void setCurrentDelta (const real delta);

            // Forward declarations.
            struct Impl;

            Impl* m_impl            { nullptr };    //!< A pointer to the implementation data.
            float m_currentDelta    { 0.f };        //!< Keep the current delta value outside the pimpl for efficiency.
    };   
}

#endif