#include "WaterToken.hpp"


// Engine headers.
#include <Engine/WaterEngine.hpp>


// Game namespace.
namespace wt
{
    void Game::run()
    {
        m_engine = std::make_unique<water::Engine>();

        if (m_engine->initialise ("../EngineConfig.xml"))
        {
            m_engine->run();
        }
    }
}