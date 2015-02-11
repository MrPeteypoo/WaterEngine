#if !defined WATER_ENGINE_BUILDER_INCLUDED
#define WATER_ENGINE_BUILDER_INCLUDED


// STL headers.
#include <string>


// Engine namespace.
namespace water
{
    // Forward declarations.
    class Engine;

    /// <summary>
    /// A simple static class with the sole purpose of constructing Engine objects based on an XML configuration file.
    /// </summary>
    class EngineBuilder final
    {
        public:

            /// <summary> Configures an engine according to the .xml file provided. </summary>
            /// <param name="engine"> The engine object to build. </param>
            /// <param name="file"> The location of the .xml configuration file. </param>
            static void build (Engine& engine, const std::string& file);

        private:


    };
}


#endif