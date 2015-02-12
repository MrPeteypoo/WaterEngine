#if !defined WATER_GAME_OBJECT_INCLUDED
#define WATER_GAME_OBJECT_INCLUDED


// Engine headers.
#include <Interfaces/IGameObject.hpp>
#include <Misc/Vector2.hpp>


// Engine namespace.
namespace water
{
    // Forward declarations.
    class IAudio;
    class IInput;
    class ILogger;
    class IRenderer;
    class ITime;


    /// <summary>
    /// A basic abstract game object class with the common functionality required by all objects in the game.
    /// </summary>
    class GameObject : public IGameObject
    {
        public:

            // Ensure destructor is virtual.
            virtual ~GameObject() {}


            #pragma region Getters and setters

            /// <summary> Obtain a reference to the objects position. </summary>
            /// <returns> The position vector. </returns>
            const Vector2<float>& getPosition() const           { return m_position; }

            /// <summary> Set the position of the physics object. </summary>
            /// <param name="position"> The new position to assign to the object. </param>
            void setPosition (const Vector2<float>& position)   { m_position = position; }

            #pragma endregion


            #pragma region Commonly accessed systems

            /// <summary> Obtains a reference to the IAudio system used by the engine. </summary>
            static IAudio& audio();

            /// <summary> Obtains a reference to the IInput system used by the engine. </summary>
            static IInput& input();

            /// <summary> Obtains a reference to the ILogger system used by the engine. </summary>
            static ILogger& logger();

            /// <summary> Obtains a reference to the IRenderer system used by the engine. </summary>
            static IRenderer& renderer();

            /// <summary> Obtains a reference to the ITime system used by the engine. </summary>
            static ITime& time();

            #pragma endregion

        private:

            #pragma region Implementation data

            Vector2<float>  m_position  { 0, 0 };   //!< The position of the GameObject in the world.

            #pragma endregion
    };
}


#endif