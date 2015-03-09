#if !defined WATER_COLLIDER_INCLUDED
#define WATER_COLLIDER_INCLUDED


// Engine headers.
#include <Misc/Rectangle.hpp>


// Engine namespace.
namespace water
{
    /// <summary>
    /// A basic structure containing 2D physics information. The collider class enforces correct positioning of
    /// the rectangular collision box.
    /// </summary>
    class Collider final
    {
        public:

            ///////////////////////////////////
            /// Constructors and destructor ///
            ///////////////////////////////////

            Collider()                                  = default;
            Collider (const Collider& copy)             = default;
            Collider& operator= (const Collider& copy)  = default;
            ~Collider()                                 = default;

            Collider (Collider&& move);
            Collider& operator= (Collider&& move);


            ///////////////////////////
            /// Getters and setters ///
            ///////////////////////////

            /// <summary> Obtain a reference to the box of the collider. </summary>
            const Rectangle<float>& getBox() const  { return m_box; }

            /// <summary> Obtains the layer ID of the collider. </summary>
            unsigned int getLayer() const           { return m_layer; }

            /// <summary> Indicates whether the collider is a trigger collider. </summary>
            bool isTrigger() const                  { return m_isTrigger; }

            /// <summary> Sets the bounding box of the collider, this should be the size and offset of the collider. </summary>
            /// <param name="box"> The desired collision box, this will be checked for validity and ignored if invalid. </param>
            void setBox (const Rectangle<float>& box);

            /// <summary> Sets the layer of the collider, this determines which objects it should collide with. </summary>
            /// <param name="layer"> This value must range from 0 to 31 to be valid. </param>
            void setLayer (const unsigned int layer);

            /// <summary> Sets whether the collider is a trigger or not. Triggers will allow other objects to pass through them. </summary>
            /// <param name="isTrigger"> Whether the collider should be a trigger or not. </param>
            void setTrigger (const bool isTrigger)  { m_isTrigger = isTrigger; }

        private:

            ///////////////////////////
            /// Implementation data ///
            ///////////////////////////

            Rectangle<float>    m_box       { 0, 0, 1, 1 }; //!< The collider offset and size.
            unsigned int        m_layer     { 0 };          //!< The layer of the collider, is a 0 to 32 value which effects which objects collide with it.
            bool                m_isTrigger { false };      //!< Determines whether other objects can enter the collision area and how the collider is handled.
    };
}

#endif
