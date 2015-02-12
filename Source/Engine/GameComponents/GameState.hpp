#if !defined WATER_GAME_STATE_INCLUDED
#define WATER_GAME_STATE_INCLUDED


// STL headers.
#include <vector>


// Engine namespace.
namespace water
{
    // Forward declarations. 
    class IGameWorld;
    class Systems;
    class PhysicsObject;


    /// <summary>
    /// An abstract class which represents every state in the game. A state is effectively a clearly defined part of a game, typically a piece of a game with
    /// a single specific purpose. Examples would be a main menu, a level, a status screen, a world map, etc. Games made with the water engine must use the 
    /// GameState class to control the flow of the game.
    ///
    /// The way physics integrates with the game requires every engine-managed object with collision detection must both derive from the PhysicsObject class
    /// and must be enabled using GameState::addPhysicsObject(). Without this physicsUpdate() won't be called and no collision detection will occur. Also if 
    /// you ever need to delete a PhysicsObject you must call GameState::removePhysicsObject() otherwise access violation errors will occur in the physics 
    /// system.
    /// </summary>
    class GameState
    {
        public:

            /// <summary> Call this in child classes to ensure the state reserves space for PhysicsObject's. This increases efficiency when adding. </summary>
            /// <param name="size"> How many elements are expected to be held by the state. </param>
            GameState (const unsigned int elementCount = 100);

            // Ensure virtual destructor of the abstract class.
            virtual ~GameState() { };


            #pragma region Game flow

            /// <summary> Called when the state is added to the IGameWorld. Effectively a constructor. </summary>
            /// <returns> Whether an error has occurred. The state will not be added if this returns false. </returns>
            virtual bool onAdd() = 0;

            /// <summary> Called when the state is removed from the IGameWorld. Effectively a destructor. </summary>
            /// <returns> If something went wrong then false would inform the IGameWorld to report an error. </returns>
            virtual bool onRemove() = 0;

            /// <summary> Called when the state is becoming the active state. </summary>
            virtual void onEntry() = 0;

            /// <summary> Called when the state is becoming inactive. </summary>
            virtual void onExit() = 0;

            /// <summary> Requests that the state should perform a physics update, this only occurs when the state is active. </summary>
            virtual void updatePhysics() = 0;

            /// <summary> Requests that the state should update, this only occurs when the state is active. </summary>
            virtual void update() = 0;

            /// <summary> Requests that the state should render, this only occurs when the state is active. </summary>
            virtual void render() = 0;

            #pragma endregion

        protected:

            #pragma region Physics management

            /// <summary>
            /// Adds a PhysicsObject to the list of objects which should be managed by the physics system. This means they will receive collsion detection.
            /// Ensure you're never adding an object which already exists.
            /// </summary>
            /// <param name="object"> The PhysicsObject to add, nullptr will be ignored. </param>
            void addPhysicsObject (PhysicsObject* const object);

            /// <summary>
            /// Similar to addPhysicsObject(), the only difference is that the state will check the list beforehand to ensure the object isn't already on
            /// the list.
            /// </summary>
            void addUniquePhysicsObject (PhysicsObject* const object);

            /// <summary>
            /// Removes a PhysicsObject from the list of physics-managed objects. This should ALWAYS be done before deleting a PhysicsObject otherwise
            /// dangling pointers will cause the physics loop to crash the game.
            /// </summary>
            /// <param name="object"> The object to remove, nullptr will be ignored. </param>
            void removePhysicsObject (PhysicsObject* const object);

            /// <summary>
            /// Requests that all stored objects be removed. This is an effective method of cleaning the physics system. 
            /// </summary>
            void removePhysicsObjects();

            #pragma endregion


            #pragma region Utility functions

            /// <summary> Obtain a reference to the active IGameWorld. </summary>
            /// <returns> The game world. </returns>
            IGameWorld& gameWorld() const;

            #pragma endregion

        private:

            #pragma region Physics integration
            
            // Let IGameWorld manage interaction with the physics system.
            friend class GameWorld;

            /// <summary> Attempts to find the given PhysicsObject in the vector of objects. Don't make it const otherwise the pointer becomes const. </summary>
            /// <param name="object"> The object to search for. </param>
            /// <returns> The iterator of the vector for the object. </returns>
            std::vector<PhysicsObject*>::iterator findObject (const PhysicsObject* const object);

            /// <summary> Obtains the collection of PhysicsObject's in the state. </summary>
            /// <returns> A reference to the state-contained vector. </returns>
            const std::vector<PhysicsObject*>& getPhysicsObjects() const    { return m_objects; }

            std::vector<PhysicsObject*> m_objects   { };    //!< A collection of PhysicsObject's to be managed by the physics system.

            #pragma endregion
    };
}


#endif