#if !defined WATER_INTERFACE_GAME_OBJECT_INCLUDED
#define WATER_INTERFACE_GAME_OBJECT_INCLUDED


// Engine namespace.
namespace water
{
    /// <summary>
    /// A basic interface for objects in the game. This is more of a convenience interface, it does not need to be used in the engine.
    /// </summary>
    class IGameObject
    {
        public:

            // Ensure destructor is virtual as this is an interface.
            virtual ~IGameObject() {}


            /////////////////
            /// Game flow ///
            /////////////////

            /// <summary> Initialises the object, this should be performed during load or creation. </summary>
            /// <returns> Whether the initialisation was successful. </returns>
            virtual bool initialise() = 0;

            /// <summary> This is where all physics operations should be performed. The delta time value here will be fixed every frame. </summary>
            virtual void updatePhysics() = 0;

            /// <summary> This is where all non-physics operations should be performed. </summary>
            virtual void update() = 0;

            /// <summary> The game object should render itself onto the screen here. </summary>
            virtual void render() = 0;
    };
}

#endif
