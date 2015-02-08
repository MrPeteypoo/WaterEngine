#if !defined AUDIO_SYSTEM_SFML_INCLUDED
#define AUDIO_SYSTEM_SFML_INCLUDED


// Engine headers.
#include <Audio/IAudio.h>



/// <summary> 
/// An audio engine which uses SFML to play multiple sounds at the same time. 
/// </summary>
class AudioSFML final : public IAudio
{
    public:

        #pragma region Constructors and destructor

        AudioSFML();
        AudioSFML (AudioSFML&& move);
        AudioSFML& operator= (AudioSFML&& move);

        ~AudioSFML() override final;
        
        AudioSFML (const AudioSFML& copy)               = delete;
        AudioSFML& operator= (const AudioSFML& copy)    = delete;

        #pragma endregion


        #pragma region Initialisation

        /// <summary> Initialises the audio engine with the maximum number of sound channels specified. </summary>
        /// <param name="soundLimit"> The total number of sounds available at one time. </param>
        void initialise (const size_t soundLimit = 32) override final;

        /// <summary> Causes all loaded sound data to be deleted, invaliding all current keys. </summary>
        void clearSoundData() override final;

        /// <summary> Prepares a music track to be streamed. The track won't be changed if the given file doesn't exist. </summary>
        /// <returns> Whether the music track was changed. </returns>
        bool loadMusic (const std::string& fileLocation) override final;

        /// <summary> Loads a sound file into a sound buffer, this should be used for small sounds, not music. </summary>
        /// <returns> The SoundID used to play the sound buffer. </returns>
        SoundID loadSound (const std::string& fileLocation) override final;

        #pragma endregion


        #pragma region Playback

        /// <summary> Updates the audio system. </summary>
        void update() override final;

        /// <summary> Causes all playing sounds to be stopped. </summary>
        void stopAll() override final;

        /// <summary> Resumes all paused sounds. </summary>
        void resumeAll() override final;

        /// <summary> Causes all playing sounds to be paused. </summary>
        void pauseAll() override final;

        /// <summary> Plays the currently loaded music file. </summary>
        /// <param name="offset"> An offset in seconds can be provided to start the track at a particular point. </param>
        /// <param name="loop"> Whether the track should be looped or not.</param>
        void playMusic (const float offset = 0.f, const bool loop = true) override final;

        /// <summary> Stops the music from playing entirely. </summary>
        void stopMusic() override final;

        /// <summary> Resumes the music track from the current point. </summary>
        void resumeMusic() override final;

        /// <summary> Pauses the music at its current position. </summary>
        void pauseMusic() override final;

        /// <summary> Plays the given sound with the desired parameters. </summary>
        /// <param name="sound"> The loaded sound buffer to play. </param>
        /// <param name="volume"> A normalised volume value from 0 to 1. </param>
        /// <param name="offset"> An offset in seconds can be provided to start the sound at a particular point. </param>
        /// <param name="loop"> Should the sound loop after finishing? </param>
        /// <returns> The ID assigned to the newly playing sound. </returns>
        PlaybackID playSound (const SoundID sound, const float volume = 1.f, const float offset = 0.f, const bool loop = false) override final;

        /// <summary> Stops a particular sound from playing. </summary>
        void stopSound (const PlaybackID sound) override final;

        /// <summary> Resumes a paused sound, this will start from where it left off. </summary>
        void resumeSound (const PlaybackID sound) override final;

        /// <summary> Pauses a particular sound, this will maintain its position. </summary>
        void pauseSound (const PlaybackID sound) override final;
        
        #pragma endregion

    
        #pragma region Sound properties

        /// <summary> Changes the effects mixer volume. </summary>
        /// <param name="volume"> A normalised volume between 0 and 1. </param>
        void adjustEffectsVolume (const float volume) override final;

        /// <summary> Changes the music mixer volume. </summary>
        /// <param name="volume"> A normalised volume between 0 and 1. </param>
        void adjustMusicVolume (const float volume) override final;

        /// <summary> Adjusts the properties of a playing sound. </summary>
        /// <param name="sound"> The currently playing sound to modify. </param>
        /// <param name="volume"> A normalised volume value from 0 to 1. </param>
        /// <param name="offset"> An offset in seconds for the sound. </param>
        /// <param name="loop"> Should the sound loop after finishing? </param>
        void adjustSoundProperties (const PlaybackID sound, const float volume, const float offset = 0.f, const bool loop = false) override final;

        #pragma endregion

    private:

        // Forward declarations.
        class Sound;
        struct Impl;


        Impl*   m_impl  { nullptr };    //!< A pointer to the implementation data.
};

#endif // AUDIO_SYSTEM_SFML_INCLUDED