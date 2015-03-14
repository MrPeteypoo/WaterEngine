#include "RendererSFML.hpp"


namespace water
{

    RendererSFML::RendererSFML(const RendererSFML& other)
    {
        //copy ctor
    }


    RendererSFML& RendererSFML::operator=(const RendererSFML& rhs)
    {
        if (this == &rhs) return *this; // handle self assignment
        //assignment operator
        return *this;
    }
}
