#include "AudioSFML.hpp"


// STL headers.
#include <exception>
#include <limits>


// Engine headers.
#include <Systems.hpp>
#include <Utility/Maths.hpp>


// Third party headers.
#include <SFML/Audio/Music.hpp>


// Engine namespace.
namespace water
{
    /////////////////////////////////
    // Constructors and destructor //
    /////////////////////////////////


    AudioSFML::AudioSFML()
    {
        // We must make the sf::Music object a pointer so we can at least support move construction, sf::Music is non-copyable with no move constructor.
        m_bgm.reset (new sf::Music());
    }


    AudioSFML::AudioSFML (AudioSFML&& move)
    {
        *this = std::move (move);
    }


    AudioSFML& AudioSFML::operator= (AudioSFML&& move)
    {
        if (this != &move)
        {
            m_soundLimit    = move.m_soundLimit;
            m_bgmMixer      = move.m_bgmMixer;
            m_sfxMixer      = move.m_sfxMixer;

            m_bgm           = std::move (move.m_bgm);
            m_bgmVolume     = m_bgmVolume;

            m_hasher        = std::move (move.m_hasher);
            m_buffers       = std::move (move.m_buffers);
            m_channels      = std::move (move.m_channels);

            // Reset primitive data types.
            move.m_soundLimit   = 0;
            move.m_bgmMixer     = 0.f;
            move.m_sfxMixer     = 0.f;
            move.m_bgmVolume    = 0.f;
        }

        return *this;
    }


    AudioSFML::~AudioSFML()
    {
        cleanUp();
    }


    ///////////////////////
    // System management //
    ///////////////////////

    void AudioSFML::initialise (const unsigned int soundLimit, const float bgmMixer, const float sfxMixer)
    {
        // Pre-condition: We have a valid limit.
        if (soundLimit <= 1)
        {
            throw std::invalid_argument ("AudioSFML::initialise(): Invalid sound limit given, must be 2 or higher.");
        }

        // SFML has a hard limit so we will impose one here by allowing a maximum of 256 sound channels.
        const auto hardLimit = 256U ;

        // Calculate the limit to apply. We reserve one for the BGM.
        const auto limit = soundLimit > hardLimit ? hardLimit - 1 : soundLimit - 1;

        m_soundLimit = limit;
        m_channels.resize (limit);

        adjustMusicMixer (bgmMixer);
        adjustEffectsMixer (sfxMixer);
    }


    void AudioSFML::update()
    {
        // TODO: Implement me bruv!
    }


    /////////////////////
    // Data management //
    /////////////////////

    bool AudioSFML::loadMusic (const std::string& fileLocation)
    {
        // Let SFML tell us whether the loading was successful or not.
        return m_bgm->openFromFile (fileLocation);
    }


    SoundID AudioSFML::loadSound (const std::string& fileLocation)
    {
        // Hash the file location at load-time so that run-time accessing can be fast.
        const auto id = m_hasher (fileLocation);

        // Check if it already exists.
        const auto& iterator = m_buffers.find (id);
        if (iterator != m_buffers.end())
        {
            // We have already loaded that sound, just return the ID.
            return iterator->first;
        }

        else
        {
            // We need to attempt to load the sound buffer into memory.
            sf::SoundBuffer buffer { };

            if (buffer.loadFromFile (fileLocation))
            {
                // Add the buffer to the map and return the valid ID.
                m_buffers.emplace (std::make_pair (id, buffer));

                return id;
            }
        }

        // Output an error message to inform the user or programmer of their stupidity.
        Systems::logger().logError ("AudioSFML::loadSound(): Cannot load sound with filename \"" + fileLocation + "\".");

        // Let the user cry due to lack of sounds.
        return 0;

    }


    void AudioSFML::removeSound (const SoundID sound)
    {
        // Report silly activity.
        if (m_buffers.erase (sound) == 0)
        {
            Systems::logger().logWarning ("AudioSFML::removeSound(), attempt to remove a non-existent sound buffer.");
        }
    }


    void AudioSFML::clearSoundData()
    {
        cleanUp();

        // Reset to defaults.
        m_bgm->openFromFile ("");
        adjustMusicProperties (1.f, 0.f, true);
        m_channels.resize (m_soundLimit);
    }


    //////////////
    // Playback //
    //////////////

    void AudioSFML::playMusic (const float volume, const float offset, const bool loop)
    {
        m_bgm->play();
        adjustMusicProperties (volume, offset, loop);
    }


    void AudioSFML::stopMusic()
    {
        m_bgm->stop();
    }


