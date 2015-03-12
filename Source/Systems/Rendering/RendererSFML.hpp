#if !defined WATER_RENDERER_SFML_INCLUDED
#define WATER_RENDERER_SFML_INCLUDED


// STL headers.
#include <unordered_map>


// Engine headers.
#include <Systems/IEngineRenderer.hpp>


namespace water
{
    /// <summary>
    /// A 2D rendering engine which uses SFML for rendering and window management tasks.
    /// </summary>
    class RendererSFML final : public IEngineRenderer
    {
        public:

            ///////////////////////////////////
            /// Constructors and destructor ///
            ///////////////////////////////////

            RendererSFML()                                      = default;
            RendererSFML(const RendererSFML& other);
            RendererSFML& operator=(const RendererSFML& other);
            ~RendererSFML() override final                      = default;
        protected:
        private:
    };
}

#endif // WATER_RENDERER_SFML_INCLUDED
