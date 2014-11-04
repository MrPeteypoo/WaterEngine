#if !defined UTILITY_TIME_INCLUDED
#define UTILITY_TIME_INCLUDED


// STL headers.
#include <chrono>


/// <summary> This namespace contains dedicated helper functions with various different purposes. </summary>
namespace utility
{
    /// <summary> Obtains the current system time using the std::chrono library. </summary>
    time_t getCurrentTime();

    /// <summary> Returns the current system time as an unsigned integer. </summary>
    unsigned int getCurrentTimeUInt();
}

#endif // UTILITY_TIME_INCLUDED