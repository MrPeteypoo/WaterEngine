#include "Game.h"


// Engine headers.
#include <Maths/Utility.h>
#include <Rendering/ScreenManager.h>


// Milestone related constants.
const std::string   backgroundLocation  = "background.tga",
                    circleLocation      = "alphaThing.tga";

const float         circleSpeed = 150.f; //!< The number of pixels a second the circle can travel.



#pragma region Engine functionality

bool Game::initialise()
{
    try 
    {
        // Initialise HAPI.
        if (!HAPI->Initialise (&m_screenWidth, &m_screenHeight))
        {
            throw std::runtime_error ("Game::initialise(): HAPI couldn't initialise.");
        }

        // Set up the screen manager.
        m_pScreenManager = std::make_shared<ScreenManager> (ScreenManager (m_screenWidth, m_screenHeight));
        m_pScreenManager->clearToBlackLevel();

        // Load textures.
        if (!m_background.loadTexture (backgroundLocation) || !m_circle.loadTexture (circleLocation))
        {
            HAPI->UserMessage ("Unable to load textures.", "Error");
            return false;
        }

        // Centre the circle.
        m_circlePosition = {    m_screenWidth / 2.f - m_circle.getWidth() / 2.f,
                                m_screenHeight / 2.f - m_circle.getHeight() / 2.f };

        return true;
    }

    // Initialisation failed so just exit.
    catch (...)
    {
        return false;
    }
}


void Game::run()
{
    if (initialise())
    {
        const float sixtyFPS = (float) (1.0 / 60.0);

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
    }
}


void Game::updateDeltaTime()
{
    // Update the time figures.
    m_oldTime = m_currentTime;
    m_currentTime = HAPI->GetTime();

    // Convert from milliseconds to seconds.
    m_deltaTime = (m_currentTime - m_oldTime) / 1000.f;

    m_sixtyFPSDeltaTime += m_deltaTime;
}


void Game::updateCapped()
{
    // Update keyboard data.
    HAPI->GetKeyboardData (&m_keyboard);
}


void Game::updateMain()
{
    // Calculate the boundaries for the circle.
    const float leftBounds = 0.f, upBounds = 0.f,
                rightBounds = (float) (m_screenWidth - m_circle.getWidth()),
                downBounds  = (float) (m_screenHeight - m_circle.getHeight());

    // Handle keyboard input.
    if (m_keyboard.scanCode[HK_LEFT] || m_keyboard.scanCode['A'])
    {
        m_circlePosition.x = max (m_circlePosition.x - circleSpeed * m_deltaTime, leftBounds);
    }

    if (m_keyboard.scanCode[HK_UP] || m_keyboard.scanCode['W'])
    {
        m_circlePosition.y = max (m_circlePosition.y - circleSpeed * m_deltaTime, upBounds);
    }

    if (m_keyboard.scanCode[HK_RIGHT] || m_keyboard.scanCode['D'])
    {
        m_circlePosition.x = min (m_circlePosition.x + circleSpeed * m_deltaTime, rightBounds);
    }

    if (m_keyboard.scanCode[HK_DOWN] || m_keyboard.scanCode['S'])
    {
        m_circlePosition.y = min (m_circlePosition.y + circleSpeed * m_deltaTime, downBounds);
    }
}


void Game::renderAll()
{
    // Render images.
    m_pScreenManager->blit ({ -10, -10 }, m_background, false);
    //m_pScreenManager->blit ((Vector2D<int>) m_circlePosition, m_circle);
}

#pragma endregion Engine functionality