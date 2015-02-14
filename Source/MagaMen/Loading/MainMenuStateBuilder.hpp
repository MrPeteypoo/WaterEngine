#if !defined MAGA_MEN_MAIN_MENU_STATE_BUILDER_INCLUDED
#define MAGA_MEN_MAIN_MENU_STATE_BUILDER_INCLUDED


// Game headers.
#include <MagaMen/Loading/StateBuilder.hpp>


// Game namespace.
namespace mm
{
    // Forward declarations.
    class MainMenuState;


    /// <summary>
    /// A static class dedicated to the building of the MainMenuState from an XML file.
    /// </summary>
    class MainMenuStateBuilder final : public StateBuilder
    {
        public:

            /// <summary> Attempts to load a MainMenuState from the file information stored within the state. </summary>
            /// <param name="state"> The state to be loaded. </param>
            /// <returns> Whether the operation was successful or not. </returns>
            static bool loadFromFile (MainMenuState& state);
    };
}


#endif