    void AudioSFML::resumeMusic()
    {
        m_bgm->play();
    }


    void AudioSFML::pauseMusic()
    {
        m_bgm->pause();
    }


    PlaybackID AudioSFML::playSound (const SoundID sound, const float volume, const float offset, const bool loop)
    {
        // If the buffer doesn't exist an out-of-range error will be thrown.
        const auto& buffer  = m_buffers.at (sound);

        // We need to check if we have a valid channel to play the buffer from.
        const auto channel  = findInactiveChannel();

        if (channel != std::numeric_limits<PlaybackID>::max())
        {
            // Cache the current channel.
            auto& currentChannel = m_channels[channel];

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
            m_channels[sound].stop();
        }
    }


    void AudioSFML::resumeSound (const PlaybackID sound)
    {
        if (isValidID (sound))
        {
            m_channels[sound].play();
        }
    }


    void AudioSFML::pauseSound (const PlaybackID sound)
    {
        if (isValidID (sound))
        {
            m_channels[sound].pause();
        }
    }


    void AudioSFML::stopSounds()
    {
        // Forceably stop all sounds from playing.
        auto stopFunction = [] (SFMLSound& channel) { channel.stop(); return false; };

        channelTraversal (stopFunction);
    }


    void AudioSFML::resumeSounds()
    {
        // Resume all sounds from where they left off.
        auto resumeFunction = [] (SFMLSound& channel) { channel.play(); return false; };

        channelTraversal (resumeFunction);
    }


    void AudioSFML::pauseSounds()
    {
        // Pause all sounds at their current position.
        auto pauseFunction = [] (SFMLSound& channel) { channel.pause(); return false; };

        channelTraversal (pauseFunction);
    }


    //////////////////////
    // Sound properties //
    //////////////////////

    void AudioSFML::adjustMusicMixer (const float volume)
    {
        // Clamp the value between 0 and 1.
        m_bgmMixer = util::clamp (volume, 0.f, 1.f);
        m_bgm->setVolume (100.f * m_bgmVolume * m_bgmMixer);
    }


    void AudioSFML::adjustEffectsMixer (const float volume)
    {
        // Clamp the value between 0 and 1.
        m_sfxMixer = util::clamp (volume, 0.f, 1.f);

        // Now we need to create a function that will adjust the volume of each channel.
        auto volumeAdjust =
        [=] (SFMLSound& channel)
        {
            channel.resetVolume (m_sfxMixer);
            return false;
        };

        channelTraversal (volumeAdjust);
    }


    void AudioSFML::adjustMusicProperties (const float volume, const float offset, const bool loop)
    {
        // Set the volume.
        m_bgmVolume   = util::clamp (volume, 0.f, 1.f);
        m_bgm->setVolume (100.f * m_bgmVolume * m_bgmMixer);

        // Adjust the offset.
        const auto cap      = m_bgm->getDuration().asSeconds();
        const auto position = util::clamp (offset, 0.f, cap);
        m_bgm->setPlayingOffset (sf::seconds (position));

        // And tell it to loop!
        m_bgm->setLoop (loop);
    }


    void AudioSFML::adjustSoundProperties (const PlaybackID sound, const float volume, const float offset, const bool loop)
    {
        // Silently ignore programmer/user error.
        if (isValidID (sound))
        {
            // Cache the channel and adjust each property.
            auto& channel = m_channels[sound];

            channel.setVolume (volume, m_sfxMixer);
            channel.setOffset (offset);
            channel.setLooping (loop);
        }
    }


    /////////////
    // Utility //
    /////////////

    void AudioSFML::cleanUp()
    {
        // Stop everything first.
        stopMusic();
        stopSounds();

        // We need to clear the sound buffers.
        m_channels.clear();
        m_buffers.clear();
    }


    template <typename Function> PlaybackID AudioSFML::channelTraversal (Function function)
    {
        // Function should be a lambda function which will be called on each channel.
        for (auto i = 0U; i < m_channels.size(); ++i)
        {
            if (function (m_channels[i]))
            {
                return i;
            }
        }

        return std::numeric_limits<PlaybackID>::max();
    }


    PlaybackID AudioSFML::findInactiveChannel()
    {
        // Make a function which will find an inactive channel.
        auto inactiveFunction = [] (SFMLSound& channel)
        {
            return channel.hasStopped();
        };

        return channelTraversal (inactiveFunction);
    }


    bool AudioSFML::isValidID (const PlaybackID id) const
    {
        return id < m_soundLimit;
    }
}
