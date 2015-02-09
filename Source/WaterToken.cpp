#include "WaterToken.h"


// STL headers.
#include <exception>
#include <iostream>


// Engine headers.
#include <Rendering/RendererHAPI.h>
#include <Audio/AudioSFML.h>
#include <Engine/Systems.h>


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
        auto renderer = std::make_shared<water::RendererHAPI>();
        auto audio = std::make_shared<water::AudioSFML>();
        
        water::Systems::setAudio (audio.get());
        water::Systems::setRenderer (renderer.get());

        water::Systems::getAudio().initialise (32, 1, 1);


        return false;

        /*try 
        {
            // Initialise HAPI.
            if (!HAPI->Initialise (&m_screenWidth, &m_screenHeight))
            {
                throw std::runtime_error ("Game::initialise(): HAPI couldn't initialise.");
            }

            // Set up the rendering engine.
            m_renderer = std::make_shared<water::RendererHAPI>();
            m_renderer->initialise (m_screenWidth, m_screenHeight, { 128, 128 });

            // Load the textures.
            water::TextureID ids[3] = {    m_renderer->loadTexture (backgroundLocation, { 0, 0 }),
                                    m_renderer->loadTexture (circleLocation, { 0, 0 }),
                                    m_renderer->loadTexture (explosionLocation, { 5, 5 }) };

            // Make the entities.
            auto background = std::make_unique<MilestoneEntity>(), circle = std::make_unique<MilestoneEntity>();

            background->getPosition() = { 0.f, 0.f };
            background->setTextureID (ids[0]);
            background->setBlendType (water::BlendType::Opaque);
            background->setFrameSize ({ 0, 0 });
            background->setFrame ({ 0, 0 });
        
            // Centre the circle.
            circle->getPosition() = { 3, 3 };

            circle->setTextureID (ids[1]);
            circle->setBlendType (water::BlendType::Transparent);
            circle->setFrameSize ({ 0, 0 });
            circle->setFrame ({ 0, 0 });
        
            m_entities.push_back (std::move (background));
            m_entities.push_back (std::move (circle));
        
        
            // Randomise explosions everywhere.
            util::RNG<float>        rngF    { util::getCurrentTime<unsigned int>() };
            util::RNG<unsigned int> rngU    { util::getCurrentTime<unsigned int>() };
                
            for (int i = 0; i < 10; ++i)
            {
                auto explosion = std::make_unique<MilestoneEntity>();

                explosion->getPosition() = { 8 * rngF() - 2.f, 8 * rngF() - 2.f };
                explosion->setTextureID (ids[2]);
                explosion->setBlendType (water::BlendType::Transparent);
                explosion->setFrameSize ({ 5, 5 });
                explosion->setFrame ({ rngU() % 5, rngU() % 5 });

                m_entities.push_back (std::move (explosion));
            }

            m_centreZone    = { 3, 3, 5, 5 };
            m_renderer->scaleTexture (ids[0], { 8.f, 8.f }, false);
            m_renderer->scaleTexture (ids[1], { 1.9847f, 4.8785465f }, false);
            m_renderer->scaleTexture (ids[2], { 1280.f, 1280.f }, true);

            m_audio = std::make_shared<water::AudioSFML>();
            m_audio->initialise (128, 5.f, 2.f);

            m_audio->loadMusic (musicLocation);
            m_sounds.push_back (m_audio->loadSound (soundLocation));
                            
            return true;
        }

        // Initialisation failed so just exit.
        catch (const std::exception& error)
        {
            std::cerr << "Exception caught in Game::initialise(): " << error.what() << std::endl;
        }

        catch (...)
        {
            std::cerr << "Unknown error caught in Game::initialise()." << std::endl;
        }

        return false;*/
    }


    void Game::run()
    {
        /*if (initialise())
        {
            const float sixtyFPS = static_cast<float> (1.0 / 60.0);

            HAPI->SetShowFPS (true);

            // Update each system.
            while (HAPI->Update())
            {
                updateDeltaTime();

                // Check whether to perform the capped update.
                if (m_sixtyFPSDeltaTime >= sixtyFPS)
                {
                    updateCapped();
                    m_sixtyFPSDeltaTime = 0.f;
                }
            
                // Perform the uncapped update.
                updateMain();
            
                renderAll();
            }
        }*/
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