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


            #pragma region System management
    
            /// <summary> Initialise all data and prepare for rendering. </summary>
            /// <param name="screenWidth"> The width of the screen resolution. </param>
            /// <param name="screenHeight"> The height of the screen resolution. </param>
            /// <param name="internalWidth"> The width of the internal framebuffer. </param>
            /// <param name="internalHeight"> The height of the internal framebuffer. </param>
            /// <param name="filter"> The filter to be applied when scaling the internal framebuffer to the screen. </param>
            virtual void initialise (const int screenWidth, const int screenHeight, const int internalWidth, const int internalHeight, const FilterMode filter) = 0;

            /// <summary> Updates the IRenderer system. </summary>
            /// <returns> Whether the update was successful. </returns>
            virtual bool update() = 0;

            #pragma endregion
    };   
}

#endif