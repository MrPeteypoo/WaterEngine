#if !defined WATER_AUDIO_SFML_INCLUDED
#define WATER_AUDIO_SFML_INCLUDED


// STL headers.
#include <memory>
#include <unordered_map>
#include <vector>


// Engine headers.
#include <Systems/IEngineAudio.hpp>
#include <Systems/Audio/SFMLSound.hpp>


// Third party headers.
#include <SFML/Audio/SoundBuffer.hpp>


// Forward declarations.
namespace sf { class Music; }


// Engine namespace
namespace water
{
    /// <summary> 
    /// An audio engine which uses SFML to concurrently play multiple sounds.
    /// </summary>
    class AudioSFML final : public IEngineAudio
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


            #pragma region System management

            /// <summary> Initialises the audio engine with the maximum number of sound channels specified. </summary>
            /// <param name="soundLimit"> The total number of sounds available at one time. </param>
            /// <param name="bgmMixer"> The mixer volume for background music, 0 to 1. </param>
            /// <param name="sfxMixer"> The mixer volume for sound effects, 0 to 1. </param>
            void initialise (const unsigned int soundLimit, const float bgmMixer, const float sfxMixer) override final;

            /// <summary> Doesn't actually do anything. </summary>
            void update() override final;

            #pragma endregion


            #pragma region Data managment
            
            /// <summary> Prepares a music track to be streamed. </summary>
            /// <returns> Whether the music track was loaded successfully. </returns>
            bool loadMusic (const std::string& fileLocation) override final;

            /// <summary> Loads a sound file into a sound buffer, this should be used for small sounds, not music. </summary>
            /// <returns> The SoundID used to play the sound buffer. </returns>
            SoundID loadSound (const std::string& fileLocation) override final;

            /// <summary> Deletes a loaded sound from the system. </summary>
            /// <param name="sound"> The ID of the sound to delete. </param>
            void removeSound (const SoundID sound) override final;

            /// <summary> Causes all loaded sound data to be deleted, invaliding all current keys. </summary>
            void clearSoundData() override final;

            #pragma endregion


            #pragma region Playback

            /// <summary> Plays the currently loaded music file. </summary>
            /// <param name="volume"> The core volume of the music track. </param>
            /// <param name="offset"> An offset in seconds can be provided to start the track at a particular point. </param>
            /// <param name="loop"> Whether the track should be looped or not.</param>
            void playMusic (const float volume = 1.f, const float offset = 0.f, const bool loop = true) override final;

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

            /// <summary> Causes all playing sounds to be stopped. </summary>
            void stopSounds() override final;

            /// <summary> Resumes all paused sounds. </summary>
            void resumeSounds() override final;

            /// <summary> Causes all playing sounds to be paused. </summary>
            void pauseSounds() override final;
        
            #pragma endregion

    
            #pragma region Sound properties
            
            /// <summary> Changes the music mixer volume. </summary>
            /// <param name="volume"> A normalised volume between 0 and 1. </param>
            void adjustMusicMixer (const float volume) override final;

            /// <summary> Changes the effects mixer volume. </summary>
            /// <param name="volume"> A normalised volume between 0 and 1. </param>
            void adjustEffectsMixer (const float volume) override final;
            
            /// <summary> Adjusts the properties of a background music. </summary>
            /// <param name="volume"> A normalised volume value from 0 to 1. </param>
            /// <param name="offset"> An offset in seconds for the track. </param>
            /// <param name="loop"> Should the track loop after finishing? </param>
            void adjustMusicProperties (const float volume, const float offset, const bool loop) override final;

            /// <summary> Adjusts the properties of a playing sound. </summary>
            /// <param name="sound"> The currently playing sound to modify. </param>
            /// <param name="volume"> A normalised volume value from 0 to 1. </param>
            /// <param name="offset"> An offset in seconds for the sound. </param>
            /// <param name="loop"> Should the sound loop after finishing? </param>
            void adjustSoundProperties (const PlaybackID sound, const float volume, const float offset = 0.f, const bool loop = false) override final;

            #pragma endregion

        private:

            #pragma region Utility

            /// <summary> Destroys all sound data from the system. </summary>
            void cleanUp();

            /// <summary> Checks whether a given PlaybackID is valid. </summary>
            bool isValidID (const PlaybackID id) const;

            /// <summary> A generic function which iterates through each channel calling the specified function. </summary>
            /// <param name="function"> The function to perform on each channel. </param>
            /// <returns> Returns the index at which the given function returns true. </returns>
            template <typename Function> PlaybackID channelTraversal (Function function);

            /// <summary> Searches through the sound channels to find an unused sound channel. </summary>
            /// <returns> The location of the channel in the vector. </returns>
            PlaybackID findInactiveChannel();

            #pragma endregion
            

            #pragma region Implementation data

            unsigned int                                    m_soundLimit    { 31 };         //!< The number of sound effect channels available concurrently.
            float                                           m_bgmMixer      { 1.f };        //!< The background music mixer, this is how loud the music is.
            float                                           m_sfxMixer      { 1.f };        //!< The effects mixer, this is the volume applied to every sound effect.

            std::unique_ptr<sf::Music>                      m_bgm           { nullptr };    //!< The loaded background music.
            float                                           m_bgmVolume     { 1.f };        //!< The core volume of the music file.

            std::hash<std::string>                          m_hasher        { };            //!< The hasher used to hash file names.
            std::unordered_map<SoundID, sf::SoundBuffer>    m_buffers       { };            //!< A collection of sound buffers containing loaded sound effects.
            std::vector<SFMLSound>                          m_channels      { };            //!< A collection of sound channels to play audio clips back with.

            #pragma endregion
    };
}

#endif