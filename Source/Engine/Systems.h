#if !defined WATER_SYSTEMS_INCLUDED
#define WATER_SYSTEMS_INCLUDED


// Engine headers.
#include <Engine/EngineForward.h>


// Engine namespace.
namespace water
{
    /// <summary> 
    /// A static class which can be used to manage the hot-swapping and retrieval of systems. This allows systems
    /// to be changed at run-time without impacting on the workings of the game and other systems. If systems have
    /// not been set then this WILL cause access violation errors.
    /// </summary>
    class Systems final
    {
        public:

            static IAudio&      getAudio()                              { return *m_audio; }
            static IInput&      getInput()                              { return *m_input; }
            static ILogger&     getLogger()                             { return *m_logger; }
            static IRenderer&   getRenderer()                           { return *m_renderer; }
            static ITime&       getTime()                               { return *m_time; }

            static void         setAudio (IAudio* const system)         { m_audio = system; }
            static void         setInput (IInput* const system)         { m_input = system; }
            static void         setLogger (ILogger* const system)       { m_logger = system; }
            static void         setRenderer (IRenderer* const system)   { m_renderer = system; }
            static void         setTime (ITime* const system)           { m_time = system; }
      
        private:

            static IAudio*      m_audio;    //!< An audio system used for playing and manipulating sounds.
            static IInput*      m_input;    //!< An input system for obtaining abstracted input.
            static ILogger*     m_logger;   //!< The logger to be used for logging debug, warning or error messages.
            static IRenderer*   m_renderer; //!< The renderering system which is used for drawing.
            static ITime*       m_time;     //!< The time system which keeps track of delta time values.
    };
}

#endif