#if !defined WATER_CONFIGURATION_INCLUDED
#define WATER_CONFIGURATION_INCLUDED


// STL headers.
#include <string>


// Engine namespace.
namespace water
{
    /// <summary>
    /// A simple structing containing the configuration information required to initialise the engine.
    /// </summary>
    struct Configuration final
    {

        ////////////////////////////
        // Configuration segments //
        ////////////////////////////

        /// <summary> A structure containing information on which systems should be used by the engine. </summary>
        struct Systems final
        {
            std::string     audio           = "sfml";   //!< The audio system to use (sfml).
            std::string     input           = "sfml";   //!< The input system to use (sfml).
            std::string     logger          = "stl";    //!< The logging system to use (stl).
            std::string     renderer        = "sfml";   //!< The renderer to use (sfml).
            std::string     time            = "stl";    //!< The time system to use (stl).
            std::string     window          = "sfml";   //!< The window management system to use (sfml).
        };

        /// <summary> Initialisation settings for audio systems. </summary>
        struct Audio final
        {
            unsigned int    soundLimit      { 32 };     //!< The maximum number of concurrent sounds available.
            float           bgmMixer        { 1 };      //!< The mixer for background music.
            float           sfxMixer        { 1 };      //!< The mixer for sound effects.
        };

        /// <summary> Initialisation settings for logging systems. </summary>
        struct Logging final
        {
            std::string     file            = "log";    //!< The name of the file to be used by logging systems, if applicable.
            bool            timestamp       { true };   //!< Whether log messages should be timestamped.
        };

        /// <summary> Initialisation settings for rendering systems. </summary>
        struct Rendering final
        {
            int             internalWidth   { 240 };    //!< The width of the internal resolution.
            int             internalHeight  { 232 };    //!< The height of the internal resolution.
            int             filterMode      { 0 };      //!< The desired filtering mode to use during upscaling.
        };

        /// <summary> Initialisation settings for time systems. </summary>
        struct Time final
        {
            unsigned int    physicsFPS      { 60 };     //!< The desired frame rate of the physics system.
            unsigned int    updateFPS       { 0 };      //!< The desired frame rate for the standard update.
            unsigned int    minFPS          { 10 };     //!< The frame rate at which the game will start slowing down instead of increasing the time step.
        };

        /// <summary> Initialsation settings for window systems. </summary>
        struct Window final
        {
            unsigned int    width           { 800 };    //!< The desired width of the window.
            unsigned int    height          { 600 };    //!< The desired height of the window.
            bool            fullscreen      { false };  //!< Whether the window should be fullscreen or not.
            std::string     title           = "Water";  //!< The desired title that the window should display.
        };


        ///////////////////
        // Public access //
        ///////////////////

        /// <summary> Loads an XML file and creates a configuration from that. </summary>
        /// <param name="file"> The location of the .xml configuration file. </param>
        /// <returns> The configuration stored in the XML document. If the XML is invalid a default object is returned. </returns>
        static Configuration fromXML (const std::string& file);


        Systems     systems     { };    //!< A structure containing information on which systems should be used by the engine.
        Audio       audio       { };    //!< Initialisation settings for audio systems.
        Logging     logging     { };    //!< Initialisation settings for logging systems.
        Rendering   rendering   { };    //!< Initialisation settings for rendering systems.
        Time        time        { };    //!< Initialisation settings for time systems.
        Window      window      { };    //!< Initialisation settings for window systems.
    };
}

#endif
