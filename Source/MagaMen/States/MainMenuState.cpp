#include "MainMenuState.hpp"


// Engine headers.
#include <Engine/Misc/Rectangle.hpp>
#include <Engine/Interfaces/IGameWorld.hpp>
#include <Engine/Interfaces/IInput.hpp>
#include <Engine/Interfaces/ITime.hpp>


// Game headers.
#include <MagaMen/Misc/StateBuilder.hpp>
#include <MagaMen/Misc/Utility.hpp>


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

            m_objects = std::move (move.m_objects);
        }

        return *this;
    }

    #pragma endregion


    #pragma region Game flow

    bool MainMenuState::onAdd()
    {
        return StateBuilder::loadFromFile (*this);
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

        // Adjust the viewport.
        renderer().setViewport ({ 0, 0, 0, 0 });
    }


    void MainMenuState::onExit()
    {
        // Stop the music.
        audio().stopMusic();
    }


    void MainMenuState::update()
    {
        // Check for input to see if the user wants to start the game.
        if (input().getActionDown ((int) Action::Back))
        {
            gameWorld().requestExit();
        }
        
        else if (input().getActionDown ((int) Action::Start))
        {
            gameWorld().requestPush ((int) StateID::CutManStage);
        }
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
        const bool showText         = std::fmod (timeSinceStart, 2.0f) < 1.5f;

        if (showText)
        {
            // Since we've set the tiling viewport to 1x1 we can use relative co-ordinates!
            renderText ("PRESS START", letters, numbers, { 0.333f, 0.666f }, 0.033f);
        }
    }

    #pragma endregion
}