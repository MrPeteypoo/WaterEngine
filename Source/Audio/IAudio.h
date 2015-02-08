#if !defined WATER_INTERFACE_AUDIO_INCLUDED
#define WATER_INTERFACE_AUDIO_INCLUDED


// STL headers.
#include <string>


// Engine namespace.
namespace water
{
    // Forward declarations.
    using PlaybackID = size_t;
    using SoundID = size_t;


    /// <summary> 
    /// An interface to every audio system used in the engine. 
    /// </summary>
    class IAudio
    {
        public:
        
            // Ensure destructor is virtual since this is an interface.
            virtual ~IAudio() { }

        
            #pragma region Initialisation

            /// <summary> Initialises the audio engine with the maximum number of sound channels specified. </summary>
            /// <param name="soundLimit"> The total number of sounds available at one time. </param>
            virtual void initialise (const size_t soundLimit = 32) = 0;

            /// <summary> Causes all loaded sound data to be deleted, invaliding all current keys. </summary>
            virtual void clearSoundData() = 0;

            /// <summary> Prepares a music track to be streamed. The track won't be changed if the given file doesn't exist. </summary>
            /// <returns> Whether the music track was changed. </returns>
            virtual bool loadMusic (const std::string& fileLocation) = 0;

            /// <summary> Loads a sound file into a sound buffer, this should be used for small sounds, not music. </summary>
            /// <returns> The SoundID used to play the sound buffer. </returns>
            virtual SoundID loadSound (const std::string& fileLocation) = 0;

            #pragma endregion


            #pragma region Playback

            /// <summary> Updates the audio system. </summary>
            virtual void update() = 0;

            /// <summary> Causes all playing sounds to be stopped. </summary>
            virtual void stopAll() = 0;

            /// <summary> Resumes all paused sounds. </summary>
            virtual void resumeAll() = 0;

            /// <summary> Causes all playing sounds to be paused. </summary>
            virtual void pauseAll() = 0;

            /// <summary> Plays the currently loaded music file. </summary>
            /// <param name="offset"> An offset in seconds can be provided to start the track at a particular point. </param>
            /// <param name="loop"> Whether the track should be looped or not.</param>
            virtual void playMusic (const float offset = 0.f, const bool loop = true) = 0;

            /// <summary> Stops the music from playing entirely. </summary>
            virtual void stopMusic() = 0;

            /// <summary> Resumes the music track from the current point. </summary>
            virtual void resumeMusic() = 0;

            /// <summary> Pauses the music at its current position. </summary>
            virtual void pauseMusic() = 0;

            /// <summary> Plays the given sound with the desired parameters. </summary>
            /// <param name="sound"> The loaded sound buffer to play. </param>
            /// <param name="volume"> A normalised volume value from 0 to 1. </param>
            /// <param name="offset"> An offset in seconds can be provided to start the sound at a particular point. </param>
            /// <param name="loop"> Should the sound loop after finishing? </param>
            /// <returns> The ID assigned to the newly playing sound. </returns>
            virtual PlaybackID playSound (const SoundID sound, const float volume = 1.f, const float offset = 0.f, const bool loop = false) = 0;

            /// <summary> Stops a particular sound from playing. </summary>
            virtual void stopSound (const PlaybackID sound) = 0;

            /// <summary> Resumes a paused sound, this will start from where it left off. </summary>
            virtual void resumeSound (const PlaybackID sound) = 0;

            /// <summary> Pauses a particular sound, this will maintain its position. </summary>
            virtual void pauseSound (const PlaybackID sound) = 0;
        
            #pragma endregion

    
            #pragma region Sound properties

            /// <summary> Changes the effects mixer volume. </summary>
            /// <param name="volume"> A normalised volume between 0 and 1. </param>
            virtual void adjustEffectsVolume (const float volume) = 0;

            /// <summary> Changes the music mixer volume. </summary>
            /// <param name="volume"> A normalised volume between 0 and 1. </param>
            virtual void adjustMusicVolume (const float volume) = 0;

            /// <summary> Adjusts the properties of a playing sound. </summary>
            /// <param name="sound"> The currently playing sound to modify. </param>
            /// <param name="volume"> A normalised volume value from 0 to 1. </param>
            /// <param name="offset"> An offset in seconds for the sound. </param>
            /// <param name="loop"> Should the sound loop after finishing? </param>
            virtual void adjustSoundProperties (const PlaybackID sound, const float volume, const float offset = 0.f, const bool loop = false) = 0;

            #pragma endregion
    };
}

#endif // WATER_INTERFACE_AUDIO_INCLUDED