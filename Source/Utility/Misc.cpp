#include "Misc.hpp"


// STL headers.
#include <locale>


// Utility namespace.
namespace util
{
    std::string toLower (const std::string& string)
    {
        // We need a locale for converting properly.
        std::locale locale;

        // Convert to a C string.
        std::string lower = "";

        // Convert each character.
        for (auto i = 0U; i < string.length(); ++i)
        {
            lower.push_back (std::tolower (string[i], locale));
        }

        return lower;
    }
}