#if !defined WATER_INPUT_ACTIONS_INCLUDED
#define WATER_INPUT_ACTIONS_INCLUDED


// Engine headers.
#include <Systems/Input/Enums.hpp>


// Engine namespace.
namespace water
{
    /// <summary>
    /// An action which requires checking the keyboard for a particular key press.
    /// </summary>
    struct KeyboardKey final
    {
        int id  { 0 };  //!< The integer ID of the action, this does not have to be exclusive which means a combination of keys and buttons can work together for the same action.
        Key key;        //!< The key to check for by the input system.

        KeyboardKey (const int id_, const Key key_)
            : id (id_), key (key_) { }

        KeyboardKey (const KeyboardKey& copy)               = default;
        KeyboardKey& operator= (const KeyboardKey& copy)    = default;
        ~KeyboardKey()                                      = default;

    };


    /// <summary>
    /// An action which requires checking a controller for a button press.
    /// </summary>
    struct ControllerButton final
    {
        int             id          { 0 };  //!< The integer ID of the action, this does not have to be exclusive which means a combination of keys and buttons can work together for the same action.
        unsigned int    controller  { 0 };  //!< The ID of the controller to check.
        unsigned int    button      { 0 };  //!< The ID of the button to check on the desired controller.

        ControllerButton (const int id_, const unsigned int controller_, const unsigned int button_)
            : id (id_), controller (controller_), button (button_) { }

        ControllerButton (const ControllerButton& copy)             = default;
        ControllerButton& operator= (const ControllerButton& copy)  = default;
        ~ControllerButton()                                         = default;
    };


    /// <summary>
    /// An action which requires obtaining the value of a controller axis.
    /// </summary>
    struct ControllerAxis final
    {
        int             id          { 0 };  //!< The integer ID of the action, this does not have to be exclusive which means a combination of keys and buttons can work together for the same action.
        unsigned int    controller  { 0 };  //!< The ID of the controller to check.
        Axis            axis;               //!< The axis to obtain the value of.

        ControllerAxis (const int id_, const unsigned int controller_, const Axis axis_)
            : id (id_), controller (controller_), axis (axis_) { }

        ControllerAxis (const ControllerAxis& copy)             = default;
        ControllerAxis& operator= (const ControllerAxis& copy)  = default;
        ~ControllerAxis()                                       = default;
    };
}


#endif
