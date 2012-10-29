#include "lasagne-surface.h"

CLasagneSurface::CLasagneSurface(
        SDL_Surface *sourceSurface
    ) : m_surface(NULL)
{
    m_surface = sourceSurface;
}

CLasagneSurface::~CLasagneSurface()
{
    SDL_FreeSurface(m_surface);
}

void CLasagneSurface::Render(
        SDL_Surface *target,
        const int targetx,
        const int targety,
        const float scalex,
        const float scaley
    )
{
    const SDL_PixelFormat *const format = m_surface->format;

    const int bpp = format->BitsPerPixel;

    SDL_LockSurface(m_surface);

    const Uint16 start = *((Uint16*)m_surface->pixels);

    for (int y = 0; y < m_surface->h; ++y)
    {
        for (int x = 0; x < m_surface->w; ++x)
        {
            const Uint16 pixel = start + ((y * m_surface->w) + x);
            const SDL_Color *const pixelColor = &(format->palette->colors[pixel]);

            std::stringstream sstr;
            sstr << "color (" << x << ", " << y << ") -> " << pixelColor->r << ", " << pixelColor->g << ", " << pixelColor->b;
            LOGERROR(sstr.str().c_str());

        }
    }

    SDL_UnlockSurface(m_surface);


    // draw the entity in its 3d position
    //SDL_Rect rcRect;
    //rcRect.x = targetx;
    //rcRect.y = targety;
    //SDL_BlitSurface(m_surface, NULL, target, &rcRect);
}
