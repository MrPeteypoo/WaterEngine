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
    };


    /// <summary>
    /// An action which requires checking a controller for a button press.
    /// </summary>
    struct ControllerButton final
    {
        int             id          { 0 };  //!< The integer ID of the action, this does not have to be exclusive which means a combination of keys and buttons can work together for the same action.
        unsigned int    controller  { 0 };  //!< The ID of the controller to check.
        unsigned int    button      { 0 };  //!< The ID of the button to check on the desired controller.
    };


    /// <summary>
    /// An action which requires obtaining the value of a controller axis.
    /// </summary>
    struct ControllerAxis final
    {
        int             id          { 0 };  //!< The integer ID of the action, this does not have to be exclusive which means a combination of keys and buttons can work together for the same action.
        unsigned int    controller  { 0 };  //!< The ID of the controller to check.
        Axis            axis;               //!< The axis to obtain the value of.
    };
}


#endif