#if !defined WATER_INTERFACE_AUDIO_ENGINE_INCLUDED
#define WATER_INTERFACE_AUDIO_ENGINE_INCLUDED


// Engine headers.
#include <Interfaces/IAudio.hpp>


// Engine namespace.
namespace water
{
    /// <summary>
    /// An interface for the engine to perform system-critical functions to IAudio systems.
    /// </summary>
    class IEngineAudio : public IAudio
    {
        public:

            // Ensure destructor is virtual since this is an interface.
            virtual ~IEngineAudio() override {}


            #pragma region System management

            /// <summary> Initialises the audio engine with the maximum number of sound channels specified. </summary>
            /// <param name="soundLimit"> The total number of sounds available at one time. </param>
            /// <param name="bgmMixer"> The mixer volume for background music, 0 to 1. </param>
            /// <param name="sfxMixer"> The mixer volume for sound effects, 0 to 1. </param>
            virtual void initialise (const unsigned int soundLimit, const float bgmMixer, const float sfxMixer) = 0;

            /// <summary> Updates the audio system. </summary>
            virtual void update() = 0;

            #pragma endregion
    };   
}

#endif