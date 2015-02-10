#include "Systems.hpp"


// Initialise the statics.
namespace water
{
    IAudio*     Systems::m_audio    = nullptr;
    IInput*     Systems::m_input    = nullptr;
    ILogger*    Systems::m_logger   = nullptr;
    IRenderer*  Systems::m_renderer = nullptr;
    ITime*      Systems::m_time     = nullptr;
}