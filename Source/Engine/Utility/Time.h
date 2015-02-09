#if !defined UTILITY_TIME_INCLUDED
#define UTILITY_TIME_INCLUDED


// STL headers.
#include <chrono>
#include <type_traits>



/// <summary> This namespace contains dedicated helper functions with various different purposes. </summary>
namespace util
{
    /// <summary> Obtains the current system time using the std::chrono library. </summary>
    time_t getCurrentTime()
    {
        // Use chrono to convert from a time_point to time_t.
        return  std::chrono::system_clock::to_time_t 
                (
                    std::chrono::system_clock::now()
                );
    }


    /// <summary> Returns the current system time cast to the arithmetic type specified. </summary>
    template <typename T>  typename std::enable_if<std::is_arithmetic<T>::value, T>::type getCurrentTime()
    {
        return static_cast<T> (getCurrentTime());
    }
}

#endif // UTILITY_TIME_INCLUDED