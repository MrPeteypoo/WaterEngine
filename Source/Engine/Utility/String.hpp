#if !defined WATER_UTILITY_STRING_INCLUDED
#define WATER_UTILITY_STRING_INCLUDED


// STL headers.
#include <string>


// Utility namespace.
namespace util
{
    /// <summary> Converts a given string to its lower case equivalent. </summary>
    /// <param name="string"> The string to convert. </param>
    /// <returns> The string in lower case form. </returns>
    std::string toLower (const std::string& string);
}

#endif