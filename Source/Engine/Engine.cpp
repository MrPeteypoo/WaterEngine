#include "Engine.hpp"


// STL headers.
#include <exception>


// Engine headers.
#include <Systems.hpp>

#include <Audio/IAudioEngine.hpp>
#include <Logging/ILoggerEngine.hpp>
#include <Rendering/IRendererEngine.hpp>
#include <Time/ITimeEngine.hpp>

#include <Construction/EngineBuilder.hpp>


namespace water
{
    #pragma region Constructors and destructor

    Engine::Engine()
    {
        //m_states = new StateManager();
    }


    Engine::Engine (Engine&& move)
    {
        *this = std::move (move);
    }


    Engine& Engine::operator= (Engine&& move)
    {
        if (this != &move)
        {
            // Start by cleaning the current engine.
            clean();

            // Obtain ownership.
            m_audio = move.m_audio;
            m_logger = move.m_logger;
            m_renderer = move.m_renderer;
            m_time = move.m_time;
            m_states = move.m_states;
            m_ready = move.m_ready;
            m_close = move.m_close;

            // Reset the dangling pointers.
            move.m_audio = nullptr;
            move.m_logger = nullptr;
            move.m_renderer = nullptr;
            move.m_time = nullptr;
            move.m_states = nullptr;
            move.m_ready = false;
            move.m_close = false;

            // Reset the systems.
            setSystems();
        }
        
        return *this;
    }


    Engine::~Engine()
    {
        clean();
    }

    #pragma endregion


    #pragma region Engine management

    bool Engine::initialiseFromFile (const std::string& file)
    {
        // Request the EngineBuilder initialise the engine from the given configuration file.
        EngineBuilder::build (*this, file);

        return m_ready;
    }


    void Engine::run()
    {
        // Pre-condition: The engine is ready.
        if (!m_ready)
        {
            throw std::runtime_error ("Engine::run(), attempt to run the engine without successful initialisation.");
        }

        try
        {
            // Reset the time as we're ready to start the game loop.
            m_time->resetTime();

            // If the renderer fails we must close.
            while (!m_close && m_renderer->update())
            {
                // Only perform a physics update if the time specifies so.
                if (m_time->physicsUpdate())
                {
                    // TODO: Implement me bruv!
                }

                // Only perform an update if the time specifies so.
                if (m_time->update())
                {
                   // TODO: Implement me bruv!
                }

                // Render the beautiful imagery all over the screen!
                // TODO: Implement me bruv!

                // Ensure the time values are accurate for the next frame.
                m_time->endFrame();
            }
        }

        catch (const std::exception& error)
        {
            m_logger->logError (error.what() + std::string ("Application will now close."));
        }

        catch (...)
        {
            m_logger->logError ("Engine::run(), an unexpected error occurred.");
        }
    }

    #pragma endregion


    #pragma region Internal workings

    void Engine::clean()
    {
        // We need to test each pointer because not all compilers will ignore the deletion of a nullptr.
        if (m_audio)    { delete m_audio; }
        if (m_logger)   { delete m_logger; }
        if (m_renderer) { delete m_renderer; }
        if (m_time)     { delete m_time; }
        if (m_states)   { delete m_states; }
    }


    void Engine::setSystems()
    {
        // Set each system in the Systems class so that every game object gains access.
        Systems::setAudio (m_audio);
        Systems::setLogger (m_logger);
        Systems::setRenderer (m_renderer);
        Systems::setTime (m_time);
    }

    #pragma endregion
}