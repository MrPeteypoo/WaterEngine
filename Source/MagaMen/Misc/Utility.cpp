#include "Utility.hpp"


// Engine headers.
#include <Engine/Systems.hpp>
#include <Engine/Misc/Vector2.hpp>
#include <Engine/Utility/Misc.hpp>


// Game namespace.
namespace mm
{
    void renderText (const std::string& text, const water::TextureID charID, const water::TextureID numID, const Vector2<float>& position, const float distBetweenChar)
    {
        // This is a very game-specific function where we know we only have character a-z and numbers 0-9.
        const auto& lowerText = util::toLower (text);
        auto textPosition = position;
        for (auto i = 0U; i < lowerText.size(); ++i)
        {
            // Each character and number are individual frames of a texture.
            auto idToUse    = water::TextureID (0);
            auto frameToUse = -1;
            char character  = lowerText[i];

            // Check if the character is a number or letter.
            if (character >= 'a' && character <= 'z')
            {
                // Calculate the frame by taking advantage of character order. We must ensure 'a' == 0 to get a correct frame offset.
                idToUse     = charID;
                frameToUse  = character - 'a';
            }

            else if (character >= '0' && character <= '9')
            {
                idToUse     = numID;
                frameToUse  = character - '0';
            }

            // Render the text.
            if (frameToUse != -1)
            {
                water::Systems::renderer().drawToScreen (textPosition, idToUse, { frameToUse, 0 }, water::BlendType::Transparent);
            }

            // Increase the position offset for the next render.
            textPosition.x += distBetweenChar;
        }
    }
}