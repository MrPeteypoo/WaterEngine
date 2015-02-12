#include "GameWorld.hpp"


// Engine headers.
#include <GameComponents/GameState.hpp>
#include <Systems.hpp>


// Engine namespace.
namespace water
{
    #pragma region Constructors and destructor

    GameWorld::GameWorld (GameWorld&& move)
    {
        *this = std::move (move);
    }


    GameWorld& GameWorld::operator= (GameWorld&& move)
    {
        if (this != &move)
        {
            // Ensure our states are cleaned before moving data.
            clearWorld();

            // Obtain ownership.
            m_states = std::move (move.m_states);
            m_stack  = std::move (move.m_stack);
        }
        
        return *this;
    }


    GameWorld::~GameWorld()
    {
        // Ensure the stack and map are clean.
        clearWorld();
    }

    #pragma endregion


    #pragma region System management

    void GameWorld::updatePhysics()
    {
        if (!m_stack.empty())
        {
            m_stack.top()->updatePhysics();
        }
    }


    void GameWorld::update()
    {
        if (!m_stack.empty())
        {
            m_stack.top()->update();
        }
    }


    void GameWorld::render()
    {
        if (!m_stack.empty())
        {
            m_stack.top()->render();
        }
    }


    void GameWorld::endFrame()
    {
        // Clean up after ourselves if we've been requested to exit.
        if (m_exit)
        {
            clearWorld();
        }
    }

    #pragma endregion


    #pragma region Getters

    const std::vector<PhysicsObject*>& GameWorld::getPhysicsObjects() const
    {
        if (!m_stack.empty())
        {
            m_stack.top()->getPhysicsObjects();
        }

        throw std::runtime_error ("Call to GameWorld::getPhysicsObjects() when the stack is empty.");
    }

    #pragma endregion


    #pragma region State management

    bool GameWorld::addState (const int id, const std::shared_ptr<GameState>& state)
    {
        // Pre-condition: State is valid.
        if (!state)
        {
            Systems::logger().logError ("GameWorld::addState(), attempt to add invalid state " + std::to_string (id) + ", pointer was null.");
            return false;
        }

        // Pre-condition: A state with the same ID doesn't exist.
        if (m_states.find (id) != m_states.end())
        {
            Systems::logger().logError ("GameWorld::addState(), attempt to overwrite state " + std::to_string (id) + ", state ignored.");
            return false;
        }

        // Check if the state should be added.
        if (state->onAdd())
        {
            m_states[id] = state;
            return true;
        }

        Systems::logger().logError ("GameWorld::addState(), the onAdd() method for state " + std::to_string (id) + " returned false, state was not added.");
        return false;
    }


    bool GameWorld::removeState (const int id)
    {
        // Pre-condition: The state exists.
        auto& iterator = m_states.find (id);

        if (iterator == m_states.end())
        {
            Systems::logger().logWarning ("GameWorld::removeState(), attempt to remove non-existent state " + std::to_string (id) + ".");
            return false;
        }

        // Call onRemove to make sure the state cleans itself.
        const bool successful = iterator->second->onRemove();

        if (!successful)
        {
            Systems::logger().logWarning ("GameWorld::removeState(), the onRemove() method for state " + std::to_string (id) + " returned false.");   
        }

        // Remove the state.
        m_states.erase (id);
        return successful;
    }


    bool GameWorld::push (const int id)
    {
        // Pre-condition: The ID is valid.
        auto& iterator = m_states.find (id);

        if (iterator == m_states.end())
        {
            Systems::logger().logError ("GameWorld::push(), attempt to push non-existent state " + std::to_string (id) + ".");
            return false;
        }

        // Push it to the top of the stack.
        iterator->second->onEntry();
        m_stack.push (iterator->second);

        return true;
    }


    void GameWorld::pop()
    {
        // Why can't all functions be parameterless? It makes things so simple!
        if (!m_stack.empty())
        {
            // Exit the game if this is the initial state.
            if (m_stack.size() == 1)
            {
                requestExit();
            }

            else
            {
                m_stack.top()->onExit();
                m_stack.pop();
            }
        }
    }


    bool GameWorld::swap (const int id)
    {
        // Pre-condition: The ID is valid.
        auto& iterator = m_states.find (id);

        if (iterator == m_states.end())
        {
            Systems::logger().logError ("GameWorld::swap(), attempt to swap to non-existent state " + std::to_string (id) + ".");
            return false;
        }

        // Pop the stack!
        pop();

        // Push to the stack!
        iterator->second->onEntry();
        m_stack.push (iterator->second);

        return true;
    }

    #pragma endregion


    #pragma region Internal workings

    void GameWorld::clearWorld()
    {
        // Exit all states.
        while (!m_stack.empty())
        {
            m_stack.top()->onExit();
            m_stack.pop();
        }

        // Clean all states.
        for (auto& state : m_states)
        {
            if (!state.second->onRemove())
            {
                Systems::logger().logWarning ("GameWorld::clearWorld(), the onRemove() method for state " + std::to_string (state.first) + " returned false.");
            }
        }

        m_states.clear();
    }

    #pragma endregion
}