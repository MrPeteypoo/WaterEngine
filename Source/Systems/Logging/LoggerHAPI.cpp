#include "LoggerHAPI.hpp"


// Engine headers.
#include <Utility/Time.hpp>


// Third party headers.
#include <HAPI/HAPI_lib.h>


// Engine namespace.
namespace water
{   
    #pragma region Constructors and destructor
    
    LoggerHAPI::LoggerHAPI (LoggerHAPI&& move)
    {
        *this = std::move (move);
    }


    LoggerHAPI& LoggerHAPI::operator= (LoggerHAPI&& move)
    {
        if (this != &move)
        {
            m_timestamp = move.m_timestamp;
            move.m_timestamp = false;
        }

        return *this;
    }

    #pragma endregion


    #pragma region System management

    bool LoggerHAPI::initialise (const std::string& file, const bool timestamp)
    {
        // HAPI doesn't support specifying the file, sadface Keith!
        m_timestamp = timestamp;

        return true;
    }


    void LoggerHAPI::update()
    {
        // TODO: Implement me bruv!
    }

    #pragma endregion


    #pragma region Logging

    bool LoggerHAPI::log (const std::string& message)
    {
        return logMessage ("Message: " + message);
    }


    bool LoggerHAPI::logWarning (const std::string& message)
    {
        return logMessage ("Warning: " + message);
    }

    
    bool LoggerHAPI::logError (const std::string& message)
    {
        return logMessage ("Error: " + message);
    }


    bool LoggerHAPI::logMessage (const std::string& message)
    {
        // We need to check if we need the timestamp.
        std::string finalMessage;

        if (m_timestamp)
        {
            // Use YYYY/MM/DD HH:MM:SS format.
            const auto& format = "(%Y/%m/%d %H:%M:%S) ";
            finalMessage += util::getCurrentTimeAsString (format);
        }

        finalMessage += message;
        
        // The documentation says this returns a bool! KEITH YOU LIED TO ME!
        HAPI->DebugText (finalMessage + '\n');

        return true;
    }

    #pragma endregion
}