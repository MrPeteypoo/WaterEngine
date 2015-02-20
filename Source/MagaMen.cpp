#include "MagaMen.hpp"


// Engine headers.
#include <Engine/WaterEngine.hpp>


// Game headers.
#include <MagaMen/States/MainMenuState.hpp>
#include <MagaMen/States/CutManState.hpp>


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

                // Enable input.
                addControllerInput();
                addKeyboardInput();

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
        const auto workingDir = std::string ("../");

        // Start by creating the states.
        m_mainMenu = std::make_shared<MainMenuState> (workingDir + "MainMenuState.xml");
        m_cutMan   = std::make_shared<CutManState> (workingDir + "CutManState.xml");
    }


    void Game::addStatesToWorld()
    {
        // Cache a reference to the game world.
        auto& world = m_engine->getGameWorld();

        // Simply add each state with the corresponding ID.
        world.addState ((int) StateID::MainMenu, m_mainMenu);
        world.addState ((int) StateID::CutManStage, m_cutMan);
    }


    void Game::addControllerInput()
    {
        // Left analogue stick for movement.
        water::ControllerAxis   xAxis   { (int) Action::Right, 0, water::Axis::X },
                                yAxis   { (int) Action::Down,  0, water::Axis::Y };

        // A for shoot, start for start and back for back.
        water::ControllerButton shoot   { (int) Action::Shoot, 0, 0 },
                                start   { (int) Action::Start, 0, 7 },
                                back    { (int) Action::Back,  0, 6 };
        
        water::Systems::input().addAction (xAxis);
        water::Systems::input().addAction (yAxis);
        water::Systems::input().addAction (shoot);
        water::Systems::input().addAction (start);
        water::Systems::input().addAction (back);
    }


    void Game::addKeyboardInput()
    {
        // W, A, S, D, up, left, down, right for movement.
        water::KeyboardKey  up1     { (int) Action::Up,     water::Key::W },
                            up2     { (int) Action::Up,     water::Key::Up },
                            left1   { (int) Action::Left,   water::Key::A },
                            left2   { (int) Action::Left,   water::Key::Left },
                            down1   { (int) Action::Down,   water::Key::S },
                            down2   { (int) Action::Down,   water::Key::Down },
                            right1  { (int) Action::Right,  water::Key::D },
                            right2  { (int) Action::Right,  water::Key::Right },

                            shoot   { (int) Action::Shoot,  water::Key::Space },
                            start   { (int) Action::Start,  water::Key::Return },
                            back    { (int) Action::Back,   water::Key::BackSpace };
        
        water::Systems::input().addAction (up1);
        water::Systems::input().addAction (up2);
        water::Systems::input().addAction (left1);
        water::Systems::input().addAction (left2);
        water::Systems::input().addAction (down1);
        water::Systems::input().addAction (down2);
        water::Systems::input().addAction (right1);
        water::Systems::input().addAction (right2);

        water::Systems::input().addAction (shoot);
        water::Systems::input().addAction (start);
        water::Systems::input().addAction (back);
    }
}