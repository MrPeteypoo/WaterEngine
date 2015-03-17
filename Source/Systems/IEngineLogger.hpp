#if !defined WATER_INTERFACE_LOGGER_ENGINE_INCLUDED
#define WATER_INTERFACE_LOGGER_ENGINE_INCLUDED


// Engine headers.
#include <Interfaces/ILogger.hpp>


// Engine namespace.
namespace water
{
    /// <summary>
    /// An interface for the engine to perform system-critical functions to ILogger systems.
    /// </summary>
    class IEngineLogger : public ILogger
    {
        public:

            // Ensure destructor is virtual since this is an interface.
            virtual ~IEngineLogger() override {}


            ///////////////////////
            // System management //
            ///////////////////////

            /// <summary> Initialise the logger so that its ready for logging. </summary>
            /// <param name="file"> The file to log messages to. </param>
            /// <param name="timestamp"> Whether message output should show a timestamp or not. </param>
            /// <returns> Whether the file and logger was successfully initialised. </returns>
            virtual bool initialise (const std::string& file, const bool timestamp) = 0;

            /// <summary> Updates the logger system. </summary>
            virtual void update() = 0;

            /// <summary> Changes the location the logger will write to. </summary>
            /// <param name="newFile"> The destination of the file to log messages to from now on. </param>
            /// <returns> Whether the file was successfully initialised. </returns>
            virtual bool changeLogDestination (const std::string& newFile) = 0;
    };
}

#endif
