#if !defined BLEND_TYPE_INCLUDED
#define BLEND_TYPE_INCLUDED


/// <summary> Represents how an image should be blended when blit using the ScreenManager. </summary>
enum class BlendType : int
{
    Opaque = 0,         //!< Enables fast blitting by disregarding alpha values.
    Transparent = 1     //!< Enables alpha blending when blitting, slower but allows for transparency.
};


#endif // BLEND_TYPE_INCLUDED