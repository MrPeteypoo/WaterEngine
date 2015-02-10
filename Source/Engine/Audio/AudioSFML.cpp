#include "AudioSFML.h"


// STL headers.
#include <exception>
#include <iostream>
#include <unordered_map>
#include <vector>


// Engine headers.
#include <Systems.h>
#include <ILogger.h>
#include <Audio/SFMLSound.h>
#include <Utility/Maths.h>


// Third party headers.
#include <SFML/Audio/Music.hpp>
#include <SFML/Audio/SoundBuffer.hpp>


// Engine namespace.
namespace water
{
    #pragma region Implementation data

    /// <summary> Contains the implementation data for the AudioSystemSFML class. </summary>
    struct AudioSFML::Impl final
    {
        size_t                                          soundLimit      { 31 };     //!< The number of sound effect channels available concurrently.
        float                                           bgmMixer        { 1.f };    //!< The background music mixer, this is how loud the music is.
        float                                           sfxMixer        { 1.f };    //!< The effects mixer, this is the volume applied to every sound effect.

        sf::Music                                       bgm             { };        //!< The loaded background music.
        float                                           bgmVolume       { 1.f };    //!< The core volume of the music file.

        std::hash<std::string>                          hasher          { };        //!< The hasher used to hash file names.
        std::unordered_map<SoundID, sf::SoundBuffer>    buffers         { };        //!< A collection of sound buffers containing loaded sound effects.
        std::vector<Sound>                              channels        { };        //!< A collection of sound channels to play audio clips back with.
    };

    #pragma endregion


    #pragma region Constructors and destructor

    AudioSFML::AudioSFML()
    {
        m_impl = new Impl();
    }


    AudioSFML::AudioSFML (AudioSFML&& move)
    {
        *this = std::move (move);
    }


    AudioSFML& AudioSFML::operator= (AudioSFML&& move)
    {
        if (this != &move)
        {
            // Delete current data if necessary.
            if (m_impl)
            {
                delete m_impl;
            }

            m_impl = move.m_impl;
            move.m_impl = nullptr;
        }

        return *this;
    }


    AudioSFML::~AudioSFML()
    {
        cleanUp();

        if (m_impl)
        {
            delete m_impl;
            m_impl = nullptr;
        }
    }

    #pragma endregion


    #pragma region System management

    void AudioSFML::initialise (const size_t soundLimit, const float bgmMixer, const float sfxMixer)
    {
        // Pre-condition: We have a valid limit.
        if (soundLimit <= 1)
        {
            throw std::invalid_argument ("AudioSFML::initialise(): Invalid sound limit given, must be 2 or higher.");
        }

        // SFML has a hard limit so we will impose one here by allowing a maximum of 256 sound channels.
        const size_t hardLimit  { 256 };

        // Calculate the limit to apply. We reserve one for the BGM.
        const auto limit = soundLimit > hardLimit ? hardLimit - 1 : soundLimit - 1;

        m_impl->soundLimit = limit;
        m_impl->channels.resize (limit);
        adjustMusicMixer (bgmMixer);
        adjustEffectsMixer (sfxMixer);
    }


    void AudioSFML::update()
    {
        // TODO: Implement me bruv!
    }

    #pragma endregion


    #pragma region Data management
    
    bool AudioSFML::loadMusic (const std::string& fileLocation)
    {
        // Let SFML tell us whether the loading was successful or not.
        return m_impl->bgm.openFromFile (fileLocation);
    }


    SoundID AudioSFML::loadSound (const std::string& fileLocation)
    {
        // We need to attempt to load the sound buffer into memory.
        sf::SoundBuffer buffer { };

        if (buffer.loadFromFile (fileLocation))
        {
            // Hash the file location at load-time so that run-time accessing can be fast.
            const auto id = m_impl->hasher (fileLocation);

            // Add the buffer to the map and return the valid ID.
            m_impl->buffers.emplace (std::make_pair (id, buffer));

            return id;
        }

        // Output an error message to inform the user or programmer of their stupidity.
        Systems::logger().logError ("AudioSFML::loadSound(): Cannot load sound with filename \"" + fileLocation + "\".");

        // Let the user cry due to lack of sounds.
        return 0;
    }


    void AudioSFML::removeSound (const SoundID sound)
    {
    
    }

    
    void AudioSFML::clearSoundData()
    {
        cleanUp();

        // Reset to defaults.
        m_impl->bgm.openFromFile ("");
        adjustMusicProperties (1.f, 0.f, true);
        m_impl->channels.resize (m_impl->soundLimit);
    }

    #pragma endregion


    #pragma region Playback

    void AudioSFML::playMusic (const float volume, const float offset, const bool loop)
    {
        m_impl->bgm.play();
        adjustMusicProperties (volume, offset, loop);
    }


    void AudioSFML::stopMusic()
    {
        m_impl->bgm.stop();
    }

    
    void AudioSFML::resumeMusic()
    {
        m_impl->bgm.play();
    }


    void AudioSFML::pauseMusic()
    {
        m_impl->bgm.pause();
    }


