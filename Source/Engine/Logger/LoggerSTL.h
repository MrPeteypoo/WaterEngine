#if !defined WATER_LOGGER_STL_INCLUDED
#define WATER_LOGGER_STL_INCLUDED


// Engine headers.
#include <ILogger.h>


// Engine namespace.
namespace water
{
    class LoggerSTL final : public ILogger
    {
        public:

            #pragma region Constructors and destructor
            
            LoggerSTL();
            LoggerSTL (LoggerSTL&& move);
            LoggerSTL& operator= (LoggerSTL&& move);

            ~LoggerSTL() override final;
        
            LoggerSTL (const LoggerSTL& copy)               = delete;
            LoggerSTL& operator= (const LoggerSTL& copy)    = delete;

            #pragma endregion

            
            #pragma region initialisation 
            
            /// <summary> Initialise the logger so that its ready for logging. </summary>
            /// <param name="file"> The file to log messages to. </param>
            /// <returns> Whether the file and logger was successfully initialised. </returns>
            bool initialise (const std::string& file) override final;

            /// <summary> Changes the location the logger will write to. </summary>
            /// <param name="newFile"> The destination of the file to log messages to from now on. </param>
            /// <returns> Whether the file was successfully initialised. </returns>
            bool changeLogDestination (const std::string& newFile) override final;

            #pragma endregion


            #pragma region Logging

            /// <summary> Passes a message to LoggerSTL::logMessage(). </summary>
            /// <param name="message"> The text to log. </param>
            /// <returns> Whether the message was successfully logged to the desired file. </returns>
            bool log (const std::string& message) override final;

            /// <summary> Attaches a warning message and passes the desired message to LoggerSTL::logMessage(). </summary>
            /// <param name="message"> The text to log. </param>
            /// <returns> Whether the message was successfully logged to the desired file. </returns>
            bool logWarning (const std::string& message) override final;

            /// <summary> Attaches an error message and passes the desired message to LoggerSTL::logMessage(). </summary>
            /// <param name="message"> The text to log. </param>
            /// <returns> Whether the message was successfully logged to the desired file. </returns>
            bool logError (const std::string& message) override final;

            /// <summary> Displays a message box to the user with the contained message. </summary>
            /// <param name="message"> The desired message. </param>
            /// <param name="title"> The title of the message. </param>
            void displayMessage (const std::string& title, const std::string& message);

            #pragma endregion

        private:

            /// <summary> Performs the actual logging action with the given message. </summary>
            /// <param name="message"> The message to log. </param>
            /// <returns> Whether the log was successful or not. </returns>
            bool logMessage (const std::string& message);


            // Forward declarations.
            struct Impl;


            Impl* m_impl { nullptr };   //!< A pointer to the implementation data.
    };
}

#endif