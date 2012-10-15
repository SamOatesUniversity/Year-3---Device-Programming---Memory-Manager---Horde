#include "lasagne-text.h"

/*
*   \brief Class constructor
*/
CLasagneText::CLasagneText()
{

}

/*
*   \brief Class destructor
*/
CLasagneText::~CLasagneText()
{

}

/*
*   \brief Create the text object
*/
const bool CLasagneText::Create(
        const std::string text
    )
{
    SDL_Color foregroundColor = { 255, 255, 255 };
    SDL_Color backgroundColor = { 0, 0, 0 };
    m_font = TTF_OpenFont("arial.ttf", 12);

    if (m_font == NULL)
    {
        std::stringstream errorMessage;
        errorMessage << "Failed to Create a font. Function TTF_OpenFont('ARIAL.TTF', 12) returned value 'NULL'.";
        errorMessage << "\nSDL_GetError: " << SDL_GetError();
        SHOWLOGERROR(errorMessage.str().c_str());
        return false;
    }

    m_surface = TTF_RenderText_Shaded(
        m_font,
        text.c_str(),
        foregroundColor,
        backgroundColor
    );
}

void CLasagneText::Render(
        SDL_Surface *surface        //!< The surface to blit our text surface too
    )
{
    SDL_BlitSurface(m_surface, NULL, surface, &m_drawRect);
}
