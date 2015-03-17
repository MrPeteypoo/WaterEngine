#if !defined WATER_INTERFACE_PHYSICS_INCLUDED
#define WATER_INTERFACE_PHYSICS_INCLUDED


// Engine namespace.
namespace water
{
    /// <summary>
    /// An interface to every physics system in the water engine. Physics systems use layer masks to
    /// determine collision. This means each individual bit of an unsigned integer represents a collidable
    /// layer.
    /// </summary>
    class IPhysics
    {
        public:

            // Ensure destructor is virtual.
            virtual ~IPhysics() { }


            //////////////////////
            // Layer management //
            //////////////////////

            /// <summary> Set the layer mask to use for a given layer. This will effect what objects collide with it. </summary
            /// <param name="layer"> The layer to set the layer mask of. Must be lower than 32. </param>
            /// <param name="collidable"> The layermask to use for collision detection, each individual bit represents a layer. </param>
            virtual void setLayerMask (const unsigned int layer, const unsigned int collidable) = 0;

            /// <summary> Adds a layer to the collidable layers of a layer. This maintains the other collisions. </summary>
            /// <param name="layerToModify"> The layer to have its collision mask changed. Must be lower than 32. </param>
            /// <param name="layerToAdd"> The layer to add to the collision mask, must be lower than 32. </param>
            virtual void addToMask (const unsigned int layerToModify, const unsigned int layerToAdd) = 0;

            /// <summary> Removes a layer from the collidable layers of a layer. This maintains the other collisions. </summary>
            /// <param name="layerToModify"> The layer to have its collision mask changed. Must be lower than 32. </param>
            /// <param name="layerToAdd"> The layer to remove from the collision mask, must be lower than 32. </param>
            virtual void removeFromMask (const unsigned int layerToModify, const unsigned int layerToAdd) = 0;
    };
}

#endif
