#if !defined HELPER_MAIN_INCLUDED
#define HELPER_MAIN_INCLUDED

#include <chrono>   // Required for time_t.

namespace helper
{
    /// <summary> Obtains the current system time using the std::chrono library. </summary>
    time_t getCurrentTime();

    /// <summary> Returns the current system time as an unsigned integer. </summary>
    unsigned int getCurrentTimeUInt();
}

#endif // HELPER_MAIN_INCLUDED