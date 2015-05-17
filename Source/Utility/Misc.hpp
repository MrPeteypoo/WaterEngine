#if !defined WATER_UTILITY_MISC_INCLUDED
#define WATER_UTILITY_MISC_INCLUDED


// STL headers.
#include <string>
#include <vector>


// Utility namespace.
namespace util
{
    /// <summary> Converts a given string to its lower case equivalent. </summary>
    /// <param name="string"> The string to convert. </param>
    /// <returns> The string in lower case form. </returns>
    std::string toLower (const std::string& string);


    /// <summary> Removes the contents of a vector by popping the back until empty, this maintains the reserved capacity. </summary>
    /// <param name="toEmpty"> The vector to be emptied. </param>
    template <typename T> void removeContents (std::vector<T>& toEmpty)
    {
        while (!toEmpty.empty())
        {
            toEmpty.pop_back();
        }
    }
}

#endif
