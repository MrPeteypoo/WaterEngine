#if !defined MILESTONE_ENTITY_INCLUDED
#define MILESTONE_ENTITY_INCLUDED


// Engine headers.
#include <World/Entities/IEntity.h>
#include <Rendering/IRenderer.h>
#include <Misc/Vector2.h>



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
        void render (const std::shared_ptr<water::IRenderer>& renderer) final override;

        /// <summary> Delete this after milestone 4. Milestone purposes only. </summary>
        Vector2<float>& getPosition() final override           { return m_position; }

        void setTextureID (const water::TextureID id)                  { m_texture = id; }
        void setBlendType (const water::BlendType blend)               { m_blend = blend; }
        void setFrame (const Vector2<unsigned int>& frame)     { m_frame = frame; }
        void setFrameSize (const Vector2<unsigned int>& size)  { m_frameSize = size; }

    private:

        Vector2<float>         m_position  {  };       //!< The position of the entity.

        water::TextureID               m_texture   {  };       //!< The texture ID required for rendering the entity.
        water::BlendType               m_blend     {  };       //!< The blending algorithm the texture should use.
        Vector2<unsigned int>  m_frame     {  };       //!< The current animation frame to render.
        Vector2<unsigned int>  m_frameSize {  };       //!< The total number of frames in the animation.

        float                   m_time      { 0.f };    //!< Keeps track of how much time has passed.
};


#endif // MILESTONE_ENTITY_INCLUDED