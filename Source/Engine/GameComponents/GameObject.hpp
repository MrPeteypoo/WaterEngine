#if !defined WATER_GAME_OBJECT_INCLUDED
#define WATER_GAME_OBJECT_INCLUDED


// STL headers.
#include <string>


// Engine headers.
#include <Interfaces/IGameObject.hpp>
#include <Interfaces/IRenderer.hpp>
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

            #pragma region Constructors and destructor
            
            GameObject()                                    = default;
            GameObject (GameObject&& move);
            GameObject& operator= (GameObject&& move);

            GameObject (const GameObject& copy)             = default;
            GameObject& operator= (const GameObject& copy)  = default;

            // Ensure destructor is virtual.
            virtual ~GameObject() {}

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


            #pragma region Getters and setters

            /// <summary> Obtain a reference to the objects position. </summary>
            /// <returns> The position vector. </returns>
            const Vector2<float>& getPosition() const           { return m_position; }

            /// <summary> Gets a copy of the objects texture ID. </summary>
            /// <returns> The TextureID of the object. </returns>
            TextureID getBaseTextureID() const                  { return m_baseTexture; }

            /// <summary> Gets the blending mode that the object currently uses. </summary>
            /// <returns> The BlendType in use. </returns>
            BlendType getBlendType() const                      { return m_blendType; }

            /// <summary> Obtain a reference to the objects name. </summary>
            /// <returns> The name of the object. </returns>
            const std::string& getName() const                  { return m_name; }

            /// <summary> Obtain a reference to the objects tag. </summary>
            /// <returns> The tag of the object. </returns>
            const std::string& getTag() const                   { return m_tag; }

            /// <summary> Set the position of the object. </summary>
            /// <param name="position"> The new position to assign to the object. </param>
            void setPosition (const Vector2<float>& position)   { m_position = position; }

            /// <summary> Sets the texture for the object to use. </summary>
            /// <param name="texture"> The desired TextureID of the object. </param>
            /// <param name="removeCurrent"> Indicates whether the objects current texture should be removed from the IRenderer. </param>
            void setBaseTextureID (const TextureID texture, const bool removeCurrent = false);

            /// <summary> Sets the blending type of the textures used by the GameObject. </summary>
            /// <param name="type"> Specifies the blending algorithm to be used, Opaque is fastest. </param>
            void setBlendType (const BlendType type)            { m_blendType; }

            /// <summary> Sets the name of the object. </summary>
            /// <param name="name"> The value to set the name of the object to. </param>
            void setName (const std::string& name)              { m_name = name; }

            /// <summary> Sets the tag of the object. </summary>
            /// <param name="tag"> The value to set the tag of the object to. </param>
            void setTag (const std::string& tag)                { m_tag = tag; }

            #pragma endregion

        protected:

            #pragma region Implementation data

            Vector2<float>  m_position      { 0, 0 };   //!< The position of the GameObject in the world.
            TextureID       m_baseTexture   { 0 };      //!< The standard texture of the GameObject.
            BlendType       m_blendType     { };        //!< The blending type to be used by the object in rendering.
            std::string     m_name          = "";       //!< The name of the GameObject.
            std::string     m_tag           = "";       //!< The tag of the GameObject.

            #pragma endregion
    };
}


#endif