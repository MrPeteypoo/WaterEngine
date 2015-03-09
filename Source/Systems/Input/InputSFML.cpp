#include "InputSFML.hpp"


// STL headers.
#include <algorithm>


// Engine headers.
#include <Systems.hpp>
#include <Utility/Misc.hpp>


// Third party headers.
#include <SFML/Window/Joystick.hpp>
#include <SFML/Window/Keyboard.hpp>


// Engine namespace.
namespace water
{
    ////////////////////
    /// Constructors ///
    ////////////////////

    InputSFML::InputSFML (InputSFML&& move)
    {
        *this = std::move (move);
    }


    InputSFML& InputSFML::operator= (InputSFML&& move)
    {
        if (this != &move)
        {
            m_actions   = std::move (move.m_actions);
            m_keys      = std::move (move.m_keys);
            m_buttons   = std::move (move.m_buttons);
            m_axes      = std::move (move.m_axes);
        }

        return *this;
    }


    /////////////////////////
    /// System management ///
    /////////////////////////

    void InputSFML::initialise()
    {
        // Reserve some memory in the vectors and map to try and provide run-time speed for new actions.
        const auto reservation = 25U;

        m_actions.reserve (reservation);
        m_keys.reserve (reservation);
        m_buttons.reserve (reservation);
        m_axes.reserve (reservation);
    }


    void InputSFML::update()
    {
        // SFML requires controllers to be updated every frame.
        sf::Joystick::update();

        // Use the beauty of functional programming to do the work for us!
        std::for_each (m_keys.begin(),    m_keys.end(),    [&] (const KeyboardKey& key)         { processKey (key); });
        std::for_each (m_buttons.begin(), m_buttons.end(), [&] (const ControllerButton& button) { processButton (button); });
        std::for_each (m_axes.begin(),    m_axes.end(),    [&] (const ControllerAxis& axis)     { processAxis (axis); });

        // Finally increment the frame counter.
        ++m_frame;
    }


    /////////////////////////
    /// Internal workings ///
    /////////////////////////

    void InputSFML::processKey (const KeyboardKey& key)
    {
        // Use KeyButton algorithm to avoid code duplication.
        keyButtonAlgorithm (key.id, isKeyPressed (key.key));
    }


    void InputSFML::processButton (const ControllerButton& button)
    {
        // Don't waste computation cycles if the controller isn't plugged in.
        if (isConnected (button.controller))
        {
            // Use KeyButton algorithm to avoid code duplication.
            keyButtonAlgorithm (button.id, isButtonPressed (button.controller, button.button));
        }
    }


    void InputSFML::processAxis (const ControllerAxis& axis)
    {
        // Calculate the value of the axis.
        const auto value = axisPosition (axis.controller, axis.axis);

        // Construct a function to update the state with.
        const auto update = [=] (ActionState& action)
        {
            action.axis = value;
        };

        // Update the state of the action!
        updateActionState (axis.id, update);
    }


    void InputSFML::keyButtonAlgorithm (const int id, const bool pressed)
    {
        // Construct the beautiful function required here.
        const auto update = [=] (ActionState& action)
        {
            // We need to know if this current key/button is pressed.
            if (pressed)
            {
                // We know that the button is currently pressed so we can invert the previous value to know if we've pressed it down.
                action.down    = !action.previous;
                action.pressed = true;
                action.up      = false;
            }

            // Check if another binding has flagged the action as pressed, if so we can't possibly be up!
            else if (action.pressed || !action.previous)
            {
                action.up = false;
            }
        };

        // Update the state of the action.
        updateActionState (id, update);
    }


    void InputSFML::updateActionState (const int id, const std::function<void (ActionState&)>& updateFunction)
    {
        // We need to get access to the state first of all, if no bugs exist then .at() will never throw an exception.
        auto& actionState = m_actions.at (id);

        // Request it to reset, it will do this if the frame it last reset on isn't the current frame.
        actionState.requestReset (m_frame);

        // Update the state using the function given!
        if (updateFunction)
        {
            updateFunction (actionState);
        }
    }


    template <typename T, typename U>
    T InputSFML::getActionMember (const int id, const U& getMember) const
    {
        // Find the action in the map.
        const auto& iterator = m_actions.find (id);

        if (iterator != m_actions.cend())
        {
            return getMember (iterator->second);
        }

        // Warn the silly programmer!
        Systems::logger().logWarning ("InputSFML::getActionMember(), unable to find action " + std::to_string (id) + ".");

        return (T) 0;
    }


    template <typename T>
    void InputSFML::removeActionByID (const int id, std::vector<T>& actions)
    {
        // Simply iterate through the vector swapping and popping matching ID values!
        for (auto i = 0U; i < actions.size(); ++i)
        {
            auto& action = actions[i];
            if (action.id == id)
            {
                // Swap'n'pop!
                std::swap (action, actions.back());
                actions.pop_back();
                --i;

                // Update the references and erase the ActionState if necessary.
                if (!updateActionReferenceCount (id, false))
                {
                    m_actions.erase (id);
                    break;
                }
            }
        }
    }


