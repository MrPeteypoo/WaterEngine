#include "SFMLSound.h"


// Engine headers.
#include <Utility/Maths.h>


// Third party headers.
#include <SFML/Audio/SoundBuffer.hpp>


// Engine namespace.
namespace water
{
    #pragma region Constructors

    AudioSFML::Sound::Sound (Sound&& move)
    {
        *this = std::move (move);
    }


    AudioSFML::Sound& AudioSFML::Sound::operator= (Sound&& move)
    {
        if (this != &move)
        {
            m_volume = move.m_volume;
            m_sound  = std::move (m_sound);

            // Reset primitives.
            m_volume = 0.f;
        }

        return *this;
    }

    #pragma endregion


    #pragma region Wrapped functionality

    void AudioSFML::Sound::setVolume (const float volume, const float mixer)
    {
        // Assume the audio system is passing us good values.
        m_volume = volume;

        // Use the reset function to change the sounds volume.
        resetVolume (mixer);
    }


    void AudioSFML::Sound::resetVolume (const float mixer)
    {
        // Apply the current volume and mixer together.
        m_sound.setVolume (100.f * m_volume * mixer);
    }


    void AudioSFML::Sound::setOffset (const float seconds)
    {
        // We should clamp the offset given.
        const auto duration = m_sound.getBuffer()->getDuration().asSeconds();

        const auto offset   = util::clamp (seconds, 0.f, duration);

        m_sound.setPlayingOffset (sf::seconds (offset));
    }

    #pragma endregion
}