#include "Time.hpp"


// Utility namespace
namespace util
{
    #pragma region Implementations
       
    time_t util::getCurrentTime()
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
        struct tm localTime;
        localtime_s (&localTime, &time);

        // Prepare a buffer to use for formatting the time.
        char buffer[60];
        strftime (buffer, 60, format.c_str(), &localTime);

        // Returns as a string.
        return std::string (buffer);
    }

    #pragma endregion
}