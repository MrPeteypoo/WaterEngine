#if !defined WATER_INTERFACE_INPUT_INCLUDED
#define WATER_INTERFACE_INPUT_INCLUDED


// Engine headers.
#include <Systems/Input/Actions.hpp>


// Engine namespace.
namespace water
{
    /// <summary>
    /// Specifies the type of action required.
    /// </summary>
    enum class Action : int
    {
        Key     = 0,    //!< Used for keyboard actions.
        Button  = 1,    //!< Used for controller button actions.
        Axis    = 2,    //!< Used for controller axis actions.
        All     = 3     //!< Applies to all actions.
    };


    /// <summary>
    /// An interface to every input system used by the engine. This is useful for tracking keys, buttons and axis. There is currently no mouse support.
    /// A warning about usage; the system is designed for you to use the action system, this allows for quick run-time checking of input, values are
    /// cached every frame for quick access. Run-time speed will be very fast provided actions are not added and removed at run-time, these operations
    /// should be performed at load-time. Real-time state access of raw input is provided but no guarantees are made about the speed of such functions.
    /// </summary>
    class IInput
    {
        public: 

            // Ensure destructor is virtual for the interface!
            virtual ~IInput() { }


            #pragma region Getters

            /// <summary> Checks whether a controller is connected. </summary>
            /// <param name="controller"> The index of the desired controller. </param>
            /// <returns> The connection status of the desired controller. </returns>
            virtual bool isConnected (const unsigned int controller) const = 0;

            /// <summary> Checks whether a controller supports the desired axis. </summary>
            /// <param name="controller"> The index of the desired controller. </param>
            /// <param name="axis"> The axis to check support for. </param>
            /// <returns> If the controller has the axis, false if the controller isn't connected. </returns>
            virtual bool hasAxis (const unsigned int controller, const Axis axis) const = 0;

            /// <summary> Checks how many buttons exist on the controller. </summary>
            /// <param name="controller"> The index of the desired controller. </param>
            /// <returns> The button count, zero if the controller doesn't exist. </returns>
            virtual unsigned int getButtonCount (const unsigned int controller) const = 0;

            /// <summary> Checks how many keys exist on the current keyboard. </summary>
            /// <returns> The key count. </returns>
            virtual unsigned int getKeyCount() const = 0;

            /// <summary> Checks if the action is currently pressed, this is specific to the current frame. </summary>
            /// <param name="id"> The ID of the action to check. </param>
            /// <returns> The status of the action, false if the action is invalid. </returns>
            virtual bool getActionPressed (const int id) const = 0;

            /// <summary> Checks if this is the first frame an action is no longer being held down. </summary>
            /// <param name="id"> The ID of the action to check. </param>
            /// <returns> The "up" status of the action, false if the action is invalid. </returns>
            virtual bool getActionUp (const int id) const = 0;

            /// <summary> Checks if this is the first frame an action is being pushed down. </summary>
            /// <param name="id"> The ID of the action to check. </param>
            /// <returns> The "down" status of the action, false if the action is invalid. </returns>
            virtual bool getActionDown (const int id) const = 0;

            /// <summary> Obtains the value of an axis action with the given ID. </summary>
            /// <param name="id"> The ID of the action to check. </param>
            /// <returns> The current value of the axis, zero if the action is invalid. </returns>
            virtual float getActionAxis (const int id) const = 0;

            #pragma endregion


            #pragma region Real-time state

            /// <summary> Performs a real-time check on the state of a key, this is accessing the raw data and could be slow. </summary>
            /// <param name="key"> The key to check the status of. </param>
            /// <returns> Whether the key is currently being pressed. </returns>
            virtual bool isKeyPressed (const Key key) const = 0;
        
            /// <summary> Performs a real-time check on the state of a button, this is accessing the raw data and could be slow. </summary>
            /// <param name="controller"> The index of the desired controller. </param>
            /// <param name="button"> The index of the desired button. </param>
            /// <returns> The status of the button, false if the controller or button are invalid. </returns>
            virtual bool isButtonPressed (const unsigned int controller, const unsigned int button) const = 0;

            /// <summary> Performs a real-time check on the position of an axis, this is accessing raw data and could be slow. </summary>
            /// <param name="controller"> The index of the desired controller. </param>
            /// <param name="axis"> The desired axis to obtain information for. </param>
            /// <returns> The position of the axis, zero if the controller is invalid. </returns>
            virtual float axisPosition (const unsigned int controller, const Axis axis) const = 0;

            #pragma endregion


            #pragma region Action management

            /// <summary> Adds a KeyboardKey action to the list of actions to check the status of. </summary>
            /// <param name="action"> The action to be added. </param>
            /// <returns> Whether the action was added successfully. </returns>
            virtual bool addAction (const KeyboardKey& action) = 0;
        
            /// <summary> Adds a ControllerButton action to the list of actions to check the status of. </summary>
            /// <param name="action"> The action to be added. </param>
            /// <returns> Whether the action was added successfully. </returns>
            virtual bool addAction (const ControllerButton& action) = 0;
        
            /// <summary> Adds a ControllerAxis action to the list of axis to check the status of. </summary>
            /// <param name="action"> The action to be added. The ID of the action must be unique here. </param>
            /// <returns> Whether the action was added successfully. </returns>
            virtual bool addAction (const ControllerAxis& action) = 0;

            /// <summary> Attempts to remove actions related to the given ID. </summary>
            /// <param name="id"> The ID of actions to be removed. </param>
            /// <param name="type"> Specifies the type of actions to remove, e.g. Key will remove all KeyboardKey actions for the ID. </param>
            virtual void removeAction (const int id, const Action type = Action::All) = 0;

            /// <summary> Completely clears the list of actions, all action checks from this point will be invalid. </summary>
            virtual void removeActions() = 0;

            #pragma endregion
    };
}


#endif