#include "Engine.hpp"


// STL headers.
#include <exception>
#include <iostream>


// Engine headers.
#include <Systems.hpp>

#include <Systems/Audio/AudioSFML.hpp>
#include <Systems/Logging/LoggerHAPI.hpp>
#include <Systems/Logging/LoggerSTL.hpp>
#include <Systems/Rendering/RendererHAPI.hpp>
#include <Systems/Time/TimeSTL.hpp>

#include <Configuration.hpp>


namespace water
{
    #pragma region Systems initial values

    IAudio*     Systems::m_audio    = nullptr;
    //IInput*     Systems::m_input    = nullptr;
    ILogger*    Systems::m_logger   = nullptr;
    IRenderer*  Systems::m_renderer = nullptr;
    ITime*      Systems::m_time     = nullptr;

    #pragma endregion


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

    bool Engine::initialise()
    {
        // Just create a default Configuration object.
        return initialise (Configuration());
    }


    bool Engine::initialise (const std::string& file)
    {
        // Obtain a configuration from the Configuration class and pass it to the standard initialsation process.
        return initialise (Configuration::fromXML (file));
    }


    bool Engine::initialise (const Configuration& config)
    {
        // Start by attempting to create each system.
        if (createSystems (config) && initialiseLogger (config))
        {
            // We have the guarantee that we are using the specified systems and we have a working logger. Now it's time to 
            // initialise the rest of the engine. Be careful of systems throwing exceptions during initialsation.
            try
            {
                initialiseSystems (config);
                setSystems();

                // Set the system as ready!
                return m_ready = true;
            }

            catch (const std::exception& error)
            {
                m_logger->logError ("Unable to load engine. " + std::string (error.what()));
            }

            catch (...)
            {
                m_logger->logError ("Unknown error occurred whilst initialising the engine. The computer may explode.");
            }
        }

        else
        {
            // We may have a working logger so we can output error messages.
            if (initialiseLogger (config))
            {
                m_logger->logError ("Engine::initialise(), system configuration is invalid. Ensure valid values are present.");
            }

            // Fall back to std::cerr in hopes that it will reach the user.
            else
            {
                std::cerr << "Error: Engine::initialise(), system configuration is invalid. Ensure valid values are present." << std::endl;
            }
        }

        return false;
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
                if (m_time->updatePhysics())
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


    bool Engine::createSystems (const Configuration& config)
    {
        // This WILL be messy, we need to check the string value and load the correct system. Start with the logger first.
        if (config.systems.logger == "stl" || config.systems.logger == "")
        {
            m_logger = new LoggerSTL();
        }

        else if (config.systems.logger == "hapi")
        {
            m_logger = new LoggerHAPI();
        }

        else { return false; }

        // Audio!
        if (config.systems.audio == "sfml" || config.systems.audio == "")
        {
            m_audio = new AudioSFML();
        }

        else { return false; }

        // Graphics!
        if (config.systems.renderer == "hapi" || config.systems.renderer == "")
        {
            m_renderer = new RendererHAPI();
        }

        else { return false; }

        // Time!
        if (config.systems.time == "stl" || config.systems.time == "")
        {
            m_time = new TimeSTL();
        }

        else { return false; }

        // We made it!
        return true;
    }


    bool Engine::initialiseLogger (const Configuration& config)
    {
        // Ensure we have a valid pointer.
        if (m_logger)
        {
            return m_logger->initialise (config.logging.file, config.logging.timestamp);
        }

        return false;
    }


    void Engine::initialiseSystems (const Configuration& config)
    {
        // We can assume all pointers from here on are valid.
        m_audio->initialise (config.audio.soundLimit, config.audio.bgmMixer, config.audio.sfxMixer);
        m_renderer->initialise (config.rendering.screenWidth, config.rendering.screenHeight, config.rendering.unitToPixel);
        m_time->initialise (config.time.physicsFPS, config.time.updateFPS, config.time.minFPS);
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