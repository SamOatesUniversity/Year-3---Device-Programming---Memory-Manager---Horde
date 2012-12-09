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
    const SDL_PixelFormat& fmt = *(m_surface->format);
    SDL_Surface *tempSurface = SDL_CreateRGBSurface(SDL_SWSURFACE, (int)(m_surface->w * scalex), (int)(m_surface->h * scaley), fmt.BitsPerPixel, fmt.Rmask, fmt.Gmask, fmt.Bmask, fmt.Amask);

    SDL_LockSurface(m_surface);
    SDL_LockSurface(tempSurface);

    unsigned int *imagePtr = (unsigned int *)m_surface->pixels;
    unsigned int *targetPtr = (unsigned int *)tempSurface->pixels;

    for (int y = 0; y < m_surface->h; ++y)
    {
        for (int x = 0; x < m_surface->w; ++x)
        {
            const int pixelOffset = ((y * m_surface->w) + x);
            //unsigned int pixel = imagePtr[pixelOffset];

            //const int targetPixel = ((y * tempSurface->w) + x);
            targetPtr[pixelOffset] = imagePtr[pixelOffset];
        }
    }

    SDL_UnlockSurface(tempSurface);
    SDL_UnlockSurface(m_surface);

    // draw the entity in its 3d position
    SDL_Rect rcRect;
    rcRect.x = targetx;
    rcRect.y = targety;
    SDL_BlitSurface(tempSurface, NULL, target, &rcRect);

    SDL_FreeSurface(tempSurface);
}
