#if !defined MILESTONE_ENTITY_INCLUDED
#define MILESTONE_ENTITY_INCLUDED


// Engine headers.
#include <World/Entities/IEntity.h>
#include <Rendering/IRenderer2D.h>
#include <Maths/Vector2D.h>



/// <summary> A basic class for achieving milestone 4, there is no other use. </summary>
class MilestoneEntity final : public IEntity
{
    public:
        /// <summary> Ensure a virtual destructor for child classes. </summary>
        ~MilestoneEntity() {}

        /// <summary> 
        /// Initialises the entity, this is where any dynamic memory allocation should occur and where dependencies should be met.
        /// <returns> Returns whether the initialisation was successful. </returns>
        /// </summary>
        bool initialise() final override;

        /// <summary> Updates the entity each frame, this is where dynamic code and simulation should occur. </summary>
        void update (const float deltaTime) final override;

        /// <summary> Renders the object on screen through the ScreenManager. </summary>
        void render (const std::shared_ptr<IRenderer2D>& renderer) final override;

        /// <summary> Delete this after milestone 4. Milestone purposes only. </summary>
        Vector2D<float>& getPosition() final override { return m_position; }

        void setTextureID (const TextureID id)                  { m_texture = id; }
        void setBlendType (const BlendType blend)               { m_blend = blend; }
        void setFrame (const Vector2D<unsigned int>& frame)     { m_frame = frame; }
        void setFrameSize (const Vector2D<unsigned int>& size)  { m_frameSize = size; }

    private:

        Vector2D<float>         m_position  {  };       //!< The position of the entity.

        TextureID               m_texture   {  };       //!< The texture ID required for rendering the entity.
        BlendType               m_blend     {  };       //!< The blending algorithm the texture should use.
        Vector2D<unsigned int>  m_frame     {  };       //!< The current animation frame to render.
        Vector2D<unsigned int>  m_frameSize {  };       //!< The total number of frames in the animation.

        float                   m_time      { 0.f };    //!< Keeps track of how much time has passed.
};


#endif // MILESTONE_ENTITY_INCLUDED