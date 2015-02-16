#if !defined MAGA_MEN_MISC_ACTIONS_INCLUDED
#define MAGA_MEN_MISC_ACTIONS_INCLUDED


// Game namespace.
namespace mm
{
    /// <summary
    /// A collection of input actions that can be performed in the game.
    /// </summary>
    enum class Action : int
    {
        Up      = 1,    //!< Move up.
        Down    = 2,    //!< Move down.
        Left    = 3,    //!< Move left.
        Right   = 4,    //!< Move right.
        Jump    = 5,    //!< Make a character jump.
        Shoot   = 6,    //!< Shoot projectiles.
        Start   = 7,    //!< Start the game.
        Pause   = 8     //!< Pause the game.
    };
}


#endif