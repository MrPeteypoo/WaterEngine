#include "GameWorld.hpp"


// Engine headers.
#include <GameComponents/GameState.hpp>
#include <Systems.hpp>


// Engine namespace.
namespace water
{
    /////////////////////////////////
    // Constructors and destructor //
    /////////////////////////////////

    GameWorld::GameWorld (GameWorld&& move)
    {
        *this = std::move (move);
    }


    GameWorld& GameWorld::operator= (GameWorld&& move)
    {
        if (this != &move)
        {
            // Ensure our states are cleaned before moving data.
            clear();

            // Obtain ownership.
            m_states = std::move (move.m_states);
            m_stack  = std::move (move.m_stack);
        }

        return *this;
    }


    GameWorld::~GameWorld()
    {
        // Ensure the stack and map are clean.
        clear();
    }


    ///////////////////////
    // System management //
    ///////////////////////

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


    void GameWorld::processQueue()
    {
        // Iterate through the queue performing the specified task.
        while (!m_tasks.empty())
        {
            // Each task is a function!
            auto& task = m_tasks.front();
            if (task)
            {
                task();
            }

            // Pop the task! Eagle eyes are handy when you forget things like this, it's OK I fixed his bug too.
            m_tasks.pop();
        }
    }


    /////////////
    // Getters //
    /////////////

    const std::vector<PhysicsObject*>& GameWorld::getPhysicsObjects() const
    {
        if (!m_stack.empty())
        {
            return m_stack.top()->getPhysicsObjects();
        }

        throw std::runtime_error ("Call to GameWorld::getPhysicsObjects() when the stack is empty.");
    }


    //////////////////////
    // State management //
    //////////////////////

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

        // Stop execution immediately.
        throw std::runtime_error ("GameWorld::addState(), the onAdd() method for state " + std::to_string (id) + " returned false, state was not added.");
    }


    bool GameWorld::removeState (const int id)
    {
        // Pre-condition: The state exists.
        const auto& iterator = m_states.find (id);

        if (iterator == m_states.end())
        {
            Systems::logger().logWarning ("GameWorld::removeState(), attempt to remove non-existent state " + std::to_string (id) + ".");
            return false;
        }

        // Call onRemove to make sure the state cleans itself.
        if (iterator->second->onRemove())
        {
            // Remove the state.
            m_states.erase (id);
            return true;
        }

        // Stop execution immediately.
        throw std::runtime_error ("GameWorld::removeState(), the onRemove() method for state " + std::to_string (id) + " returned false.");
    }


    void GameWorld::requestPush (const int id)
    {
        // Add to the task list!
        const auto function = [=] () { push (id); };
        m_tasks.push (function);
    }


    void GameWorld::requestPop()
    {
        // Add to the task list!
        const auto function = [=] () { pop(); };
        m_tasks.push (function);
    }


    void GameWorld::requestSwap (const int id)
    {
        // Add to the task list!
        const auto function = [=] () { swap (id); };
        m_tasks.push (function);
    }


    void GameWorld::requestExit()
    {
        // Add to the task list!
        const auto function = [=] () { clear(); };
        m_tasks.push (function);
    }


    ///////////////////////
    // Internal workings //
    ///////////////////////

    void GameWorld::push (const int id)
    {
        // Pre-condition: The ID is valid.
        const auto& iterator = m_states.find (id);

        if (iterator == m_states.end())
        {
            Systems::logger().logError ("GameWorld::push(), attempt to push non-existent state " + std::to_string (id) + ".");
        }

        else
        {
            // Inform the programmer if they're pushing a state on top of itself. This should be handled carefully as it may be intentional.
            if (!m_stack.empty() && iterator->second == m_stack.top())
            {
                Systems::logger().logWarning ("GameWorld::push(), pushing state " + std::to_string (id) + " on top of itself.");
            }

            // Push it onto the stack.
            iterator->second->onEntry();
            m_stack.push (iterator->second);
        }
    }


    void GameWorld::push (const std::shared_ptr<GameState>& state)
    {
        // Pre-condition: We have a valid pointer.
        if (state)
        {
            state->onEntry();
            m_stack.push (state);
        }
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

            m_stack.top()->onExit();
            m_stack.pop();
        }

        if (!m_stack.empty())
        {
            m_stack.top()->onEntry();
        }
    }


    void GameWorld::swap (const int id)
    {
        // Pre-condition: The ID is valid.
        const auto& iterator = m_states.find (id);

        if (iterator == m_states.end())
        {
            Systems::logger().logError ("GameWorld::swap(), attempt to swap to non-existent state " + std::to_string (id) + ".");
        }

        else
        {
            // Pop the stack!
            pop();

            // Push to the stack!
            push (iterator->second);
        }
    }


    void GameWorld::clear()
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
}
