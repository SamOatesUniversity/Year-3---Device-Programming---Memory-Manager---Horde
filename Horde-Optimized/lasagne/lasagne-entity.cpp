#include "lasagne-entity.h"

CLasagneEntity::CLasagneEntity(
        const char* imagePath,
        const TVector<int, 2> &screenSize
    ) :
    m_image(NULL),
    m_rotation(0),
    m_noofFramesX(0),
    m_noofFramesY(0),
    m_currentFrame(0),
    m_fps(75),
    m_visible(true),
	m_rotateFrame(NULL)
{
    m_image = IMG_Load(imagePath);
    if (m_image == NULL)
    {
        SHOWLOGERROR("Failed to load image");
    }
	else
	{
		m_frameSize.x = 0;
		m_frameSize.y = 0;
		m_frameSize.w = m_image->w;
		m_frameSize.h = m_image->h;

		m_flags.allFlags = 0;
		m_flags.animated = false;

		m_name = imagePath;
		m_name = m_name.substr(m_name.find_last_of("/") + 1);

		m_screenPosition.Set(0.0f, 0.0f);
	}
}

CLasagneEntity::CLasagneEntity(
        const char* imagePath,
        const TVector<int, 2> &screenSize,
        const TVector<int, 2> &noofFrames
    ) :
    m_image(NULL),
    m_rotation(0),
    m_noofFramesX(noofFrames.x()),
    m_noofFramesY(noofFrames.y()),
    m_currentFrame(0),
    m_fps(75),
    m_visible(true),
	m_rotateFrame(NULL)
{
    m_image = IMG_Load(imagePath);
    if (m_image == NULL)
    {
        SHOWLOGERROR("Failed to load image");
    }
    else
    {
        m_frameSize.x = 0;
		m_frameSize.y = 0;
		m_frameSize.w = m_image->w / m_noofFramesX;
		m_frameSize.h = m_image->h / m_noofFramesY;

		m_flags.allFlags = 0;
		m_flags.animated = true;

		m_name = imagePath;
		m_name = m_name.substr(m_name.find_last_of("/") + 1);
    }

    m_screenPosition.Set(0.0f, 0.0f);

    m_lastFrameTime = SDL_GetTicks();
}

CLasagneEntity::~CLasagneEntity()
{
	SDL_FreeSurface(m_rotateFrame);
}

void CLasagneEntity::Render(
        SDL_Surface *screen
    )
{
	if (!m_visible)
		return;

	if (!IsOnScreen())
		return;

	if (!m_flags.animated)
    {
         // draw the entity in its 3d position
        static SDL_Rect rcRect;
        rcRect.x = static_cast<Sint16>(m_screenPosition.x());
        rcRect.y = static_cast<Sint16>(m_screenPosition.y());

        SDL_BlitSurface(m_image, NULL, screen, &rcRect);
    }
    else
    {

        // update aniamtion
        if (SDL_GetTicks() - m_lastFrameTime > m_fps)
        {
            m_lastFrameTime = SDL_GetTicks();

            m_currentFrame++;

            if (m_currentAnimation.length() == 0)
            {
                if (m_currentFrame >= (m_noofFramesX * m_noofFramesY)) {
					m_currentFrame = 0;
				}
            }
            else
            {
				const AnimationData &animData = m_animation[m_currentAnimation];
                IVec2 frames = animData.frames;
                if (m_currentFrame >= frames.y())
				{
					if (animData.loop) {
						m_currentFrame = frames.x();
					} else {
						m_currentFrame = frames.y() - 1;
					}
				}
            }

			SDL_FreeSurface(m_rotateFrame);

			int xOffset = m_currentFrame;
			int yOffset = 0;
			while (xOffset > m_noofFramesX - 1)
			{
				yOffset++;
				xOffset -= m_noofFramesX;
			}

			const int pixelOffset = ((m_image->w * m_frameSize.h) * yOffset) + (m_frameSize.w * xOffset);

			SDL_PixelFormat *const fmt = m_image->format;
			SDL_Surface *const frame = SDL_CreateRGBSurfaceFrom(
				(void*)((unsigned int*)m_image->pixels + pixelOffset),
				m_frameSize.w, m_frameSize.h, fmt->BitsPerPixel, m_image->pitch,
				fmt->Rmask, fmt->Gmask, fmt->Bmask, fmt->Amask
			);

			if (m_rotation != 0.0f) {
				m_rotateFrame = rotozoomSurface(frame, m_rotation, 1, 0);
				SDL_FreeSurface(frame);
			} else {
				m_rotateFrame = frame;
			}
        }

		static SDL_Rect rcRect;
        rcRect.x = static_cast<Sint16>(m_screenPosition.x());
        rcRect.y = static_cast<Sint16>(m_screenPosition.y());
        SDL_BlitSurface(m_rotateFrame, NULL, screen, &rcRect);
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

	if (m_screenPosition.x() + m_frameSize.w < otherPosition.x())
		return false;

	if (m_screenPosition.y() > otherPosition.y() + otherSize.y())
		return false;

	if (m_screenPosition.y() + m_frameSize.h < otherPosition.y())
		return false;

	return true;
}


bool CLasagneEntity::IsOnScreen() const
{
	if (m_screenPosition.x() > 320)
		return false;

	if (m_screenPosition.x() + m_frameSize.w < 0)
		return false;

	if (m_screenPosition.y() > 240)
		return false;

	if (m_screenPosition.y() + m_frameSize.h < 0)
		return false;

	return true;
}
