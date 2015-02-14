#if !defined MAGA_MEN_UTILITY_INCLUDED
#define MAGA_MEN_UTILITY_INCLUDED


// STL headers.
#include <string>


// Forward declarations.
namespace water { class IRenderer; using TextureID = size_t; }
template <typename T> struct Vector2;


// Game namespace.
namespace mm
{
    /// <summary> A function which will render a string of upper case characters and numbers based on the parameters given. </summary>
    /// <param name="text"> The text to render. </param>
    /// <param name="charID"> The ID of the texture to use for characters. </param>
    /// <param name="numID"> The ID of the texture to use for numbers. </param>
    /// <param name="position"> The position to render the text in world space. </param>
    /// <param name="distBetweenChar"> The padding between characters in the text. </param>
    void renderText (const std::string& text, const water::TextureID charID, const water::TextureID numID, const Vector2<float>& position, const float distBetweenChar);
}


#endif