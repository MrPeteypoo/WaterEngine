#if !defined WATER_LOGGER_STL_INCLUDED
#define WATER_LOGGER_STL_INCLUDED


// Engine headers.
#include <Logging/ILoggerEngine.h>


// Engine namespace.
namespace water
{
    /// <summary>
    /// This logger uses STL implementation to provide logging functionality. This means it's cross-platform and
    /// doesn't rely on any external library.
    /// </summary>
    class LoggerSTL final : public ILoggerEngine
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

            
            #pragma region System management
            
            /// <summary> Initialise the logger so that its ready for logging. </summary>
            /// <param name="file"> The file to log messages to. </param>
            /// <returns> Whether the file and logger was successfully initialised. </returns>
            bool initialise (const std::string& file) override final;

            /// <summary> Doesn't do anything. </summary>
            void update() override final { }

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

            #pragma endregion

        private:

            /// <summary> Performs the actual logging action with the given message. </summary>
            /// <param name="message"> The message to log. </param>
            /// <returns> Whether the log was successful or not. </returns>
            bool logMessage (const std::string& message);

            #pragma region File handling

            /// <summary> Clears a file of its contents, fresh for modification. </summary>
            void clearFile (const std::string& name);

            /// <summary> Attemps to open a new file stream with the given filename. </summary>
            bool outputToStream (const std::string& name, const std::string& output);
            
            /// <summary> Closes the current file stream by injecting the footer.
            void closeCurrentStream();

            /// <summary> Get the log header used for the HTML document. </summary>
            /// <returns> The header part of the HTML. </returns>
            std::string getLogHeader() const;

            /// <summary> Gets the HTML footer for the log file. </summary>
            /// <returns> The footer part of the HTML. </returns>
            std::string getLogFooter() const;

            #pragma endregion


            // Forward declarations.
            struct Impl;


            Impl* m_impl { nullptr };   //!< A pointer to the implementation data.
    };
}

#endif