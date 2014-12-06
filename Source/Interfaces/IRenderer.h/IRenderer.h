#if !defined INTERFACE_RENDERER_INCLUDED
#define INTERFACE_RENDERER_INCLUDED



// Forward declarations.
struct Colour;

using Byte = unsigned char;


/// <summary> An interface for 2D rendering systems, used for platform-independant drawing functionality. </summary>
class IRenderer2D
{
    /// <summary> Initialise all data and prepare for rendering. </summary>
    bool initialise();

    /// <summary> Clears the screen to a black level between 0 and 255, quicker than clearing to a colour. </summary>
    void clearToBlack (const Byte blackLevel = 0);        

    /// <summary> Clears the entire screen to a single colour. </summary>
    void clearToColour (const Colour& colour);
};


#endif // INTERFACE_RENDERER_INCLUDED