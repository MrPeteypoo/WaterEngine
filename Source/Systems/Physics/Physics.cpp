#include "Physics.hpp"


// Engine namespace.
#include <GameComponents/PhysicsObject.hpp>


// Engine namespace.
namespace water
{
    ////////////////////
    /// Constructors ///
    ////////////////////

    Physics::Physics (Physics&& move)
    {
        *this = std::move (move);
    }


    Physics& Physics::operator= (Physics&& move)
    {
        if (this != &move)
        {
            m_layers = std::move (move.m_layers);
        }

        return *this;
    }


    /////////////////////////
    /// System management ///
    /////////////////////////

    void Physics::initialise()
    {
        // Nothing to do but resize the layermask collection.
        m_layers.resize (32);
    }


    void Physics::detectCollisions (const std::vector<PhysicsObject*>& objects)
    {
        // Lets go through each collidable object checking if they collide.
        for (auto i = 0U; i < objects.size(); ++i)
        {
            // Cache the values for the first object.
            auto        first        = objects[i];
            const auto& check        = first->getCollider();
            const auto  checkLayer   = m_layers[check.getLayer()];

            auto checkBox           = check.getBox();
            checkBox.translate (first->getPosition().x, first->getPosition().y);

            for (auto j = i + 1; j < objects.size(); ++j)
            {
                // Cache the values for the second object.
                auto        second  = objects[j];
                const auto& against = second->getCollider();

                // Check if their layers collide.
                if ((checkLayer | (1 << against.getLayer())) > 0)
                {
                    // We must now check if the rectangles intersect.
                    auto againstBox = against.getBox();
                    againstBox.translate (second->getPosition().x, second->getPosition().y);

                    // Check if they intersect.
                    if (checkBox.intersects (againstBox))
                    {
                        // Determine the desired collision type.
                        if (check.isTrigger())
                        {
                            first->onTrigger (second);

                            if (against.isTrigger())
                            {
                                second->onTrigger (first);
                            }
                        }

                        // Collider on trigger.
                        else if (against.isTrigger())
                        {
                            second->onTrigger (first);
                        }

                        // Collider on collider.
                        else
                        {
                            first->onCollision (second);
                            second->onCollision (first);
                        }
                    }
                }
            }
        }
    }


    ////////////////////////
    /// Layer management ///
    ////////////////////////

    void Physics::setLayerMask (const unsigned int layer, const unsigned int collidable)
    {
        // Pre-condition: The given layer is valid.
        if (layer < 32)
        {
            m_layers[layer] = collidable;
        }
    }


    void Physics::addToMask (const unsigned int layerToModify, const unsigned int layerToAdd)
    {
        // Pre-condition: Both layers are valid.
        if (layerToModify < 32 && layerToAdd < 32)
        {
            m_layers[layerToModify] |= 1 << layerToAdd;
        }
    }


    void Physics::removeFromMask (const unsigned int layerToModify, const unsigned int layerToRemove)
    {
        // Pre-condition: Both layers are valid.
        if (layerToModify < 32 && layerToRemove < 32)
        {
            m_layers[layerToModify] &= ~(1 << layerToRemove);
        }
    }
}
