#if !defined WATER_INPUT_SFML_INCLUDED
#define WATER_INPUT_SFML_INCLUDED


// STL headers.
#include <functional>
#include <unordered_map>
#include <vector>


// Engine headers.
#include <Systems/IEngineInput.hpp>


// Engine namespace.
namespace water
{
    /// <summary>
    /// An input management system which utilises the raw-state functionality of SFML to obtain keyboard and controller input.
    /// </summary>
    class InputSFML final : public IEngineInput
    {
        public: 

            #pragma region Constructors and destructor

            InputSFML()                                     = default;
            InputSFML (InputSFML&& move);
            InputSFML& operator= (InputSFML&& move);

            ~InputSFML() override final {}
        
            InputSFML (const InputSFML& copy)               = delete;
            InputSFML& operator= (const InputSFML& copy)    = delete;

            #pragma endregion


            #pragma region System management

            /// <summary> Initialises the system, reserving memory for the containers. </summary>
            void initialise() override final;

            /// <summary> Updates the system, this refreshes the state of actions. </summary>
            void update() override final;

            #pragma endregion


            #pragma region Getters

            /// <summary> Checks whether a controller is connected. </summary>
            /// <param name="controller"> The index of the desired controller. </param>
            /// <returns> The connection status of the desired controller. </returns>
            bool isConnected (const unsigned int controller) const override final;

            /// <summary> Checks whether a controller supports the desired axis. </summary>
            /// <param name="controller"> The index of the desired controller. </param>
            /// <param name="axis"> The axis to check support for. </param>
            /// <returns> If the controller has the axis, false if the controller isn't connected. </returns>
            bool hasAxis (const unsigned int controller, const Axis axis) const override final;

            /// <summary> Checks how many buttons exist on the controller. </summary>
            /// <param name="controller"> The index of the desired controller. </param>
            /// <returns> The button count, zero if the controller doesn't exist. </returns>
            unsigned int getButtonCount (const unsigned int controller) const override final;

            /// <summary> Checks how many keys exist on the current keyboard. </summary>
            /// <returns> The key count. </returns>
            unsigned int getKeyCount() const override final;

            /// <summary> Checks if the action is currently pressed, this is specific to the current frame. </summary>
            /// <param name="id"> The ID of the action to check. </param>
            /// <returns> The status of the action, false if the action is invalid. </returns>
            bool getActionPressed (const int id) const override final;

            /// <summary> Checks if this is the first frame an action is no longer being held down. </summary>
            /// <param name="id"> The ID of the action to check. </param>
            /// <returns> The "up" status of the action, false if the action is invalid. </returns>
            bool getActionUp (const int id) const override final;

            /// <summary> Checks if this is the first frame an action is being pushed down. </summary>
            /// <param name="id"> The ID of the action to check. </param>
            /// <returns> The "down" status of the action, false if the action is invalid. </returns>
            bool getActionDown (const int id) const override final;

            /// <summary> Obtains the value of an axis action with the given ID. </summary>
            /// <param name="id"> The ID of the action to check. </param>
            /// <returns> The current value of the axis, zero if the action is invalid. </returns>
            float getActionAxis (const int id) const override final;

            #pragma endregion


            #pragma region Real-time state

            /// <summary> Performs a real-time check on the state of a key, this is accessing the raw data and could be slow. </summary>
            /// <param name="key"> The key to check the status of. </param>
            /// <returns> Whether the key is currently being pressed. </returns>
            bool isKeyPressed (const Key key) const override final;
        
            /// <summary> Performs a real-time check on the state of a button, this is accessing the raw data and could be slow. </summary>
            /// <param name="controller"> The index of the desired controller. </param>
            /// <param name="button"> The index of the desired button. </param>
            /// <returns> The status of the button, false if the controller or button are invalid. </returns>
            bool isButtonPressed (const unsigned int controller, const unsigned int button) const override final;

            /// <summary> Performs a real-time check on the position of an axis, this is accessing raw data and could be slow. </summary>
            /// <param name="controller"> The index of the desired controller. </param>
            /// <param name="axis"> The desired axis to obtain information for. </param>
            /// <returns> The position of the axis, zero if the controller is invalid. </returns>
            float axisPosition (const unsigned int controller, const Axis axis) const override final;

            #pragma endregion


            #pragma region Action management

            /// <summary> Adds a KeyboardKey action to the list of actions to check the status of. </summary>
            /// <param name="action"> The action to be added. </param>
            /// <returns> Whether the action was added successfully. </returns>
            bool addAction (const KeyboardKey& action) override final;
        
            /// <summary> Adds a ControllerButton action to the list of actions to check the status of. </summary>
            /// <param name="action"> The action to be added. </param>
            /// <returns> Whether the action was added successfully. </returns>
            bool addAction (const ControllerButton& action) override final;
        
