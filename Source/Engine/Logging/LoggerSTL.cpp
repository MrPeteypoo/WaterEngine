#include "LoggerSTL.hpp"


// STL headers.
#include <fstream>


// Engine headers.
#include <Utility/Time.hpp>


// Engine namespace.
namespace water
{
    #pragma region Implmentation data

    struct LoggerSTL::Impl final
    {
        std::fstream    m_file      { };        //!< The file stream used for logging messages.
        std::string     m_filename  { };        //!< The file name used for in the file stream.
        bool            m_timestamp { false };  //!< Determines whether a timestamp should be displayed before each logged message.
    };

    #pragma endregion


    #pragma region Constructors and destructor
            
    LoggerSTL::LoggerSTL()
    {
        m_impl = new Impl();
    }

    
    LoggerSTL::LoggerSTL (LoggerSTL&& move)
    {
        *this = std::move (move);
    }


    LoggerSTL& LoggerSTL::operator= (LoggerSTL&& move)
    {
        if (this != &move)
        {
            if (m_impl)
            {
                delete m_impl;
            }

            m_impl = move.m_impl;
            move.m_impl = nullptr;
        }

        return *this;
    }

    LoggerSTL::~LoggerSTL()
    {
        // Make sure we close the file stream and delete the implementation data.
        if (m_impl)
        {
            // Ensure the log contains valid HTML.
            closeCurrentStream();

            delete m_impl;
            m_impl = nullptr;
        }
    }

    #pragma endregion

            
    #pragma region System management 

    bool LoggerSTL::initialise (const std::string& file, const bool timestamp)
    {
        // Enable the timestamp functionality.
        m_impl->m_timestamp = timestamp;

        // Let the openNewStream function handle it.
        clearFile (file);
        if (outputToStream (file, getLogHeader()))
        {
            m_impl->m_filename = file;
            return true;
        }

        return false;
    }


    void LoggerSTL::update()
    {
        // TODO: Implement me bruv!
    }


    bool LoggerSTL::changeLogDestination (const std::string& newFile)
    {
        // We need to ensure the current file has valid HTML and attempt to open the new file.
        closeCurrentStream();

        clearFile (newFile);
        if (outputToStream (newFile, getLogHeader()))
        {
            m_impl->m_filename = newFile;
            return true;
        }

        return false;
    }

    #pragma endregion


    #pragma region Logging

    bool LoggerSTL::log (const std::string& message)
    {
        // Make information logs green.
        const auto& output = "<font color=\"#00ff00\">Info: " + message + "</font><br />";

        // Add the timestamp if necessary.
        return m_impl->m_timestamp ? 
                                    outputToStream (m_impl->m_filename, timestampMessage (output)) :
                                    outputToStream (m_impl->m_filename, output);
    }


    bool LoggerSTL::logWarning (const std::string& message)
    {
        // Make warnings amber.
        const auto& output = "<font color=\"#ffbf00\">Warning: " + message + "</font><br />";

        // Add the timestamp if necessary.
        return m_impl->m_timestamp ? 
                                    outputToStream (m_impl->m_filename, timestampMessage (output)) :
                                    outputToStream (m_impl->m_filename, output);
    }

    

    bool LoggerSTL::logError (const std::string& message)
    {
        // Make errors red.
        const auto& output = "<font color=\"#ff0000\">Error: " + message + "</font><br />";

        // Add the timestamp if necessary.
        return m_impl->m_timestamp ? 
                                    outputToStream (m_impl->m_filename, timestampMessage (output)) :
                                    outputToStream (m_impl->m_filename, output);
    }

    #pragma endregion


    #pragma region File handling

    void LoggerSTL::clearFile (const std::string& name)
    {
        // IO can be tricky.
        try
        {
            // Clear by using the truncate flag.
            m_impl->m_file.open (name + ".html", std::ios::out | std::ios::trunc);
            m_impl->m_file.close();
        }

        // Nothing to do.
        catch (...)
        {
        }
    }

    bool LoggerSTL::outputToStream (const std::string& name, const std::string& output)
    {
        // Be careful when dealing with file handling.
        try
        {
            // We need to test if the file is actually a valid filename.
            m_impl->m_file.open (name + ".html", std::ios::out | std::ios::app);
        
            if (m_impl->m_file.is_open())
            {
                // Start by preparing the file.
                m_impl->m_file << output << std::endl;
                m_impl->m_file.close();

                return true;
            }
        }

        // If we crash there's nothing we can do.
        catch (...)
        {
        }

        return false;
    }


    void LoggerSTL::closeCurrentStream()
    {
        // Inject the HTML footer into the current file.
        outputToStream (m_impl->m_filename, getLogFooter());
    }


    std::string LoggerSTL::timestampMessage (const std::string& message)
    {
        // We need to check if we need the timestamp.
        std::string finalMessage;

        if (m_impl->m_timestamp)
        {
            // Use YYYY/MM/DD HH:MM:SS format.
            const auto& format = "%Y/%m/%d %H:%M:%S. ";
            finalMessage += util::getCurrentTimeAsString (format);
        }

        return finalMessage + message;
    }


    std::string LoggerSTL::getLogHeader() const
    {
        // Include the head and start the html and body tags.
        return "<html>\n<head>\n<title>Water Engine Log</title>\n</head>\n<body>\n<h1>Water Engine Log</h1>\n";
    }


    std::string LoggerSTL::getLogFooter() const
    {
        // End the body and html tags.
        return "</body>\n</html>";
    }

    #pragma endregion
}