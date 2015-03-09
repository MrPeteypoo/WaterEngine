#if !defined WATER_LOGGER_HAPI_INCLUDED
#define WATER_LOGGER_HAPI_INCLUDED


// Engine headers.
#include <Systems/IEngineLogger.hpp>


// Engine namespace.
namespace water
{
    /// <summary> 
    /// A simple wrapper for the HAPI libraries logging features. Unfortunately the output location for logging
    /// can't be changed so that functionality is redundant. 
    /// </summary>
    class LoggerHAPI final : public IEngineLogger
    {
        public:

            #pragma region Constructors and destructor
            
            LoggerHAPI()                                    = default;
            LoggerHAPI (const LoggerHAPI& copy)             = default;
            LoggerHAPI& operator= (const LoggerHAPI& copy)  = default;

            LoggerHAPI (LoggerHAPI&& move);
            LoggerHAPI& operator= (LoggerHAPI&& move);
            ~LoggerHAPI() override final { }        

            #pragma endregion
            

            #pragma region System management
            
            /// <summary> Initialise the logger so that its ready for logging. </summary>
            /// <param name="file"> The file to log messages to. </param>
            /// <param name="timestamp"> Whether message output should show a timestamp or not. </param>
            /// <returns> Whether the file and logger was successfully initialised. </returns>
            bool initialise (const std::string& file, const bool timestamp) override final;

            /// <summary> Doesn't do anything. </summary>
            void update() override final;

            /// <summary> Changes the location the logger will write to. </summary>
            /// <param name="newFile"> The destination of the file to log messages to from now on. </param>
            /// <returns> Whether the file was successfully initialised. </returns>
            bool changeLogDestination (const std::string& newFile) override final { return false; }

            #pragma endregion


            #pragma region Logging

            /// <summary> Passes a message to LoggerHAPI::logMessage(). </summary>
            /// <param name="message"> The text to log. </param>
            /// <returns> Whether the message was successfully logged to the desired file. </returns>
            bool log (const std::string& message) override final;

            /// <summary> Attaches a warning message and passes the desired message to LoggerHAPI::logMessage(). </summary>
            /// <param name="message"> The text to log. </param>
            /// <returns> Whether the message was successfully logged to the desired file. </returns>
            bool logWarning (const std::string& message) override final;

            /// <summary> Attaches an error message and passes the desired message to LoggerHAPI::logMessage(). </summary>
            /// <param name="message"> The text to log. </param>
            /// <returns> Whether the message was successfully logged to the desired file. </returns>
            bool logError (const std::string& message) override final;

            #pragma endregion

        private:

            /// <summary> Performs the actual logging action with the given message. </summary>
            /// <param name="message"> The message to log. </param>
            /// <returns> Whether the log was successful or not. </returns>
            bool logMessage (const std::string& message);


            bool m_timestamp { false }; //!< Whether the logger should feature a timestamp next to logs.
    };
}

#endif