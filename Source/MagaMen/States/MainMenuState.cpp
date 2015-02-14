#include "MainMenuState.hpp"


// Engine headers.
#include <Engine/Misc/Vector2.hpp>


// Game headers.
#include <MagaMen/Utility/Utility.hpp>


// Game namespace.
namespace mm
{
    #pragma region Constructors and destructor

    MainMenuState::MainMenuState (MainMenuState&& move)
    {
        *this = std::move (move);
    }


    MainMenuState& MainMenuState::operator= (MainMenuState&& move)
    {
        if (this != &move)
        {
            MagaMenState::operator= (std::move (move));

            m_dataFile  = std::move (move.m_dataFile);
            m_bgmFile   = std::move (move.m_bgmFile);
            m_bgmVolume = move.m_bgmVolume;

            // Reset primitives.
            move.m_bgmVolume = 0;
        }

        return *this;
    }

    #pragma endregion


    #pragma region Game flow

    bool MainMenuState::onAdd()
    {
        return true;
    }


    bool MainMenuState::onRemove()
    {
        return true;
    }


    void MainMenuState::onEntry()
    {
        // Load and play the music.
        audio().loadMusic (m_bgmFile);
        audio().playMusic (m_bgmVolume);
    }


    void MainMenuState::onExit()
    {
        // Stop the music.
        audio().stopMusic();
    }


    void MainMenuState::update()
    {
    
    }


    void MainMenuState::render()
    {
        renderText ("AN73 VOX02-;S 24P[N", 0, 0, { 0, 0 }, 8);
    }

    #pragma endregion


    #pragma region Interal workings


    #pragma endregion
}