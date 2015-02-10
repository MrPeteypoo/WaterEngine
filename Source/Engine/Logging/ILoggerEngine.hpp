#if !defined WATER_INTERFACE_LOGGER_ENGINE_INCLUDED
#define WATER_INTERFACE_LOGGER_ENGINE_INCLUDED


// Engine headers.
#include <ILogger.hpp>


// Engine namespace.
namespace water
{
    /// <summary>
    /// An interface for the engine to perform system-critical functions to ILogger systems.
    /// </summary>
    class ILoggerEngine : public ILogger
    {
        public:

            // Ensure destructor is virtual since this is an interface.
            virtual ~ILoggerEngine() {}


            #pragma region System management
            
            /// <summary> Initialise the logger so that its ready for logging. </summary>
            /// <param name="file"> The file to log messages to. </param>
            /// <returns> Whether the file and logger was successfully initialised. </returns>
            virtual bool initialise (const std::string& file) = 0;

            /// <summary> Updates the logger system. </summary>
            virtual void update() = 0;

            /// <summary> Changes the location the logger will write to. </summary>
            /// <param name="newFile"> The destination of the file to log messages to from now on. </param>
            /// <returns> Whether the file was successfully initialised. </returns>
            virtual bool changeLogDestination (const std::string& newFile) = 0;

            #pragma endregion
    };   
}

#endif