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
        const std::string &text
    )
{
	m_text = text;
	return true;
}

void CLasagneText::SetText(
        const std::string &text      //!< The string this text object represents
    )
{
	m_text = text;
}

void CLasagneText::Render(
        SDL_Surface *surface         //!< The surface to blit our text surface too
    )
{
	stringColor(surface, m_position.x(), m_position.y(), m_text.c_str(), 0xFFFFFFFF);
}

void CLasagneText::SetPosition( 
		int x,						//!< 
		int y						//!< 
	)
{
	m_position.Set(x, y);
}
