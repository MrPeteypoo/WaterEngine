#include "MagaMenLevelState.hpp"


// Game namespace.
namespace mm
{
    #pragma region Constructors

    MagaMenLevelState::MagaMenLevelState (MagaMenLevelState&& move)
    {
        *this = std::move (move);
    }


    MagaMenLevelState& MagaMenLevelState::operator= (MagaMenLevelState&& move)
    {
        if (this != &move)
        {
            MagaMenState::operator= (std::move (move));
        }

        return *this;
    }

    #pragma endregion
}