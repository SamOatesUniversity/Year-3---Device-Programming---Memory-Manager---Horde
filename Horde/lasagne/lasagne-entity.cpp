#include "lasagne-entity.h"

CLasagneEntity::CLasagneEntity(
        const char* imagePath,
        const TVector<int, 2> screenSize
    ) :
    m_image(NULL),
    m_noofFramesX(0),
    m_noofFramesY(0),
    m_currentFrame(0)

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
        const TVector<int, 2> screenSize,
        const TVector<int, 2> noofFrames
    ) :
    m_image(NULL),
    m_noofFramesX(noofFrames.x()),
    m_noofFramesY(noofFrames.y()),
    m_currentFrame(0)
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
    if (m_noofFramesX + m_noofFramesY == 0)
    {
         // draw the entity in its 3d position
        SDL_Rect rcRect;
        rcRect.x = m_screenPosition.x();
        rcRect.y = m_screenPosition.y();
        SDL_BlitSurface(m_image, NULL, screen, &rcRect);
    }
    else
    {
        SDL_Rect rcRect;
        rcRect.x = m_screenPosition.x();
        rcRect.y = m_screenPosition.y();

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

        if (SDL_GetTicks() - m_lastFrameTime > 100)
        {
            m_lastFrameTime = SDL_GetTicks();
            m_currentFrame++;
            if (m_currentFrame >= (m_noofFramesX * m_noofFramesY))
                m_currentFrame = 0;
        }

        srcRect.x = xOffset * m_frameSize.x();
        srcRect.y = yOffset * m_frameSize.y();

        SDL_BlitSurface(m_image, &srcRect, screen, &rcRect);
    }
}
