#ifndef LASAGNE_TEXT_HEADER
#define LASAGNE_TEXT_HEADER

#include "../SOGI/SOGI.h"

#include <SDL.h>
#include <SDL_ttf.h>

#include <string>
#include <sstream>

class CLasagneText {
private:

    TTF_Font        *m_font;                        //!<

    SDL_Surface     *m_surface;                     //!< The text surface to render too

    SDL_Rect        m_drawRect;                     //!< The bounds of the draw region

public:
                    //! Class constructor
                    CLasagneText();

                    //! Class destructor
                    ~CLasagneText();

                    //! Create the text object
    const bool      Create(
                        const std::string &text      //!< The string this text object represents
                    );

                    //! Set the text the object should display
    void            SetText(
                        const std::string &text      //!< The string this text object represents
                    );

                    //! Create the text object
    void            Render(
                        SDL_Surface *surface,       //!< The surface to blit our text surface too
                        const int &x,
                        const int &y
                    );

public:

    static TTF_Font *font;
};

#endif
