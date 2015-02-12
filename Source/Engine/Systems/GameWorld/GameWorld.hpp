#if !defined WATER_GAME_WORLD_INCLUDED
#define WATER_GAME_WORLD_INCLUDED


// STL headers.
#include <stack>
#include <unordered_map>


// Engine headers.
#include <Systems/IEngineGameWorld.hpp>


// Engine namespace
namespace water
{
    /// <summary> 
    /// A stack-based state system which controls the flow of the game world. Games are created using
    /// </summary>
    class GameWorld final : public IEngineGameWorld
    {
        public:
        
            #pragma region Constructors and destructor

            GameWorld()                                     = default;
            GameWorld (GameWorld&& move);
            GameWorld& operator= (GameWorld&& move);

            ~GameWorld() override final;

            GameWorld (const GameWorld& copy)               = delete;
            GameWorld& operator= (const GameWorld& copy)    = delete;

            #pragma endregion


            #pragma region System management

            /// <summary> Perform a physics update on the active state. </summary>
            void updatePhysics() override final;

            /// <summary> Perform an update call on the active state. </summary>
            void update() override final;

            /// <summary> Render the active state. </summary>
            void render() override final;

            /// <summary> Will exit the game world if requested, popping and removing all states. </summary>
            void endFrame() override final;

            #pragma endregion


            #pragma region Getters

            /// <summary> Tests if the stack is empty. If so then the engine knows that it can exit. </summary>
            /// <returns> Whether the stack is empty. </returns>
            bool isStackEmpty() const override final    { return m_stack.empty(); }

            /// <summary> Obtain the collection of physics objects in the active state. </summary>
            /// <returns> The vector of objects. </returns>
            const std::vector<PhysicsObject*>& getPhysicsObjects() const override final;

            #pragma endregion


            #pragma region State management

            /// <summary> Informs the game world to close after the current frame. This pops all states, removes all states and then exits the game. </summary>
            void requestExit() override final  { m_exit = true; }

            /// <summary> Adds a game state to the world, this allows it to be accessible.</summary>
            /// <param name="id"> The unique identifier of the state. If a state with this ID already exists it will be ignored. </param>
            /// <param name="state"> The state to add. </param>
            /// <returns> True if successful, false if the ID is already in use or the state doesn't exist. </returns>
            bool addState (const int id, const std::shared_ptr<GameState>& state) override final;

            /// <summary> Removes a state from the game world, this will cause it to be inaccessible from now on. </summary>
            /// <param name="id"> The unique identifier of the state to remove. </param>
            /// <returns> If the ID is invalid this will return false. This will return the states onRemove() value. </returns>
            bool removeState (const int id) override final;

            /// <summary> Request a state to be pushed onto the top of the state stack, therefore becoming the active state. </summary>
            /// <param name="id"> The unique identifier of the state to push. </param>
            /// <returns> Whether the action could be performed. </returns>
            bool push (const int id) override final;

            /// <summary> 
            /// Request the active state be popped from the top of the state stack. The previously push state will become active. 
            /// If this results in an empty state stack then the game will close.
            /// </summary>
            void pop() override final;

            /// <summary> Swaps the active state with another, this pops the active state but doesn't reactivate the previous state. </summary>
            /// <param name="id"> The identifier of the state to swap to. </param>
            /// <returns> Whether the swap was successful. </returns>
            bool swap (const int id) override final;

            #pragma endregion

        private:

            #pragma region Internal workings

            /// <summary> Pops every state on the stack and removes every state from the map. </summary>
            void clearWorld();

            #pragma endregion

            #pragma region Implementation data

            std::unordered_map<int, std::shared_ptr<GameState>> m_states    { };        //!< A map of game states with a unique ID, this is how states are accessed externally.
            std::stack<std::shared_ptr<GameState>>              m_stack     { };        //!< The active stack of game states. Only to top-most state is updated every frame.
            bool                                                m_exit      { false };  //!< A flag to determine whether the game world should clean up after itself.

            #pragma endregion
    };
}

#endif