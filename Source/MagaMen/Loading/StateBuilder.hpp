#if !defined MAGA_MEN_STATE_BUILDER_INCLUDED
#define MAGA_MEN_STATE_BUILDER_INCLUDED


// STL headers.
#include <string>


// Forward declarations.
namespace pugi
{
    class xml_node;
}


// Game namespace.
namespace mm
{
    // Forward declarations.
    class StaticObject;

    /// <summary>
    /// A static class which provides common functionality required to build states from an XML file.
    /// </summary>
    class StateBuilder
    {
        public:

            #pragma region Construct objects

            /// <summary> Fills the given StaticObject with the data specified in the given node. </summary>
            /// <param name="output"> The object to have its data filled. </param>
            /// <param name="node"> The node to retrieve information from. </param>
            /// <param name="workingDir"> The working directory for texture loading. </param>
            /// <returns> Whether something went wrong. </returns>
            static bool constructStaticObject (StaticObject& output, const pugi::xml_node& node, const std::string& workingDir);

            #pragma endregion
    };
}


#endif