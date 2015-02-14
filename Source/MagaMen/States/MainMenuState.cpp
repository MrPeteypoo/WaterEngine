#include "MainMenuState.hpp"


// Engine headers.
#include <Engine/Misc/Vector2.hpp>
#include <Engine/Interfaces/ITime.hpp>


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
        removePhysicsObjects();
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
        // Check for input to see if the user wants to start the game.

    }


    void MainMenuState::render()
    {
        // Render each object.
        for (auto& object : m_objects)
        {
            object.render();
        }

        // Make the "Press Start" text flicker on and off every third of a second.
        const float timeSinceStart  = StaticObject::time().timeSinceStart();
        const bool showText         = std::remainder (timeSinceStart, 0.66f) < 0.33f;

        if (showText)
        {
            // Since we've set the tiling viewport to 1x1 we can use relative co-ordinates!
            renderText ("Press Start", letters, numbers, { 0.333f, 0.666f }, 0.033f);
        }
    }

    #pragma endregion


    #pragma region Interal workings


    #pragma endregion
}