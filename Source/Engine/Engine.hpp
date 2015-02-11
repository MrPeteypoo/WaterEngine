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
    class IAudioEngine;
    class ILoggerEngine;
    class IRendererEngine;
    class ITimeEngine;


    /// <summary>
    /// The Engine class is the entry point for game application that want to use the water engine as the basis for their game.
    /// Water provides simple interfaces which grant access to the core systems a game needs to function. The water engine uses
    /// a state system which allows states to be stacked on top of each other, allowing for easy manipulation of game flow.
    /// </summary>
    class Engine final
    {
        public:

            // Forward declarations.
            class StateManager;

            #pragma region Constructors and destructor

            Engine();            
            Engine (Engine&& move);
            Engine& operator= (Engine&& move);
            ~Engine();

            Engine (const Engine& copy)             = delete;
            Engine& operator= (const Engine& copy)  = delete;

            #pragma endregion

            
            #pragma region Getters and setters

            /// <summary> 
            /// Obtains a reference to the state manager. This allows games to add states to the engine so the game can run.
            /// </summary>
            /// <returns> The state manager. </returns>
            StateManager& getStateManager() const   { return *m_states; }

            #pragma endregion

            
            #pragma region Engine management

            /// <summary> 
            /// Attempt to load the engine from a file. This should be a .xml file. This will configure all of the systems
            /// and prepare the engine for usage by the game. 
            /// </summary>
            /// <param name="file"> The XML file to load from. </param>
            /// <returns> Whether the initialisation was successful or not. If false then the game should close. </returns>
            bool initialiseFromFile (const std::string& file);

            /// <summary>
            /// Run the engine, this will start the game loop and run the game.
            /// </summary>
            void run();

            /// <summary>
            /// Forcibly closes the engine which will close the active application.
            /// </summary>
            void close()    { m_close = true; }

            #pragma endregion

        private:

            #pragma region Internal workings

            /// <summary> Stops and deletes all systems. </summary>
            void clean();

            /// <summary> Sets each system in the Systems service locator. </summary>
            void setSystems();

            #pragma endregion


            #pragma region Implementation data

            // Allow the EngineBuilder to be a friend for easy modification of the engine.
            friend class EngineBuilder;

            IAudioEngine*       m_audio     { nullptr };    //!< The audio system used for playing audio.
            ILoggerEngine*      m_logger    { nullptr };    //!< The logging system used for logging messages throughout the engine and game.
            IRendererEngine*    m_renderer  { nullptr };    //!< The renderering system used for drawing onto the screen.
            ITimeEngine*        m_time      { nullptr };    //!< The time system used for maintaining the game loop and delta time.
            StateManager*       m_states    { nullptr };    //!< A state manager to control the flow of the game.

            bool                m_ready     { false };      //!< A flag to indicate whether the engine is ready to run or not.
            bool                m_close     { false };      //!< Used to externally close the engine.

            #pragma endregion
    };
}

#endif