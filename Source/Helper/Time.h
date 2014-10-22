#if !defined HELPER_TIME_INCLUDED
#define HELPER_TIME_INCLUDED


#include <chrono>   // Required for time_t.


/// <summary> This namespace contains dedicated helper functions with various different purposes. </summary>
namespace helper
{
    /// <summary> Obtains the current system time using the std::chrono library. </summary>
    time_t getCurrentTime();

    /// <summary> Returns the current system time as an unsigned integer. </summary>
    unsigned int getCurrentTimeUInt();
}

#endif // HELPER_TIME_INCLUDED