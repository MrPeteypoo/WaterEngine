#if !defined INTERFACE_RENDERER_INCLUDED
#define INTERFACE_RENDERER_INCLUDED


// Forward declarations.
struct Colour;
template <typename T> class Vector2D;

using Byte = unsigned char;
namespace std { struct string; }



/// <summary> An interface for 2D rendering systems, used for platform-independant drawing functionality. </summary>
class IRenderer2D
{
    public:
        // Ensure destructor is virtual since this is an interface.
        virtual ~IRenderer2D() = 0;
    
        /// <summary> Initialise all data and prepare for rendering. </summary>
        virtual bool initialise() = 0;

        /// <summary> Clears the screen to a black level between 0 and 255, quicker than clearing to a colour. </summary>
        virtual void clearToBlack (const Byte blackLevel = 0) = 0;        

        /// <summary> Clears the entire screen to a single colour. </summary>
        virtual void clearToColour (const Colour& colour) = 0;

        /// <summary>
        /// Loads a texture from local storage, ready for rendering. 
        /// <returns> Returns the ID for the loaded texture. </returns>
        /// </summary>
        virtual int loadTexture (const std::string& fileLocation) = 0;

        /// <summary> 
        /// Requests that a texture be drawn onto the screen at a particular point. 
        /// </summary>
        /// <param name="position"> The top-left point where the texture should render from. </param>
        /// <param name="id"> The ID of the texture to render. </param>
        /// <param name="frame"> Which frame to render from the texture. If no frames exist the entire texture will be drawn. </param>
        virtual void drawTexture (const Vector2D<int>& point, const int id, const int frame = 0) = 0;
};


#endif // INTERFACE_RENDERER_INCLUDED