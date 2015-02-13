#include "MagaMen.hpp"


// Engine headers.
#include <Engine/WaterEngine.hpp>


// Game headers.
#include <MagaMen/States/MainMenuState.hpp>


// Game namespace.
namespace mm
{
    void Game::run()
    {
        // Create and initialise the engine so we can use the logger if required.
        m_engine = std::make_unique<water::Engine>();

        if (m_engine->initialise ("../EngineConfig.xml"))
        {
            try
            {
                // Create the states.
                createStates();

                // Add each state to the game world.
                addStatesToWorld();

                // Push the starting state.
                m_engine->getGameWorld().requestPush ((int) StateID::MainMenu);

                // Run the game!
                m_engine->run();
            }

            catch (const std::exception& error)
            {
                water::Systems::logger().logError (error.what());
            }

            catch (...)
            {
                water::Systems::logger().logError ("Game::run(), an unknown error occurred, the application will close.");
            }
        }
    }


    void Game::createStates()
    {
        // Start by creating the states.
        m_mainMenu = std::make_shared<MainMenuState> ("Nothing.xml");
    }


    void Game::addStatesToWorld()
    {
        // Cache a reference to the game world.
        auto& world = m_engine->getGameWorld();

        // Simply add each state with the corresponding ID.
        world.addState ((int) StateID::MainMenu, m_mainMenu);
    }
}