#include "AGameState.hpp"


// STL headers.
#include <algorithm>


// Engine headers.
#include <Systems.hpp>


// Engine namespace.
namespace water
{
    #pragma region Constructor

    AGameState::AGameState (const unsigned int elementCount)
    {
        m_objects.reserve (elementCount);
    }

    #pragma endregion


    #pragma region Physics management

    void AGameState::addPhysicsObject (IPhysicsObject* const object)
    {
        // Pre-condition: The object isn't a nullptr.
        if (!object)
        {
            Systems::logger().logWarning ("AGameState::addPhysicsObject(), attempt to add a nullptr.");
        }
        
        else
        {
            m_objects.push_back (object);
        }
    }


    void AGameState::addUniquePhysicsObject (IPhysicsObject* const object)
    {
        // Pre-condition: The object isn't a nullptr.
        if (!object)
        {
            Systems::logger().logWarning ("AGameState::addUniquePhysicsObject(), attempt to add a nullptr.");
        }

        else
        {
            // Attempt to find the object.
            const auto iterator = findObject (object);
        
            // If the iterator is at equal to the end we know it doesn't exist.
            if (iterator == m_objects.end())
            {
                m_objects.push_back (object);
            }

            else
            {
                Systems::logger().logWarning ("AGameState::addUniquePhysicsObject(), attempt to add an object that has already been added.");
            }
        }
    }


    void AGameState::removePhysicsObject (IPhysicsObject* const object)
    {
        // Pre-condition: The object isn't a nullptr.
        if (!object)
        {
            Systems::logger().logWarning ("AGameState::removePhysicsObject(), attempt to remove a nullptr.");   
        }

        else
        {
            // Attempt to find the object.
            auto iterator = findObject (object);

            // Check if the result is valid.
            if (iterator != m_objects.end())
            {
                // Swap and pop for efficiency!
                std::swap (*iterator, m_objects.back());
                m_objects.pop_back();
            }

            else
            {
                Systems::logger().logWarning ("AGameState::removePhysicsObject(), attempt to remove a non-existent object.");
            }
        }
    }


    void AGameState::removePhysicsObjects()
    {
        // Pop until empty to ensure we keep our reserved capacity.
        while (!m_objects.empty())
        {
            m_objects.pop_back();
        }
    }

    #pragma endregion


    #pragma region Physics intergration

    std::vector<IPhysicsObject*>::iterator AGameState::findObject (const IPhysicsObject* const object)
    {
        // Construct a function to find where the object is.
        const auto equal = [=] (const IPhysicsObject* const test)
        {
            return test == object;  
        };

        // Find the object.
        auto& location = std::find_if (m_objects.begin(), m_objects.end(), equal);

        return location;
    }

    #pragma endregion


    #pragma region Utility functions

    IGameWorld& AGameState::gameWorld() const
    {
        return Systems::gameWorld();
    }

    #pragma endregion
}