    bool InputSFML::updateActionReferenceCount (const int id, const bool increment)
    {
        // Allow the function to create the ActionState if necessary since this will be called by addAction().
        auto& state = m_actions[id];

        // Change the reference count.
        if (increment)
        {
            ++state.references;
            return true;
        }

        else
        {
            return --state.references > 0;
        }
    }


    ///////////////
    /// Getters ///
    ///////////////

    bool InputSFML::isConnected (const unsigned int controller) const
    {
        return sf::Joystick::isConnected (controller);
    }


    bool InputSFML::hasAxis (const unsigned int controller, const Axis axis) const
    {
        return sf::Joystick::hasAxis (controller, (sf::Joystick::Axis) axis);
    }


    unsigned int InputSFML::getButtonCount (const unsigned int controller) const
    {
        return sf::Joystick::getButtonCount (controller);
    }


    unsigned int InputSFML::getKeyCount() const
    {
        return sf::Keyboard::KeyCount;
    }


    bool InputSFML::getActionPressed (const int id) const
    {
        // We need to get the "pressed" member.
        return getActionMember<bool> (id, [] (const ActionState& action) { return action.pressed; });
    }


    bool InputSFML::getActionDown (const int id) const
    {
        // We need to get the "down" member.
        return getActionMember<bool> (id, [] (const ActionState& action) { return action.down; });
    }


    bool InputSFML::getActionUp (const int id) const
    {
        // We need to get the "up" member.
        return getActionMember<bool> (id, [] (const ActionState& action) { return action.up; });
    }


    float InputSFML::getActionAxis (const int id) const
    {
        // We need to get the "axis" member.
        return getActionMember<float> (id, [] (const ActionState& action) { return action.axis; });
    }


    ///////////////////////
    /// Real-time state ///
    ///////////////////////

    bool InputSFML::isKeyPressed (const Key key) const
    {
        return sf::Keyboard::isKeyPressed ((sf::Keyboard::Key) key);
    }


    bool InputSFML::isButtonPressed (const unsigned int controller, const unsigned int button) const
    {
        return sf::Joystick::isButtonPressed (controller, button);
    }


    float InputSFML::axisPosition (const unsigned int controller, const Axis axis) const
    {
        return sf::Joystick::getAxisPosition (controller, (sf::Joystick::Axis) axis) / 100.f;
    }


    /////////////////////////
    /// Action management ///
    /////////////////////////

    bool InputSFML::addAction (const KeyboardKey& action)
    {
        // Nothing to test here! Add it straight away!
        m_keys.push_back (action);
        return updateActionReferenceCount (action.id, true);
    }


    bool InputSFML::addAction (const ControllerButton& action)
    {
        // We need to check whether the specified controller and button values CAN be valid for SFML.
        if (action.controller <= sf::Joystick::Count && action.button <= sf::Joystick::ButtonCount)
        {
            m_buttons.push_back (action);
            return updateActionReferenceCount (action.id, true);
        }

        // Log the error.
        Systems::logger().logError ("InputSFML::addAction(), attempt to add a ControllerButton with an invalid controller or button ID. ID = " +
                                    std::to_string (action.id) + ", controller = " + std::to_string (action.controller) + ", button = " + std::to_string (action.button) + ".");
        return false;
    }


    bool InputSFML::addAction (const ControllerAxis& action)
    {
        // Check if the controller value is valid.
        if (action.controller <= sf::Joystick::Count)
        {
            // Ensure an axis action doesn't already exist by removing any that do.
            removeActionByID (action.id, m_axes);

            m_axes.push_back (action);
            return updateActionReferenceCount (action.id, true);
        }

        Systems::logger().logError ("InputSFML::addAction(), attempt to add a ControllerAxis with an invalid controller ID. ID = " + std::to_string (action.id) +
                                    ", controller = " + std::to_string (action.controller) + ", axis = " + std::to_string ((int) action.axis) + ".");
        return false;
    }


    void InputSFML::removeAction (const int id, const Action type)
    {
        // Test if the action exists here instead of wasting computation time.
        if (m_actions.find (id) != m_actions.end())
        {
            switch (type)
            {
                case Action::All:
                    removeActionByID (id, m_keys);
                    removeActionByID (id, m_buttons);
                    removeActionByID (id, m_axes);
                    break;

                case Action::Key:
                    removeActionByID (id, m_keys);
                    break;

                case Action::Button:
                    removeActionByID (id, m_buttons);
                    break;

                case Action::Axis:
                    removeActionByID (id, m_axes);
                    break;
            }
        }
    }


    void InputSFML::removeActions()
    {
        // Just remove all current data, don't call clear so that we can keep the reserved capacity.
        m_actions.erase (m_actions.cbegin(), m_actions.cend());

        util::removeContents (m_keys);
        util::removeContents (m_buttons);
        util::removeContents (m_axes);
    }
}
