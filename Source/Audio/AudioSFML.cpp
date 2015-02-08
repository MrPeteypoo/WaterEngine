#include "AudioSFML.h"


// STL headers.
#include <map>
#include <unordered_map>


// Engine headers.
#include <Audio/Sound.h>


// Third party headers.
#include <SFML/Audio.hpp>



#pragma region Implmentation data

/// <summary> Contains the implementation data for the AudioSystemSFML class. </summary>
struct AudioSFML::Impl final
{
    size_t                                          soundLimit  { 31 };     //!< The number of sound effect channels available concurrently.
    float                                           sfxMixer    { 1.f };    //!< The effects mixer, this is the volume applied to every sound effect.
    float                                           bgmMixer    { 1.f };    //!< The background music mixer, this is how loud the music is.

    sf::Music                                       bgm         { };        //!< The loaded background music.
    float                                           bgmVolume   { 1.f };    //!< The core volume of the music file.

    std::hash<std::string>                          hasher      { };        //!< The hasher used to hash file names.
    std::unordered_map<SoundID, sf::SoundBuffer>    buffers     { };        //!< A collection of sound buffers containing loaded sound effects.
    std::map<PlaybackID, Sound>                     sounds      { };        //!< A collection of currently playing sounds.
};

#pragma endregion