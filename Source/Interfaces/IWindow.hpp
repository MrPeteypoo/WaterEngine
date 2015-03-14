#ifndef WATER_INTERFACE_WINDOW_INCLUDED
#define WATER_INTERFACE_WINDOW_INCLUDED


// STL headers.
#include <string>


// Engine namespace.
namespace water
{
    /// <summary>
    /// An interface to every window management system in the engine. These are used to create graphical windows and manage
    /// window events such as controlling whether the application should close, etc.
    /// </summary>
    class IWindow
    {
        public:

            // Ensure destructor is virtual since this is an interface.
            virtual ~IWindow() { }


            ///////////////////////////
            /// Getters and setters ///
            ///////////////////////////
/*
            /// <summary> Prepares a music track to be streamed. </summary>
            /// <returns> Whether the music track was loaded successfully. </returns>
            virtual bool loadMusic (const std::string& fileLocation) = 0;

            /// <summary> Loads a sound file into a sound buffer, this should be used for small sounds, not music. </summary>
            /// <returns> The SoundID used to play the sound buffer. </returns>
            virtual SoundID loadSound (const std::string& fileLocation) = 0;

            /// <summary> Deletes a loaded sound from the system. </summary>
            /// <param name="sound"> The ID of the sound to delete. </param>
            virtual void removeSound (const SoundID sound) = 0;

            /// <summary> Causes all loaded sound data to be deleted, invaliding all current keys. </summary>
            virtual void clearSoundData() = 0;


            ////////////////
            /// Playback ///
            ////////////////

            /// <summary> Plays the currently loaded music file. </summary>
            /// <param name="volume"> The core volume of the music track. </param>
            /// <param name="offset"> An offset in seconds can be provided to start the track at a particular point. </param>
            /// <param name="loop"> Whether the track should be looped or not.</param>
            virtual void playMusic (const float volume = 1.f, const float offset = 0.f, const bool loop = true) = 0;

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

            /// <summary> Stops a particular sound from playing. Once you request it to stop you should never try to access the sound again. </summary>
            virtual void stopSound (const PlaybackID sound) = 0;

            /// <summary> Resumes a paused sound, this will start from where it left off. </summary>
            virtual void resumeSound (const PlaybackID sound) = 0;

            /// <summary> Pauses a particular sound, this will maintain its position. </summary>
            virtual void pauseSound (const PlaybackID sound) = 0;

            /// <summary> Causes all playing sounds to be stopped. </summary>
            virtual void stopSounds() = 0;

            /// <summary> Resumes all paused sounds. </summary>
            virtual void resumeSounds() = 0;

            /// <summary> Causes all playing sounds to be paused. </summary>
            virtual void pauseSounds() = 0;


            ////////////////////////
            /// Sound properties ///
            ////////////////////////

            /// <summary> Changes the effects mixer volume. </summary>
            /// <param name="volume"> A normalised volume between 0 and 1. </param>
            virtual void adjustEffectsMixer (const float volume) = 0;

            /// <summary> Changes the music mixer volume. </summary>
            /// <param name="volume"> A normalised volume between 0 and 1. </param>
            virtual void adjustMusicMixer (const float volume) = 0;

            /// <summary> Adjusts the properties of a background music. </summary>
            /// <param name="volume"> A normalised volume value from 0 to 1. </param>
            /// <param name="offset"> An offset in seconds for the track. </param>
            /// <param name="loop"> Should the track loop after finishing? </param>
            virtual void adjustMusicProperties (const float volume, const float offset, const bool loop) = 0;

            /// <summary> Adjusts the properties of a playing sound. </summary>
            /// <param name="sound"> The currently playing sound to modify. </param>
            /// <param name="volume"> A normalised volume value from 0 to 1. </param>
            /// <param name="offset"> An offset in seconds for the sound. </param>
            /// <param name="loop"> Should the sound loop after finishing? </param>
            virtual void adjustSoundProperties (const PlaybackID sound, const float volume, const float offset = 0.f, const bool loop = false) = 0;
            */
    };
}

#endif // WATER_INTERFACE_WINDOW_INCLUDED
