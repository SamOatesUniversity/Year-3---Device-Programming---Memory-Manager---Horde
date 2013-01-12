#include "lasagne-entity.h"

CLasagneEntity::CLasagneEntity(
        const char* imagePath,
        const TVector<int, 2> &screenSize
    ) :
    m_image(NULL),
    m_noofFramesX(0),
    m_noofFramesY(0),
    m_currentFrame(0),
    m_rotation(0),
	m_rendeDepth(0),
	m_visible(true)
{
    m_image = IMG_Load(imagePath);
    if (m_image == NULL)
    {
        SHOWLOGERROR("Failed to load image");
    }

    m_screenPosition.Set(0.0f, 0.0f);
}

CLasagneEntity::CLasagneEntity(
        const char* imagePath,
        const TVector<int, 2> &screenSize,
        const TVector<int, 2> &noofFrames
    ) :
    m_image(NULL),
    m_noofFramesX(noofFrames.x()),
    m_noofFramesY(noofFrames.y()),
    m_currentFrame(0),
	m_rotation(0),
	m_rendeDepth(0),
	m_visible(true)
{
    m_image = IMG_Load(imagePath);
    if (m_image == NULL)
    {
        SHOWLOGERROR("Failed to load image");
    }
    else
    {
        m_frameSize.Set(m_image->w / m_noofFramesX, m_image->h / m_noofFramesY);
    }

    m_screenPosition.Set(0.0f, 0.0f);

    m_lastFrameTime = SDL_GetTicks();
}

CLasagneEntity::~CLasagneEntity()
{

}

void CLasagneEntity::Render(
        SDL_Surface *screen
    )
{
	if (!m_visible)
		return;

    if (m_noofFramesX + m_noofFramesY == 0)
    {
         // draw the entity in its 3d position
        SDL_Rect rcRect;
        rcRect.x = static_cast<Sint16>(m_screenPosition.x());
        rcRect.y = static_cast<Sint16>(m_screenPosition.y());

        SDL_BlitSurface(m_image, NULL, screen, &rcRect);
    }
    else
    {
        SDL_Rect srcRect;
        srcRect.w = m_frameSize.x();
        srcRect.h = m_frameSize.y();

        int xOffset = m_currentFrame;
        int yOffset = 0;
        while (xOffset > m_noofFramesX - 1)
        {
            yOffset++;
            xOffset -= m_noofFramesX;
        }

        // update aniamtion at 20fps
        if (SDL_GetTicks() - m_lastFrameTime > 50)
        {
            m_lastFrameTime = SDL_GetTicks();

            m_currentFrame++;

            if (m_currentAnimation.length() == 0)
            {
                if (m_currentFrame >= (m_noofFramesX * m_noofFramesY))
					m_currentFrame = 0;
            }
            else
            {
				const AnimationData &animData = m_animation[m_currentAnimation];
                IVec2 frames = animData.frames;
                if (m_currentFrame >= frames.y())
				{
					if (animData.loop)
						m_currentFrame = frames.x();
					else
						m_currentFrame = frames.y() - 1;
				}
            }
        }

        srcRect.x = xOffset * m_frameSize.x();
        srcRect.y = yOffset * m_frameSize.y();

        SDL_Surface *frameSurface = SDL_CreateRGBSurface(0, m_frameSize.x(), m_frameSize.y(), 32, 0, 0, 0, 0);

        SDL_Rect rcRect;
        rcRect.x = 0;
        rcRect.y = 0;
        rcRect.w = m_frameSize.x();
        rcRect.h = m_frameSize.y();
        SDL_BlitSurface(m_image, &srcRect, frameSurface, &rcRect);
        SDL_Flip(frameSurface);

        SDL_Surface *rotateFrame = rotozoomSurface(frameSurface, m_rotation, 1, 0);
        SDL_SetColorKey(rotateFrame, SDL_SRCCOLORKEY, SDL_MapRGB(rotateFrame->format, 0, 0, 0));

        rcRect.x = static_cast<Sint16>(m_screenPosition.x());
        rcRect.y = static_cast<Sint16>(m_screenPosition.y());
        SDL_BlitSurface(rotateFrame, NULL, screen, &rcRect);

        SDL_FreeSurface(frameSurface);
        SDL_FreeSurface(rotateFrame);
    }
}

bool CLasagneEntity::AddAnimation(
        const char *name,               //!<
        const int frameStart,           //!<
        const int frameEnd,             //!<
		const bool loop
    )
{
	AnimationData data;
	data.frames.Set(frameStart, frameEnd);
	data.loop = loop;

    m_animation[name] = data;

    return true;
}

void CLasagneEntity::SetCurrentAnimation(
        char *animation
    )
{
    if (strcmp(animation, m_currentAnimation.c_str()) != 0)
    {
        IVec2 frames = m_animation[animation].frames;
        m_currentFrame = frames.x();
    }

    m_currentAnimation = animation;
}

void CLasagneEntity::SetVisible(
		bool visible
	)
{
	m_visible = visible;
}

const bool CLasagneEntity::Intersects( 
		CLasagneEntity *other 
	)
{
	TVector<float, 2> otherPosition = other->GetPosition();
	TVector<int, 2> otherSize = other->GetFrameSize();

	if (m_screenPosition.x() > otherPosition.x() + otherSize.x())
		return false;

	if (m_screenPosition.x() + m_frameSize.x() < otherPosition.x())
		return false;

	if (m_screenPosition.y() > otherPosition.y() + otherSize.y())
		return false;

	if (m_screenPosition.y() + m_frameSize.y() < otherPosition.y())
		return false;

	return true;
}
