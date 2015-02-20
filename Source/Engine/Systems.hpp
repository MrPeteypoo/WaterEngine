#if !defined WATER_SYSTEMS_INCLUDED
#define WATER_SYSTEMS_INCLUDED


// Engine headers.
#include <Interfaces/IAudio.hpp>
#include <Interfaces/IGameWorld.hpp>
#include <Interfaces/IInput.hpp>
#include <Interfaces/ILogger.hpp>
#include <Interfaces/IPhysics.hpp>
#include <Interfaces/IRenderer.hpp>
#include <Interfaces/ITime.hpp>


// Engine namespace.
namespace water
{
    // Forward declarations.
    class Engine;

    /// <summary> 
    /// A static class which can be used to manage the hot-swapping and retrieval of systems. This allows systems
    /// to be changed at run-time without impacting on the workings of the game and other systems. If systems have
    /// not been set then this WILL cause access violation errors.
    /// </summary>
    class Systems final
    {
        public:

            static IAudio&      audio()                                 { return *m_audio; }
            static IGameWorld&  gameWorld()                             { return *m_gameWorld; }
            static IInput&      input()                                 { return *m_input; }
            static ILogger&     logger()                                { return *m_logger; }
            static IPhysics&    physics()                               { return *m_physics; }
            static IRenderer&   renderer()                              { return *m_renderer; }
            static ITime&       time()                                  { return *m_time; }
      
        private:

            /// We let the water::Engine class be a friend so that it can set the systems.
            friend class water::Engine;

            static void         setAudio (IAudio* const system)         { m_audio = system; }
            static void         setGameWorld (IGameWorld* const system) { m_gameWorld = system; }
            static void         setInput (IInput* const system)         { m_input = system; }
            static void         setLogger (ILogger* const system)       { m_logger = system; }
            static void         setPhysics (IPhysics* const system)     { m_physics = system; }
            static void         setRenderer (IRenderer* const system)   { m_renderer = system; }
            static void         setTime (ITime* const system)           { m_time = system; }

            static IAudio*      m_audio;        //!< An audio system used for playing and manipulating sounds.
            static IGameWorld*  m_gameWorld;    //!< The game world system, allows manipulation of the game flow.
            static IInput*      m_input;        //!< An input system for obtaining abstracted input.
            static ILogger*     m_logger;       //!< The logger to be used for logging debug, warning or error messages.
            static IPhysics*    m_physics;      //!< The physics system used for collision detection by games.
            static IRenderer*   m_renderer;     //!< The renderering system which is used for drawing.
            static ITime*       m_time;         //!< The time system which keeps track of delta time values.
    };
}

#endif