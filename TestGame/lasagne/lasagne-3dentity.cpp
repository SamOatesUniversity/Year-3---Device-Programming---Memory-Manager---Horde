
#include "lasagne-3dentity.h"

float CLasagne3DEntity::EYEDISTANCE = 100.0f;

/*
* \brief Class constructor
*/
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

    m_image = new CLasagneSurface(SDL_LoadBMP("star.bmp"));
    if (m_image == NULL)
    {
        SHOWERROR("Failed to load image");
    }
}

/*
* \brief Class destructor
*/
CLasagne3DEntity::~CLasagne3DEntity()
{
    delete m_image;
}

/*
* \brief Render the surface, projected into 3D.
*/
void CLasagne3DEntity::Render(
        SDL_Surface *screen
    )
{
    const int sx = (int)(((EYEDISTANCE * (m_position.x() - m_screenPosition.x()))) / (EYEDISTANCE + m_position.z()) + m_screenPosition.x());
    const int sy = (int)(((EYEDISTANCE * (m_position.y() - m_screenPosition.y()))) / (EYEDISTANCE + m_position.z()) + m_screenPosition.y());

    m_position.SetZ(m_position.z() - 0.1f);
    if (m_position.z() < 0)
        m_position.SetZ(MAXDEPTH + (rand() % MAXDEPTH));

    m_image->Render(screen, sx, sy, 1.0f, 1.0f);
}
