#if !defined IENTITY_INCLUDED
#define IENTITY_INCLUDED


// Required headers.
#include <memory>       // Used for render().


// Forward declarations.
class ScreenManager;    // Declaration required for render().


/// <summary> An interface for every entity in the game, allows for easy initialisation, updating and rendering of all entities. </summary>
class IEntity 
{
    /// <summary> Ensure a virtual destructor for child classes. </summary>
    virtual ~IEntity() = 0;

    /// <summary> 
    /// Initialises the entity, this is where any dynamic memory allocation should occur and where dependencies should be met.
    /// <returns> Returns whether the initialisation was successful. </returns>
    /// </summary>
    virtual bool initialise() = 0;

    /// <summary> Updates the entity each frame, this is where dynamic code and simulation should occur. </summary>
    virtual void update (const float deltaTime) = 0;

    /// <summary> Renders the object on screen through the ScreenManager. </summary>
    virtual void render (const std::shared_ptr<ScreenManager>& screenManager) = 0;
};


#endif // IENTITY_INCLUDED