#include "CutManState.hpp"


// Engine headers.
#include <Engine/Interfaces/IGameWorld.hpp>
#include <Engine/Interfaces/IInput.hpp>
#include <Engine/Interfaces/ITime.hpp>


// Game headers.
#include <MagaMen/Misc/StateBuilder.hpp>
#include <MagaMen/Misc/Utility.hpp>


// Game namespace.
namespace mm
{
    #pragma region Constructors

    CutManState::CutManState (CutManState&& move)
    {
        *this = std::move (move);
    }


    CutManState& CutManState::operator= (CutManState&& move)
    {
        if (this != &move)
        {
            MagaMenLevelState::operator= (std::move (move));

            m_background = move.m_background;
            m_bgPosition = std::move (move.m_bgPosition);

            m_score      = move.m_score;

            m_lives      = std::move (move.m_lives);
            m_player     = std::move (move.m_player);

            // Reset primitives.
            m_background = 0;
            m_score      = 0;
        }

        return *this;
    }

    #pragma endregion


    #pragma region Game flow

    bool CutManState::onAdd()
    {
        return StateBuilder::loadFromFile (*this);
    }


    bool CutManState::onRemove()
    {
        return true;
    }


    void CutManState::onEntry()
    {
        // Load and play the music.
        audio().loadMusic (m_bgmFile);
        audio().playMusic (m_bgmVolume);

        // Reset the state.
        m_player.initialise();
        m_bgPosition.setPosition (0, 0);
        m_score  = 0;
        m_paused = false;

        addPhysicsObject (&m_player);

        // Adjust the viewport.
        renderer().setViewport ({ 0, 0, 14, 13 });
    }


    void CutManState::onExit()
    {
        // Stop the music.
        audio().stopMusic();

        removePhysicsObjects();
    }


    void CutManState::updatePhysics()
    {
        if (!m_paused)
        {
            m_player.updatePhysics();
        }
    }


    void CutManState::update()
    {
        // Pause the game if desired and restart if we're paused when they press back.
        if (input().getActionDown ((int) Action::Start))
        {
            time().setTimescale (m_paused ? 1 : 0);
            m_paused = !m_paused;
        }
        
        if (m_paused)
        {
            if (input().getActionDown ((int) Action::Back))
            {
                gameWorld().requestPop();
                time().setTimescale (1);
            }            
        }

        else
        {
            // It should take 5 seconds to scroll the entire background.
            m_bgPosition.x -= 30 * (time().getDelta() / 5.f);

            if (m_bgPosition.x < -30)
            {
                m_bgPosition.x += 30;
            }

            m_player.update();
        }        
    }


    void CutManState::render()
    {
        // Render each object.
        const auto backgroundLength = Vector2<float> { 30, 0 };

        renderer().drawToScreen (m_bgPosition, m_background, water::BlendType::Opaque);
        renderer().drawToScreen (backgroundLength + m_bgPosition, m_background, water::BlendType::Opaque);

        m_player.render();

        // Render the life count.
        m_lives.render();
        renderText (std::to_string (m_player.getLives()), letters, numbers, { 2.25f, 12.35f }, 0.5f);
        
        // Render the score.
        renderText ("Score " + std::to_string (m_score), letters, numbers, { 1, 1 }, 0.5f);
    }

    #pragma endregion


    #pragma region Level management

    /*void CutManState::increaseScore (const int value)
    {
    }*/


    

    #pragma endregion
}