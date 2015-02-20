#if !defined MAGA_MEN_STATE_INCLUDED
#define MAGA_MEN_STATE_INCLUDED


// STL headers.
#include <string>


// Engine headers.
#include <Engine/GameComponents/GameState.hpp>
#include <Engine/Interfaces/IAudio.hpp>


// Game headers.
#include <MagaMen/Misc/Enums.hpp>


// Forward declarations.
namespace water { using TextureID = size_t; }


// Game namespace.
namespace mm
{
    /// <summary
    /// An abstract class featuring common functionality and data required by all Maga Men game states.
    /// </summary>
    class MagaMenState : public water::GameState
    {
        public: 

            #pragma region Constructors and destructor

            /// <summary> A simple constructor which specifies the .XML file to load the states data from. </summary>
            /// <param name="loadFrom"> An .XML file containing information the state needs to load from. </param>
            MagaMenState (const std::string& loadFrom) : GameState(), m_dataFile (loadFrom) { }

            MagaMenState()                                      = default;
            MagaMenState (const MagaMenState& copy)             = default;
            MagaMenState& operator= (const MagaMenState& copy)  = default;

            MagaMenState (MagaMenState&& move);
            MagaMenState& operator= (MagaMenState&& move);

            // Ensure destructor is virtual due to being an abstract class.
            virtual ~MagaMenState() { }

            #pragma endregion

        protected:

            #pragma region Implementation data

            std::string             m_dataFile  = "Nothing.xml";    //!< The file to load from.
            std::string             m_bgmFile   = "Nothing.ogg";    //!< The ID of the backing track of the state.
            float                   m_bgmVolume { 1.f };            //!< The volume of the background music.

            static water::TextureID letters;                        //!< The ID of the texture to be used for graphical letters.
            static water::TextureID numbers;                        //!< The ID of the texture to be used for graphical numbers.

            #pragma endregion
    };
}


#endif