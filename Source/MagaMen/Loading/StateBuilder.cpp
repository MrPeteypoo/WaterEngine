#include "StateBuilder.hpp"


// Game headers.
#include <MagaMen/GameObjects/StaticObject.hpp>


// Third party headers.
#include <ThirdParty/pugixml.hpp>


// Game namespace.
namespace mm
{
    bool StateBuilder::constructStaticObject (StaticObject& output, const pugi::xml_node& node, const std::string& workingDir)
    {
        // Construct an object to fill with data.
        StaticObject fill   { };

        // Initialise the object before modifying its data.
        fill.initialise();

        // Check the node is valid.
        if (std::string (node.name()) != "StaticObject")
        {
            output = std::move (fill);
            return false;
        }

        // Obtain the information contained by the node.
        const auto name     = node.attribute ("Name").as_string();
        const auto tag      = node.attribute ("Tag").as_string();
        const auto texture  = workingDir + node.attribute ("Texture").as_string();
        const auto blend    = node.attribute ("Blend").as_int();
        const auto posX     = node.attribute ("X").as_float();
        const auto posY     = node.attribute ("Y").as_float();

        // Set the data to the corresponding values.
        fill.setName (name);
        fill.setTag (tag);
        fill.loadBaseTexture (texture);
        fill.setBlendType ((water::BlendType) blend);
        fill.setPosition ({ posX, posY });

        // Output the loaded object.
        output = std::move (fill);
        return true;
    }
}