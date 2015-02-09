#include "LoggerHAPI.h"


// Third party headers.
#include <HAPI/HAPI_lib.h>


// Engine namespace.
namespace water
{
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
        // The documentation says this returns a bool! KEITH YOU LIED TO ME!
        HAPI->DebugText (message + '\n');

        return true;
    }

    #pragma endregion
}