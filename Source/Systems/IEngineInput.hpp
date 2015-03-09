#if !defined WATER_INTERFACE_INPUT_ENGINE_INCLUDED
#define WATER_INTERFACE_INPUT_ENGINE_INCLUDED


// Engine headers.
#include <Interfaces/IInput.hpp>


// Engine namespace.
namespace water
{
    /// <summary>
    /// An interface for the engine to perform system-critical functions to IInput systems.
    /// </summary>
    class IEngineInput : public IInput
    {
        public:

            // Ensure destructor is virtual since this is an interface.
            virtual ~IEngineInput() override { }


            /////////////////////////
            /// System management ///
            /////////////////////////

            /// <summary> Initialises the system, preparing it for usage by the rest of the engine. </summary>
            virtual void initialise() = 0;

            /// <summary> Updates the system, this refreshes the state of actions. </summary>
            virtual void update() = 0;
    };
}

#endif
