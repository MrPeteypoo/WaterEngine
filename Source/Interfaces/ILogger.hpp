#if !defined WATER_INTERFACE_LOGGER_INCLUDED
#define WATER_INTERFACE_LOGGER_INCLUDED


// STL headers.
#include <string>


// Engine namespace.
namespace water
{
    /// <summary>
    /// An interface to every logging system in the water engine. This can be used to log errors, warnings or whatever
    /// else the programmer decides needs to be logged.
    /// </summary>
    class ILogger
    {
        public:

            // Ensure destructor is virtual since this is an interface.
            virtual ~ILogger() {}


            /////////////
            // Logging //
            /////////////

            /// <summary> Log an informative message. </summary>
            /// <param name="message"> The text to log. </param>
            /// <returns> Whether the message was successfully logged to the desired file. </returns>
            virtual bool log (const std::string& message) = 0;

            /// <summary> Log a warning message. </summary>
            /// <param name="message"> The text to log. </param>
            /// <returns> Whether the message was successfully logged to the desired file. </returns>
            virtual bool logWarning (const std::string& message) = 0;

            /// <summary> Log an error message. </summary>
            /// <param name="message"> The text to log. </param>
            /// <returns> Whether the message was successfully logged to the desired file. </returns>
            virtual bool logError (const std::string& message) = 0;
    };
}

#endif
