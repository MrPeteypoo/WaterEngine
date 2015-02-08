#if !defined WATER_SFML_SOUND_INCLUDED
#define WATER_SFML_SOUND_INCLUDED


// Engine headers.
#include <Audio/AudioSFML.h>


// Third party headers.
#include <SFML/Audio/Sound.hpp>


// Engine namespace.
namespace water
{
    /// <summary>
    /// A wrapper of the sf::Sound class which provides more control over the sound properties.
    /// </summary>
    class AudioSFML::Sound final
    {
        public:

            #pragma region Constructors and destructor
        
            /// <summary> Initialises the Sound with the given buffer. </summary>
            Sound (const sf::SoundBuffer& buffer) : m_sound (buffer) { }
            Sound (Sound&& move);
            Sound& operator= (Sound&& move);

            Sound()                                 = default;
            Sound (const Sound& copy)               = default;
            Sound& operator= (const Sound& copy)    = default;
            ~Sound()                                = default;        

            #pragma endregion


            #pragma region Wrapped functionality

            /// <summary> Plays the sound. </summary>
            void play()                                     { m_sound.play(); }

            /// <summary> Pauses the sound. </summary>
            void pause()                                    { m_sound.pause(); }

            /// <summary> Stops the sound from playing. </summary>
            void stop()                                     { m_sound.stop(); }

            /// <summary> Checks if the sound has finished playing. </summary>
            bool hasStopped() const                         { return m_sound.getStatus() == sf::SoundSource::Status::Stopped; }

            /// <summary> Checks if the sound is looping. </summary>
            bool isLooping() const                          { return m_sound.getLoop(); }

            /// <summary> Enable or disable looping functionality. </summary>
            void setLooping (const bool loop)               { m_sound.setLoop (true); }

            /// <summary> Sets the source that's played from the sound. </summary>SW
            void setBuffer (const sf::SoundBuffer& buffer)  { m_sound.setBuffer (buffer); }

            /// <summary> Sets the volume of the sound. </summary>
            /// <param name="volume"> A normalised value between 0 and 1. </param>
            /// <param name="mixer"> A normalised modifier to apply to the sound. </param>
            void setVolume (const float volume, const float mixer);

            /// <summary> Resets the volume of the sound based on a new mixer value. </summary>
            void resetVolume (const float mixer);

            /// <summary> Sets the offset of the sound in seconds. </summary>
            void setOffset (const float seconds);

            #pragma endregion

        private:

            #pragma region Implementation data

            float       m_volume    { 1.f };    //!< The core volume of the sound, from 0 to 1.
            sf::Sound   m_sound     { };        //!< The sound object to track.

            #pragma endregion
    };
}

#endif // WATER_SFML_SOUND_INCLUDED