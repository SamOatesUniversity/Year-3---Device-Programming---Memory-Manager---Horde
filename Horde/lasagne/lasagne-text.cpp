#include "lasagne-text.h"

TTF_Font *CLasagneText::font = NULL;

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
    SDL_FreeSurface(m_surface);
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

    if (CLasagneText::font == NULL)
    {
        CLasagneText::font = TTF_OpenFont("fonts/arial.ttf", 12);
        if (CLasagneText::font == NULL)
        {
            std::stringstream errorMessage;
            errorMessage << "Failed to Create a font. Function TTF_OpenFont('ARIAL.TTF', 12) returned value 'NULL'.";
            errorMessage << "\nSDL_GetError: " << SDL_GetError();
            SHOWLOGERROR(errorMessage.str().c_str());
            return false;
        }
    }

    m_surface = TTF_RenderText_Shaded(
        CLasagneText::font,
        text.c_str(),
        foregroundColor,
        backgroundColor
    );

    return m_surface != NULL;
}

void CLasagneText::SetText(
        const std::string text      //!< The string this text object represents
    )
{
    SDL_Color foregroundColor = { 255, 255, 255 };
    SDL_Color backgroundColor = { 0, 0, 0 };

    SDL_FreeSurface(m_surface);

    m_surface = TTF_RenderText_Shaded(
        CLasagneText::font,
        text.c_str(),
        foregroundColor,
        backgroundColor
    );
}

void CLasagneText::Render(
        SDL_Surface *surface,        //!< The surface to blit our text surface too
        const int &x,
        const int &y
    )
{
    m_drawRect.x = x;
    m_drawRect.y = y;
    SDL_BlitSurface(m_surface, NULL, surface, &m_drawRect);
}
