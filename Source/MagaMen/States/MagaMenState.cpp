#include "MagaMenState.hpp"


// Game namespace.
namespace mm
{
    #pragma region Static members

    water::TextureID MagaMenState::letters = 0;
    water::TextureID MagaMenState::numbers = 0;

    #pragma endregion


    #pragma region Constructors

    MagaMenState::MagaMenState (MagaMenState&& move)
    {
        *this = std::move (move);
    }


    MagaMenState& MagaMenState::operator= (MagaMenState&& move)
    {
        if (this != &move)
        {
            GameState::operator= (std::move (move));

            m_dataFile  = std::move (move.m_dataFile);
            m_bgmFile   = std::move (move.m_bgmFile);
            m_bgmVolume = std::move (move.m_bgmVolume);

            // Reset primitives.
            move.m_bgmVolume = 0.f;
        }

        return *this;
    }

    #pragma endregion

}