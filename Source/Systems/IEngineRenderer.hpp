#if !defined WATER_INTERFACE_RENDERER_ENGINE_INCLUDED
#define WATER_INTERFACE_RENDERER_ENGINE_INCLUDED


// Engine headers.
#include <Interfaces/IRenderer.hpp>


// Engine namespace.
namespace water
{
    /// <summary>
    /// An interface for the engine to perform system-critical functions to IRenderer systems.
    /// </summary>
    class IEngineRenderer : public IRenderer
    {
        public:

            // Ensure destructor is virtual since this is an interface.
            virtual ~IEngineRenderer() override {}


            ///////////////////////
            // System management //
            ///////////////////////

            /// <summary> Initialise all data and prepare for rendering. </summary>
            /// <param name="internalWidth"> The base width sprites were scaled to match. </param>
            /// <param name="internalHeight"> The base height sprites were scaled to match. </param>
            /// <param name="smooth"> Whether loaded textures should be smoothed or not. </param>
            virtual void initialise (const unsigned int internalWidth, const unsigned int internalHeight, const bool smooth) = 0;

            /// <summary> Updates the IRenderer system. </summary>
            virtual void update() = 0;
    };
}

#endif
