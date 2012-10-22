
#include "lasagne-3dentity.h"

float CLasagne3DEntity::EYEDISTANCE = 100.0f;

CLasagne3DEntity::CLasagne3DEntity(
        const TVector<int, 2> screenSize
    ) : m_image(NULL)
{
    m_screenPosition.Set(
        screenSize.x() * 0.5f,
        screenSize.y() * 0.5f
    );

    m_position.Set(
        rand() % screenSize.x(),
        rand() % screenSize.y(),
        rand() % (MAXDEPTH * 2)
    );

    m_image = SDL_LoadBMP("star.bmp");
    if (m_image == NULL)
    {
        SHOWERROR("Failed to load image");
    }
}

CLasagne3DEntity::~CLasagne3DEntity()
{
    SDL_FreeSurface(m_image);
}

void CLasagne3DEntity::Render(
        SDL_Surface *screen
    )
{
    const int sx = ((EYEDISTANCE * (m_position.x() - m_screenPosition.x()))) / (EYEDISTANCE + m_position.z()) + m_screenPosition.x();
    const int sy = ((EYEDISTANCE * (m_position.y() - m_screenPosition.y()))) / (EYEDISTANCE + m_position.z()) + m_screenPosition.y();

    m_position.SetZ(m_position.z() - 0.5f);
    if (m_position.z() < 0)
        m_position.SetZ(MAXDEPTH + (rand() % MAXDEPTH));

    // draw the entity in its 3d position
    SDL_Rect rcRect;
    rcRect.x = sx;
    rcRect.y = sy;
    SDL_BlitSurface(m_image, NULL, screen, &rcRect);
}
