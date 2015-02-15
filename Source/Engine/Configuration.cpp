#include "Configuration.hpp"


// STL headers.
#include <exception>
#include <iostream>


// Engine headers.
#include <Utility/String.hpp>


// Third party headers.
#include <ThirdParty/pugixml.hpp>


// Engine namespace.
namespace water
{
    Configuration Configuration::fromXML (const std::string& file)
    {
        // Here we have the joy of parsing an XML document constructing a beautiful configuration object.        
        try
        {
            // Attempt to load the XML document.
            pugi::xml_document xml { };
            if (!xml.load_file (file.c_str()))
            {
                throw std::invalid_argument ("Invalid filename \"" + file + "\".");
            }
        
            // We have a working document, attempt to access the root node.
            const auto root                 = xml.child ("WaterEngine");
            auto config                     = Configuration();

            // Obtain the systems information. System names should be lower case.
            const auto systems              = root.child ("Systems");

            config.systems.audio            = util::toLower (systems.child ("Audio").text().as_string());
            config.systems.logger           = util::toLower (systems.child ("Logger").text().as_string());
            config.systems.renderer         = util::toLower (systems.child ("Renderer").text().as_string());
            config.systems.time             = util::toLower (systems.child ("Time").text().as_string());

            // Obtain initialisation information.
            const auto settings             = root.child ("Settings");
            const auto audio                = settings.child ("Audio");
            const auto logger               = settings.child ("Logger");
            const auto renderer             = settings.child ("Renderer");
            const auto time                 = settings.child ("Time");

            // Audio settings.
            config.audio.soundLimit         = audio.attribute ("SoundLimit").as_uint();
            config.audio.bgmMixer           = audio.attribute ("BGMVolume").as_float();
            config.audio.sfxMixer           = audio.attribute ("SFXVolume").as_float();

            // Logger settings.
            config.logging.file             = logger.attribute ("Output").as_string();
            config.logging.timestamp        = logger.attribute ("Timestamp").as_bool();

            // Renderer settings.
            config.rendering.screenWidth    = renderer.attribute ("ScreenWidth").as_int();
            config.rendering.screenHeight   = renderer.attribute ("ScreenHeight").as_int();
            config.rendering.internalWidth  = renderer.attribute ("InternalWidth").as_int();
            config.rendering.internalHeight = renderer.attribute ("InternalHeight").as_int();
            config.rendering.filterMode     = renderer.attribute ("FilterMode").as_int();

            // Time settings.
            config.time.physicsFPS          = time.attribute ("PhysicsFPS").as_uint();
            config.time.updateFPS           = time.attribute ("UpdateFPS").as_uint();
            config.time.minFPS              = time.attribute ("MinFPS").as_uint();

            // Success!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
            return config;
        }

        // Since we have no guarantee of the engine being initialised we hope that std::cerr will inform the programmers.
        catch (const std::exception& error)
        {
            std::cerr << "Error building a water::Configuration from file: " << error.what() << std::endl;
        }

        catch (...)
        {
            std::cerr << "An unknown error occurred whilst building a water::Configuration from file." << std::endl;
        }

        // Return a default object.
        return Configuration();
    }
}