    PlaybackID AudioSFML::playSound (const SoundID sound, const float volume, const float offset, const bool loop)
    {
        // If the buffer doesn't exist an out-of-range error will be thrown.
        const auto& buffer  = m_impl->buffers.at (sound);

        // We need to check if we have a valid channel to play the buffer from.
        const auto channel  = findInactiveChannel();

        if (channel != std::numeric_limits<PlaybackID>::max())
        {
            // Cache the current channel.
            auto& currentChannel = m_impl->channels[channel];
                
            // We need to change swap the buffer currently in use and play the sound.
            currentChannel.stop();
            currentChannel.setBuffer (buffer);
            currentChannel.play();
            adjustSoundProperties (channel, volume, offset, loop);

            // Allow the sound to be managed externally if desired.
            return channel;
        }

        // Log the problem.
        Systems::logger().logError ("AudioSFML::playSound(), Ran out of sound channels. Sound will not be played.");
        
        // Return an invalid value.
        return std::numeric_limits<PlaybackID>::max();
    }


    void AudioSFML::stopSound (const PlaybackID sound)
    {
        // Silently ignore invalid sounds.
        if (isValidID (sound))
        {
            m_impl->channels[sound].stop();
        }
    }


    void AudioSFML::resumeSound (const PlaybackID sound)
    {
        if (isValidID (sound))
        {
            m_impl->channels[sound].play();
        }
    }


    void AudioSFML::pauseSound (const PlaybackID sound)
    {
        if (isValidID (sound))
        {
            m_impl->channels[sound].pause();
        }
    }


    void AudioSFML::stopSounds()
    {
        // Forceably stop all sounds from playing.
        auto stopFunction = [] (Sound& channel) { channel.stop(); return false; };

        channelTraversal (stopFunction);
    }


    void AudioSFML::resumeSounds()
    {
        // Resume all sounds from where they left off.
        auto resumeFunction = [] (Sound& channel) { channel.play(); return false; };

        channelTraversal (resumeFunction);
    }


    void AudioSFML::pauseSounds()
    {
        // Pause all sounds at their current position.
        auto pauseFunction = [] (Sound& channel) { channel.pause(); return false; };

        channelTraversal (pauseFunction);
    }

    #pragma endregion


    #pragma region Sound properties

    void AudioSFML::adjustMusicMixer (const float volume)
    {
        // Clamp the value between 0 and 1.
        m_impl->bgmMixer = util::clamp (volume, 0.f, 1.f);
        m_impl->bgm.setVolume (100.f * m_impl->bgmVolume * m_impl->bgmMixer);
    }


    void AudioSFML::adjustEffectsMixer (const float volume)
    {
        // Clamp the value between 0 and 1.
        m_impl->sfxMixer = util::clamp (volume, 0.f, 1.f);

        // Now we need to create a function that will adjust the volume of each channel.
        auto volumeAdjust = 
        [=] (Sound& channel)
        {
            channel.resetVolume (m_impl->sfxMixer);
            return false;
        };

        channelTraversal (volumeAdjust);
    }


    void AudioSFML::adjustMusicProperties (const float volume, const float offset, const bool loop)
    {
        // Set the volume.
        m_impl->bgmVolume   = util::clamp (volume, 0.f, 1.f);
        m_impl->bgm.setVolume (100.f * m_impl->bgmVolume * m_impl->bgmMixer);

        // Adjust the offset.
        const auto cap      = m_impl->bgm.getDuration().asSeconds();
        const auto position = util::clamp (offset, 0.f, cap);
        m_impl->bgm.setPlayingOffset (sf::seconds (position));

        // And tell it to loop!
        m_impl->bgm.setLoop (loop);
    }


    void AudioSFML::adjustSoundProperties (const PlaybackID sound, const float volume, const float offset, const bool loop)
    {
        // Silently ignore programmer/user error.
        if (isValidID (sound))
        {
            // Cache the channel and adjust each property.
            auto& channel = m_impl->channels[sound];

            channel.setVolume (volume, m_impl->sfxMixer);
            channel.setOffset (offset);
            channel.setLooping (loop);
        }
    }

    #pragma endregion


    #pragma region Utility

    void AudioSFML::cleanUp()
    {   
        // Stop everything first.
        stopMusic();
        stopSounds();

        // We need to clear the sound buffers.
        m_impl->channels.clear();
        m_impl->buffers.clear();
    }


    template <typename Function> PlaybackID AudioSFML::channelTraversal (Function function)
    {
        // Function should be a lambda function which will be called on each channel.
        for (auto i = 0U; i < m_impl->channels.size(); ++i)
        {
            if (function (m_impl->channels[i]))
            {
                return i;
            }
        }

        return std::numeric_limits<PlaybackID>::max();
    }


    PlaybackID AudioSFML::findInactiveChannel()
    {
        // Make a function which will find an inactive channel.
        auto inactiveFunction = [] (Sound& channel)
        {
            return channel.hasStopped();
        };

        return channelTraversal (inactiveFunction);
    }


    bool AudioSFML::isValidID (const PlaybackID id) const
    {
        return id < m_impl->soundLimit;
    }

    #pragma endregion
}