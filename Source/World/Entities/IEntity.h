#if !defined IENTITY_INCLUDED
#define IENTITY_INCLUDED


// Forward declarations.
class IRenderer;                                            // Declaration required for render().
namespace std { template <typename T> class shared_ptr; }   // Declaration required for render().

template <typename T> struct Vector2D;                      // Required for returning position.



/// <summary> An interface for every entity in the game, allows for easy initialisation, updating and rendering of all entities. </summary>
class IEntity 
{
    public:
        /// <summary> Ensure a virtual destructor for child classes. </summary>
        virtual ~IEntity() {}

        /// <summary> 
        /// Initialises the entity, this is where any dynamic memory allocation should occur and where dependencies should be met.
        /// <returns> Returns whether the initialisation was successful. </returns>
        /// </summary>
        virtual bool initialise() = 0;

        /// <summary> Updates the entity each frame, this is where dynamic code and simulation should occur. </summary>
        virtual void update (const float deltaTime) = 0;

        /// <summary> Renders the object on screen through the ScreenManager. </summary>
        virtual void render (const std::shared_ptr<IRenderer>& renderer) = 0;

        /// <summary> Delete this after milestone 4. Milestone purposes only. </summary>
        virtual Vector2D<float>& getPosition() = 0;
};


#endif // IENTITY_INCLUDED