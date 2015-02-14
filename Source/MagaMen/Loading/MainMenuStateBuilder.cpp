#include "MainMenuStateBuilder.hpp"


// Game headers.
#include <MagaMen/States/MainMenuState.hpp>


// Third party headers.
#include <ThirdParty/pugixml.hpp>


// Game namespace.
namespace mm
{
    bool MainMenuStateBuilder::loadFromFile (MainMenuState& state)
    {
        // Start by creating and loading the XML document.
        pugi::xml_document xml  { };

        if (xml.load_file (state.m_dataFile.c_str()))
        {
            // Test if the root is valid.
            const auto root = xml.child ("MainMenuState");

            if (!root.empty())
            {
                // Make sure we have valid textures for rendering text.
                const auto workingDir   = std::string (root.child ("WorkingDirectory").text().as_string());
                const auto letters      = root.child ("LettersTexture");
                const auto numbers      = root.child ("NumbersTexture");

                // Attempt to load the textures.
                state.letters = state.renderer().loadTexture (workingDir + letters.text().as_string());
                state.numbers = state.renderer().loadTexture (workingDir + numbers.text().as_string());
                
                // Zero is 99% likely to represent an invalid texture.
                if (state.letters == 0 || state.numbers == 0)
                {
                    return false;
                }

                // Crop each texture and set the frame dimensions.
                const auto lettersCrop      = Point (letters.attribute ("CropRight").as_int(),  letters.attribute ("CropBottom").as_int());
                const auto lettersFrames    = Point (letters.attribute ("XFrames").as_int(),    letters.attribute ("YFrames").as_int());
                const auto numbersCrop      = Point (numbers.attribute ("CropRight").as_int(),  numbers.attribute ("CropBottom").as_int());
                const auto numbersFrames    = Point (numbers.attribute ("XFrames").as_int(),    numbers.attribute ("YFrames").as_int());
                
                state.renderer().cropTexture (state.letters, lettersCrop.x, lettersCrop.y);
                state.renderer().cropTexture (state.numbers, numbersCrop.x, numbersCrop.y);

                state.renderer().setFrameDimensions (state.letters, lettersFrames);
                state.renderer().setFrameDimensions (state.numbers, numbersFrames);

                // Prepare the rest of the state. We can recover from lack of audio.
                const auto bgmNode      = root.child ("BGM");
                state.m_bgmFile         = workingDir + bgmNode.text().as_string();
                state.m_bgmVolume       = bgmNode.attribute ("Volume").as_float();
                
                // Load the static objects.
                StaticObject object { };
                for (const auto& node : root.child ("StaticObjects"))
                {
                    if (constructStaticObject (object, node, workingDir))
                    {
                        state.m_objects.emplace_back (std::move (object));                    
                    }
                }

                return true;
            }
        }

        // I guess we couldn't load.
        return false;
    }
}