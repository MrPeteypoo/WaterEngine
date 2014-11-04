#include "Time.h"


time_t utility::getCurrentTime()
{
    // Use chrono to convert from a time_point to time_t.
    return  std::chrono::system_clock::to_time_t 
            (
                std::chrono::system_clock::now()
            );
}


unsigned int utility::getCurrentTimeUInt()
{
    return (unsigned int) getCurrentTime();
}