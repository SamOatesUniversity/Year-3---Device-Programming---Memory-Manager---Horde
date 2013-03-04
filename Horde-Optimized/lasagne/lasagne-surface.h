#ifndef LASAGNE_SURFACE_HEADER
#define LASAGNE_SURFACE_HEADER

#include "../SOGI/SOGI.h"
#include <SDL.h>

class CLasagneSurface {
private:

    SDL_Surface             *m_surface;                     //!< Our RGB surface

public:
                            //! Class constructor
                            CLasagneSurface(
								SDL_Surface *sourceSurface
							);

                            //! Class destructor
                            ~CLasagneSurface();

                            //! Draw our surface
    void                    Render(
                                SDL_Surface *target,
                                const int x,
                                const int y,
                                const float scalex,
                                const float scaley
                            );
};

#endif
