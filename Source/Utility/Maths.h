#if !defined UTILITY_MATHS_INCLUDED
#define UTILITY_MATHS_INCLUDED

// STL headers.
#include <type_traits>


namespace util
{
    /// <summary> Returns the minimum value, passed by value for arithmetic types. </summary>
    template <typename T> typename std::enable_if<std::is_arithmetic<T>::value, T>::type min (const T a, const T b)
    {
        return a < b ? a : b;
    }
    

    /// <summary> Returns the maximum value, passed by value for arithmetic types. </summary>
    template <typename T> typename std::enable_if<std::is_arithmetic<T>::value, T>::type max (const T a, const T b)
    {
        return a > b ? a : b;
    }


    /// <summary> Returns the minimum value, passed by reference for non-arithmetic types. </summary>
    template <typename T> typename std::enable_if<!std::is_arithmetic<T>::value, T>::type& min (const T& a, const T& b)
    {
        return a < b ? a : b;
    }


    /// <summary> Returns the maximum value, passed by reference for non-arithmetic types. </summary>
    template <typename T> typename std::enable_if<!std::is_arithmetic<T>::value, T>::type& max (const T& a, const T& b)
    {
        return a > b ? a : b;
    }
}


#endif // UTILITY_MATHS_INCLUDED