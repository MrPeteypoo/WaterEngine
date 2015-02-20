#if !defined MAGA_MEN_STATIC_OBJECT_INCLUDED
#define MAGA_MEN_STATIC_OBJECT_INCLUDED


// Engine headers.
#include <Engine/GameComponents/GameObject.hpp>


// Game namespace.
namespace mm
{
    /// <summary>
    /// A basic GameObject decendent whose purpose is to simply exist in the scene and render itself.
    /// </summary>
    class StaticObject : public water::GameObject
    {
        public:

            #pragma region Constructors and destructor
            
            StaticObject()                                      = default;
            StaticObject (StaticObject&& move);
            StaticObject& operator= (StaticObject&& move);

            StaticObject (const StaticObject& copy)             = default;
            StaticObject& operator= (const StaticObject& copy)  = default;

            // Ensure destructor is virtual as this is widely used elsewhere.
            virtual ~StaticObject() override { }

            #pragma endregion


            #pragma region Game functionality

            /// <summary> Doesn't do anything. </summary>
            /// <returns> True. </returns>
            virtual bool initialise() override  { return true; }

            /// <summary> Does absolutely nothing. </summary>
            virtual void updatePhysics() override { }

            /// <summary> Does absolutely nothing. </summary>
            virtual void update() override { }

            /// <summary> Renders the object onto the screen at its current position. <summary>
            virtual void render() override;

            #pragma endregion


            #pragma region Texture handling

            /// <summary> Causes the object to contact the IRenderer to load a texture which the object will use. </summary>
            /// <param name="textureName"> The file name of the texture which the object should use. </param>
            /// <param name="removeCurrent"> Indicates whether the objects current TextureID should be removed from the IRenderer. </param>
            void loadBaseTexture (const std::string& textureName, const bool removeCurrent = false);

            #pragma endregion
    };
}

#endif