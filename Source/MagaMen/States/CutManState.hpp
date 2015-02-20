#if !defined MAGA_MEN_CUT_MAN_STAGE_INCLUDED
#define MAGA_MEN_CUT_MAN_STAGE_INCLUDED


// Game headers.
#include <MagaMen/States/MagaMenState.hpp>


// Game namespace.
namespace mm
{
    /// <summary>
    /// The level where the player has to defeat Cut Man. This is where the majority of gameplay is.
    /// </summary>
    class CutManState final : public MagaMenState
    {
        public:

            #pragma region Constructors and destructor

            /// <summary> A simple constructor which specifies the .XML file to load the states data from. </summary>
            /// <param name="loadFrom"> An .XML file containing information the state needs to load from. </param>
            CutManState (const std::string& loadFrom) : MagaMenState (loadFrom) { }

            CutManState (CutManState&& move);
            CutManState& operator= (CutManState&& move);
            ~CutManState() override final { }
            
            CutManState (const CutManState& copy)               = delete;
            CutManState& operator= (const CutManState& copy)    = delete;

            #pragma endregion


            #pragma region Game flow

            /// <summary> Loads all of the graphics required by the title screen. Also clears previous data. </summary>
            /// <returns> Whether the loading was successful or not. </returns>
            bool onAdd() override final;

            /// <summary> Removes all graphical and audio data from the game. </summary>
            /// <returns> If something went wrong and an error should be flagged. </returns>
            bool onRemove() override final;

            /// <summary> Plays the title music. </summary>
            void onEntry() override final;

            /// <summary> Stops the music from playing. </summary>
            void onExit() override final;

            /// <summary> Absolutely nothing. </summary>
            void updatePhysics() override final { }

            /// <summary> Checks if the player is ready to start the game or not. </summary>
            void update() override final;

            /// <summary> Renders the entire title screen! </summary>
            void render() override final;

            #pragma endregion

        private:

            #pragma region Implementation data

            // Allow the construction of the state externally.
            friend class CutManStateBuilder;

            //std::vector<

            #pragma endregion
    };
}


#endif