#if !defined MAGA_MEN_STATE_ID_INCLUDED
#define MAGA_MEN_STATE_ID_INCLUDED


// Game namespace.
namespace mm
{
    /// <summary>
    /// A simple enumeration containing the ID of every state in the game. Values here can be cast to ints to specify which state to load.
    /// </summary>
    enum class StateID : int
    {
        MainMenu    = 0,    //!< The entry point of the game.
        LevelSelect = 1,    //!< The state where the player can select which stage to play.
        Credits     = 2,    //!< The final state of the game once the player has won.
        CutManStage = 3     //!< The stage where the player fights Cut Man.
    };
}


#endif