            /// <summary> Adds a ControllerAxis action to the list of axis to check the status of. </summary>
            /// <param name="action"> The action to be added. The ID of the action must be unique here. </param>
            /// <returns> Whether the action was added successfully. </returns>
            bool addAction (const ControllerAxis& action) override final;

            /// <summary> Attempts to remove actions related to the given ID. </summary>
            /// <param name="id"> The ID of actions to be removed. </param>
            /// <param name="type"> Specifies the type of actions to remove, e.g. Key will remove all KeyboardKey actions for the ID. </param>
            void removeAction (const int id, const Action type = Action::All) override final;

            /// <summary> Completely clears the list of actions, all action checks from this point will be invalid. </summary>
            void removeActions() override final;

            #pragma endregion

        private:

            // Forward declarations.
            struct ActionState;

            #pragma region Internal workings

            /// <summary> A function which checks the state of a key and updates the associated ActionState as required. </summary>
            /// <param name="key"> The KeyboardKey object to process. </param>
            void processKey (const KeyboardKey& key);

            /// <summary> A function which checks the state of a button and updates the associated ActionState as required. </summary>
            /// <param name="button"> The ControllerButton object to process. </param>
            void processButton (const ControllerButton& button);

            /// <summary> A function which checks the state of an axis and updates the associated ActionState as required. </summary>
            /// <param name="axis"> The ControllerAxis object to process. </param>
            void processAxis (const ControllerAxis& axis);

            /// <summary> A function which sets the pressed, down and up values of an ActionState object. </summary>
            /// <param name="id"> The ID of the object to modify. </param>
            /// <param name="pressed"> If the state is pressed down. </param>
            void keyButtonAlgorithm (const int id, const bool pressed);

            /// <summary> Updates an ActionState with the desired function. This will throw exceptions if the state does not exist. </summary>
            /// <param name="id"> The ID of the state to update. </param>
            /// <param name="updateFunction"> The function to call when updating the ActionState. </param>
            void updateActionState (const int id, const std::function<void (ActionState&)>& updateFunction);

            /// <summary> Gets a member from an ActionState at the corresponding ID. </summary>
            /// <param name="id"> The ID of the ActionState to access. </param>
            /// <param name="getMember"> The function to call to retrieve the member. </param>
            /// <param name="T"> The return type of the function. </param>
            /// <returns> The member or zero. </returns>
            template <typename T, typename U> T getActionMember (const int id, const U& getMember) const;

            /// <summary> Searches through a given vector swap'n'popping all actions with a matching ID value. </summary>
            /// <param name="id"> The ID of actions to remove. </param>
            /// <param name="actions"> The vector of actions to search through. </param>
            template <typename T> void removeActionByID (const int id, std::vector<T>& actions);

            /// <summary> Increments or decrements the reference counter of the desired action. </summary>
            /// <param name="id"> The ID of the ActionState. </param>
            /// <param name="increment"> Determines whether the counter is incremented or decremented. </param>
            /// <returns> Whether the action still has references or not. </returns>
            bool updateActionReferenceCount (const int id, const bool increment);

            #pragma endregion
            

            #pragma region Implementation data
            
            /// <summary> A structure containing the pressed, down and up state of actions. </summary>
            struct ActionState final
            {
                unsigned int    frame       { 0 };      //!< The frame at which the information was last updated.
                bool            previous    { false };  //!< The previous pressed value.
                bool            pressed     { false };  //!< Whether the action is currently pressed.
                bool            down        { false };  //!< Whether the action has just been pressed down.
                bool            up          { false };  //!< Whether the action has just been released.
                float           axis        { 0.f };    //!< Used for axis actions, this allows us to not require a separate map.
                unsigned int    references  { 0 };      //!< Monitoring the number of references to an action allows us to know if we can remove it or not.

                /// <summary> Resets the action if the given frame number doesn't match the stored frame. </summary>
                void requestReset (const unsigned int currentFrame)
                {
                    if (currentFrame != frame)
                    {
                        frame    = currentFrame;
                        previous = pressed;
                        pressed  = false;
                        down     = false;
                        up       = true;    // Set this to true so we can take into account the fact that not all bindings may have let go.
                        axis    = 0.f;
                    }
                }
            };


            unsigned int                            m_frame     { 0 };  //!< The current frame counter, allows us to know when ActionState objects should be reset.

            std::unordered_map<int, ActionState>    m_actions   { };    //!< The monitored actions and their associated state.
            
            std::vector<KeyboardKey>                m_keys      { };    //!< A container of key actions to be checked.
            std::vector<ControllerButton>           m_buttons   { };    //!< A container of button actions to be checked.
            std::vector<ControllerAxis>             m_axes      { };    //!< A container of axis actions to be checked.

            #pragma endregion
    };
}


#endif