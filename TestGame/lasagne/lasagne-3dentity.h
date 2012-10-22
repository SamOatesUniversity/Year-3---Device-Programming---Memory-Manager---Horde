#ifndef LASAGNE_3DENTITY_HEADER
#define LASAGNE_3DENTITY_HEADER

#include "../SOGI/SOGI.h"
#include <SDL.h>

#define MAXDEPTH            2000

class CLasagne3DEntity {
private:

    SDL_Surface             *m_image;                       //!<

    TVector<float, 3>       m_position;                     //!<

    TVector<float, 2>       m_screenPosition;               //!<

    static float            EYEDISTANCE;                    //!<

public:
                            //!
                            CLasagne3DEntity(
                                const TVector<int, 2> screenSize
                            );

                            //!
                            ~CLasagne3DEntity();

                            //!
    void                    Render(
                                SDL_Surface *screen
                            );
};

#endif
