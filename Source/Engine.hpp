#if !defined WATER_ENGINE_INCLUDED
#define WATER_ENGINE_INCLUDED


// STL headers.
#include <string>


/// <summary>
/// The namespace of every aspect of the water engine. This includes renderering systems, audio systems, logging systems,
/// input systems, etc. Everything that is required to make a simple 2D game.
/// </summary>
namespace water
{
    // Forward declarations.
    struct Configuration;
    class IEngineAudio;
    class IEngineGameWorld;
    class IEngineInput;
    class IEngineLogger;
    class IEnginePhysics;
    class IEngineRenderer;
    class IEngineTime;
    class IGameWorld;


    /// <summary>
    /// The Engine class is the entry point for game application that want to use the water engine as the basis for their game.
    /// Water provides simple interfaces which grant access to the core systems a game needs to function. The water engine uses
    /// a state system which allows states to be stacked on top of each other, allowing for easy manipulation of game flow.
    /// </summary>
    class Engine final
    {
        public:

            /////////////////////////////////
            // Constructors and destructor //
            /////////////////////////////////

            Engine();
            Engine (Engine&& move);
            Engine& operator= (Engine&& move);
            ~Engine();

            Engine (const Engine& copy)             = delete;
            Engine& operator= (const Engine& copy)  = delete;


            ///////////////////////
            // Engine management //
            ///////////////////////

            /// <summary>
            /// Attempt to load the engine from the default configuration. This may not provide suitable results but it is great for getting
            /// to grips with the engine.
            /// </summary>
            /// <returns> Whether the initialisation was successful or not. If false then the game should close. </returns>
            bool initialise();

            /// <summary>
            /// Attempt to load the engine from a XML file. If this is unsuccesful then the engine will load from a default configuration.
            /// </summary>
            /// <param name="file"> The location of the .xml file to load from. </param>
            /// <returns> Whether the initialisation was successful or not. If false then the game should close. </returns>
            bool initialise (const std::string& file);

            /// <summary>
            /// Attempt to load the engine from a given configuration. This will configure all of the systems and prepare the
            /// engine for usage by the game.
            /// </summary>
            /// <param name="config"> The configuration to load. </param>
            /// <returns> Whether the initialisation was successful or not. If false then the game should close. </returns>
            bool initialise (const Configuration& config);

            /// <summary>
            /// Run the engine, this will start the game loop and run the game world.
            /// </summary>
            void run();


            /////////////
            // Getters //
            /////////////

            /// <summary> Obtains a reference to the game world so that the game may prepare before running the engine. </summary>
            /// <returns> The game world. </returns>
            IGameWorld& getGameWorld() const;

        private:


            ///////////////////////
            // Internal workings //
            ///////////////////////

            /// <summary> Stops and deletes all systems. </summary>
            void clean();

            /// <summary> Checks the validity of the systems data in the given config file and attempts to load the specified systems. </summary>
            /// <param name="config"> The configuration data to create the systems with. </param>
            /// <returns> Whether the systems could be determined and created. </returns>
            bool createSystems (const Configuration& config);

            /// <summary> Attempts to initialise the logger so that we can log any errors that occur during the initialisation of other systems. </summary>
            /// <param name="config"> The configuration containing initialisation data for the logger. </param>
            /// <returns> Whether the initialisation was successful or not.
            bool initialiseLogger (const Configuration& config);

            /// <summary> This will initialise every system in the engine excluding the logger. That should be initialised separately. </summary>
            /// <param name="config"> The configuration containing initialisation data for every system. </param>
            void initialiseSystems (const Configuration& config);

            /// <summary> Sets each system in the Systems service locator. </summary>
            void setSystems();


            /////////////////////////
            // Implementation data //
            /////////////////////////

            IEngineAudio*       m_audio     { nullptr };    //!< The audio system used for playing audio.
            IEngineGameWorld*   m_gameWorld { nullptr };    //!< A state manager used to control the flow of the game.
            IEngineInput*       m_input     { nullptr };    //!< An input system, the main port of call for user interaction.
            IEngineLogger*      m_logger    { nullptr };    //!< The logging system used for logging messages throughout the engine and game.
            IEnginePhysics*     m_physics   { nullptr };    //!< The physics system used by the engine.
            IEngineRenderer*    m_renderer  { nullptr };    //!< The renderering system used for drawing onto the screen.
            IEngineTime*        m_time      { nullptr };    //!< The time system used for maintaining the game loop and delta time.

            bool                m_ready     { false };      //!< A flag to indicate whether the engine is ready to run or not.
    };
}

#endif
