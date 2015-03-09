#include "Time.hpp"


// STL headers.
#include <ctime>


// Utility namespace
namespace util
{
    time_t getCurrentTime()
    {
        // Use chrono to convert from a time_point to time_t.
        return  std::chrono::system_clock::to_time_t
                (
                    std::chrono::system_clock::now()
                );
    }


    std::string getCurrentTimeAsString (const std::string& format)
    {
        // First obtain the time.
        const auto time = getCurrentTime();
        struct tm* localTime;

        localTime = localtime (&time);

        // Prepare a buffer to use for formatting the time.
        char buffer[60];
        strftime (buffer, 60, format.c_str(), localTime);

        // Returns as a string.
        return std::string (buffer);
    }
}
