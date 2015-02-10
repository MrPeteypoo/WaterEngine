#include "WaterToken.hpp"


// STL headers.
#include <memory>


// Engine headers.
#include <Rendering/RendererHAPI.hpp>
#include <Audio/AudioSFML.hpp>
#include <Engine/Systems.hpp>
#include <Engine/Logging/LoggerHAPI.hpp>
#include <Engine/Logging/LoggerSTL.hpp>
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
        auto logger = std::make_shared<water::LoggerSTL>();
        auto loggerHAPI = std::make_shared<water::LoggerHAPI>();
        logger->initialise ("log", true);
        loggerHAPI->initialise ("", true);
        water::Systems::setLogger (logger.get());


        try
        {
        
            auto renderer = std::make_shared<water::RendererHAPI>();
            auto audio = std::make_shared<water::AudioSFML>();
        
            water::Systems::setAudio (audio.get());
            water::Systems::setRenderer (renderer.get());

            int m_width = 256, m_height = 256;
            HAPI->Initialise(&m_width, &m_height);
        
        
            audio->initialise (2, 1, 1);
            renderer->initialise (0, m_height, { 64, 64 });

            return true;
        }

        catch (const std::exception& error)
        {
            water::Systems::logger().logError (error.what() + std::string ("\nThe application will now close."));
        }

        return false;
    }


    void Game::run()
    {
        if (initialise())
        {
            //const float sixtyFPS = static_cast<float> (1.0 / 60.0);

            HAPI->SetShowFPS (true);

            // Update each system.
            while (HAPI->Update())
            {
                //updateDeltaTime();

                // Check whether to perform the capped update.
                /*if (m_sixtyFPSDeltaTime >= sixtyFPS)
                {
                    updateCapped();
                    m_sixtyFPSDeltaTime = 0.f;
                }
            
                // Perform the uncapped update.
                updateMain();
            
                renderAll();*/
            }
        }
    }


    void Game::updateDeltaTime()
    {
        /*// Update the time figures.
        m_oldTime = m_currentTime;
        m_currentTime = HAPI->GetTime();

        // Convert from milliseconds to seconds.
        m_deltaTime = (m_currentTime - m_oldTime) / 1000.f;

        m_sixtyFPSDeltaTime += m_deltaTime;*/
    }


    void Game::updateCapped()
    {
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


    void Game::updateMain()
    {
        /*auto& circlePosition = m_entities[1]->getPosition();

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
        }*/
    }


    void Game::renderAll()
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