#include "WaterToken.hpp"


// STL headers.
#include <chrono>
#include <memory>


// Engine headers.
#include <Rendering/RendererHAPI.hpp>
#include <Audio/AudioSFML.hpp>
#include <Engine/Systems.hpp>
#include <Engine/Logging/LoggerHAPI.hpp>
#include <Engine/Logging/LoggerSTL.hpp>
#include <Engine/Time/TimeSTL.hpp>
#include <Misc/Vector2.hpp>


// Game namespace.
namespace wt
{
    // Milestone related constants.
    const auto  backgroundLocation  = "../Images/background.tga", //!< The file location for the background image.
                circleLocation      = "../Images/alphaThing.tga", //!< The file location for the circle image.
                explosionLocation   = "../Images/explosion.png",  //!< The file location for the explosion spritesheet.
                musicLocation       = "../Audio/music.flac",
                soundLocation       = "../Audio/sound.ogg";

    const auto  circleSpeed         = 3.f;              //!< The number of pixels a second the circle can travel.



    #pragma region Engine functionality

    bool Game::initialise()    
    {
        // Initialise the logger first.
        m_logger = std::make_shared<water::LoggerSTL>();
        
        if (m_logger->initialise ("log", true))
        {
            try
            {
                // Set the logger to be used by the other systems and the game.
                water::Systems::setLogger (m_logger.get());

                // Create the systems.
                m_audio = std::make_shared<water::AudioSFML>();
                m_renderer = std::make_shared<water::RendererHAPI>();
                m_time = std::make_shared<water::TimeSTL>();
        
                // Initialise the systems.
                HAPI->Initialise (&m_width, &m_height);        
        
                m_audio->initialise (2, 1, 1);
                m_renderer->initialise (m_width, m_height, { 64, 64 });
                m_time->initialise (60, 0, 10);

                // Set the systems for global access.
                water::Systems::setAudio (m_audio.get());
                water::Systems::setRenderer (m_renderer.get());
                water::Systems::setTime (m_time.get());

                return true;
            }

            catch (const std::exception& error)
            {
                water::Systems::logger().logError (error.what() + std::string ("\nThe application will now close."));
            }
        }

        return false;
    }


    void Game::run()
    {
        if (initialise())
        {
            try
            {
                // Show the FPS on screen and reset the time class.
                HAPI->SetShowFPS (true);

                m_time->resetTime();

                // Update each system.
                while (HAPI->Update())
                {
                    // Only perform a physics update if the time specifies so.
                    if (m_time->physicsUpdate())
                    {
                        physicsUpdate();
                    }

                    // Only perform an update if the time specifies so.
                    if (m_time->update())
                    {
                        update();
                    }

                    // Render the beautiful imagery all over the screen!
                    render();

                    // Ensure the time values are accurate for the next frame.
                    m_time->endFrame();
                }
            }

            catch (const std::exception& error)
            {
                m_logger->logError (error.what() + std::string ("Application will now close."));
            }
        }
    }


    void Game::physicsUpdate()
    {
        /*static auto previous = std::chrono::high_resolution_clock::now();
        const auto now = std::chrono::high_resolution_clock::now();

        const auto time = std::chrono::duration<double> (now - previous).count();
        previous = now;

        m_logger->logWarning ("Physics: " + std::to_string (time) + ", delta: " + std::to_string (m_time->getDelta()));*/
    }


    void Game::update()
    {
        /*static auto previous = std::chrono::high_resolution_clock::now();
        const auto now = std::chrono::high_resolution_clock::now();

        const auto time = std::chrono::duration<double> (now - previous).count();
        previous = now;

        m_logger->log ("Normal: " + std::to_string (time) + ", delta: " + std::to_string (m_time->getDelta()));*/
        /*// Update keyboard data.
        HAPI->GetKeyboardData (&m_keyboard);
    
        // Update controller data.
        m_controllerOn = HAPI->GetControllerData (0, &m_controller);

        if (m_controllerOn)
        {
            const auto& circlePosition = m_entities[1]->getPosition();
            const Rectangle<float> circleRect   { circlePosition.x, circlePosition.y, circlePosition.x + 2, circlePosition.y + 2 };

            if (circleRect.intersects (m_centreZone))
            {
                //HAPI->SetControllerRumble (0, 10000, 10000);
            }

            else
            {
                HAPI->SetControllerRumble (0, 0, 0);
            }
    
            if (m_controllerOn)
            {
                if (m_controller.digitalButtons[HK_DIGITAL_A])
                {
                    m_audio->playSound (m_sounds[0], 1.f, 0.f, false);
                }

                if (m_controller.digitalButtons[HK_DIGITAL_B])
                {
                    m_audio->pauseMusic();
                }

                if (m_controller.digitalButtons[HK_DIGITAL_Y])
                {
                    m_audio->resumeMusic();
                }

                if (m_controller.digitalButtons[HK_DIGITAL_X])
                {
                    m_audio->stopSound (0);
                }
            }
        }*/
    }


    /*void Game::updateMain()
    {
        auto& circlePosition = m_entities[1]->getPosition();

        // Handle keyboard input.
        if (m_keyboard.scanCode[HK_LEFT] || m_keyboard.scanCode['A'] || 
           (m_controllerOn && m_controller.analogueButtons[HK_ANALOGUE_LEFT_THUMB_X] < -HK_GAMEPAD_LEFT_THUMB_DEADZONE))
        {
            circlePosition.x -= circleSpeed * m_deltaTime;
        }

        if (m_keyboard.scanCode[HK_UP] || m_keyboard.scanCode['W'] || 
           (m_controllerOn && m_controller.analogueButtons[HK_ANALOGUE_LEFT_THUMB_Y] > HK_GAMEPAD_LEFT_THUMB_DEADZONE))
        {
            circlePosition.y -= circleSpeed * m_deltaTime;
        }

        if (m_keyboard.scanCode[HK_RIGHT] || m_keyboard.scanCode['D'] || 
           (m_controllerOn && m_controller.analogueButtons[HK_ANALOGUE_LEFT_THUMB_X] > HK_GAMEPAD_LEFT_THUMB_DEADZONE))
        {
            circlePosition.x += circleSpeed * m_deltaTime;
        }

        if (m_keyboard.scanCode[HK_DOWN] || m_keyboard.scanCode['S'] || 
           (m_controllerOn && m_controller.analogueButtons[HK_ANALOGUE_LEFT_THUMB_Y] < -HK_GAMEPAD_LEFT_THUMB_DEADZONE))
        {
            circlePosition.y += circleSpeed * m_deltaTime;
        }

        for (auto& entity : m_entities)
        {
            if (entity)
            {
                entity->update (m_deltaTime);
            }
        }
    }*/


    void Game::render()
    {    
        /*// Render images.
        m_renderer->clearToBlack();

        for (auto& entity : m_entities)
        {
            if (entity)
            {
                entity->render (m_renderer);
            }
        }*/
    }

    #pragma endregion Engine functionality
}