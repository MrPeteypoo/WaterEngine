#if !defined WATER_PHYSICS_INCLUDED
#define WATER_PHYSICS_INCLUDED


// Engine headers.
#include <Systems/IEnginePhysics.hpp>


// Engine namespace.
namespace water
{
    /// <summary>
    /// A basic physics engine which checks the rectangular collisions of each object passed to it.
    /// </summary>
    class Physics final : public IEnginePhysics
    {
        public:

            #pragma region Constructors and destructor

            Physics()                                   = default;
            Physics (const Physics& copy)               = default;
            Physics& operator= (const Physics& copy)    = default;

            Physics (Physics&& move);
            Physics& operator= (Physics&& move);
            ~Physics() override final { };

            #pragma endregion


            #pragma region System management
    
            /// <summary> Initialise the system, preparing it for checking collisions. </summary>
            void initialise() override final;

            /// <summary> Checks for collisions in all given PhysicsObject's. </summary>
            /// <param name="objects"> The objects to check collision for. </param>
            void detectCollisions (const std::vector<PhysicsObject*>& objects) override final;

            #pragma endregion


            #pragma region Layer management

            /// <summary> Set the layer mask to use for a given layer. This will effect what objects collide with it. </summary
            /// <param name="layer"> The layer to set the layer mask of. Must be lower than 32. </param>
            /// <param name="collidable"> The layermask to use for collision detection, each individual bit represents a layer. </param>
            void setLayerMask (const unsigned int layer, const unsigned int collidable) override final;

            /// <summary> Adds a layer to the collidable layers of a layer. This maintains the other collisions. </summary>
            /// <param name="layerToModify"> The layer to have its collision mask changed. Must be lower than 32. </param>
            /// <param name="layerToAdd"> The layer to add to the collision mask, must be lower than 32. </param>
            void addToMask (const unsigned int layerToModify, const unsigned int layerToAdd) override final;

            /// <summary> Removes a layer from the collidable layers of a layer. This maintains the other collisions. </summary>
            /// <param name="layerToModify"> The layer to have its collision mask changed. Must be lower than 32. </param>
            /// <param name="layerToAdd"> The layer to remove from the collision mask, must be lower than 32. </param>
            void removeFromMask (const unsigned int layerToModify, const unsigned int layerToAdd) override final;

            #pragma endregion

        private:

            #pragma region Implementation data

            std::vector<unsigned int>   m_layers    { };    //!< A collection of layer masks representing the layers each layer collides with.

            #pragma endregion
    };
}


#endif