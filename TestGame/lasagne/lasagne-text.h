#ifndef LASAGNE_TEXT_HEADER
#define LASAGNE_TEXT_HEADER

#include <SDL.h>
#include <SDL_ttf.h>

class CLasagneText {
private:

    TTF_Font        *m_font;

    SDL_Surface     *m_surface;                     //!< The text surface to render too

    SDL_Rect        m_drawRect;                     //!< The bounds of the draw region

public:
                    //!< Class constructor
                    CLasagneText();

                    //!< Class destructor
                    ~CLasagneText();

                    //!< Create the text object
    const bool      Create(
                        const char *text            //!< The string this text object represents
                    );

    void            Render(
                        SDL_Surface *surface        //!< The surface to blit our text surface too
                    );
};

#endif
