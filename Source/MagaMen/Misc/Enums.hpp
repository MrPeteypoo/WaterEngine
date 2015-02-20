#if !defined MAGA_MEN_MISC_ENUMS_INCLUDED
#define MAGA_MEN_MISC_ENUMS_INCLUDED


// Game namespace.
namespace mm
{
    /// <summary
    /// A collection of input actions that can be performed in the game.
    /// </summary>
    enum class Action : int
    {
        Up,     //!< Move up.
        Down,   //!< Move down.
        Left,   //!< Move left.
        Right,  //!< Move right.
        Shoot,  //!< Shoot projectiles.
        Start,  //!< Start/pause the game.
        Reset   //!< Reset the game.
    };


    /// <summary>
    /// An enumeration representing every collidable layer in the game.
    /// </summary>
    enum class CollisionLayer : unsigned int
    {
        Environment,    //!< All objects should collide with the environment.
        Player,         //!< The Player layer should only collide with the environment and enemies.
        Enemy           //!< Enemies collide with the player layer and the environment.
    };


    /// <summary>
    /// A simple enumeration containing the ID of every state in the game. Values here can be cast to ints to specify which state to load.
    /// </summary>
    enum class StateID : int
    {
        MainMenu    = 0,    //!< The entry point of the game.
        Credits     = 2,    //!< The final state of the game once the player has won.
        CutManStage = 3     //!< The stage where the player fights Cut Man.
    };
}


#endif