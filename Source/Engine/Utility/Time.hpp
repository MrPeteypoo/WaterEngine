#if !defined WATER_UTILITY_TIME_INCLUDED
#define WATER_UTILITY_TIME_INCLUDED


// STL headers.
#include <chrono>
#include <string>
#include <type_traits>


// Utility namespace.
namespace util
{
    /// <summary> Returns the current system time cast to the arithmetic type specified. </summary>
    template <typename T>  typename std::enable_if<std::is_arithmetic<T>::value, T>::type getCurrentTime()
    {
        return (T) getCurrentTime();
    }
 
    /// <summary> Obtains the current system time using the std::chrono library. </summary>
    time_t getCurrentTime();

    /// <summary> Obtains the current time and converts it to a string of the given format. </summary>
    /// <param name="format"> The desired format of the string, this follows the rules of std::strftime. </param>
    /// <returns> Returns the formatted string. </returns>
    std::string getCurrentTimeAsString (const std::string& format);
}

#endif