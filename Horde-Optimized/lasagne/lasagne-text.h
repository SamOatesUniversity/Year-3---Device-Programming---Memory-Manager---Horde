#ifndef LASAGNE_TEXT_HEADER
#define LASAGNE_TEXT_HEADER

#include "../SOGI/SOGI.h"

#include <SDL.h>
#include <SDL_gfxPrimitives.h>

#include <string>
#include <sstream>

class CLasagneText {
private:

	IVec2			m_position;						//!< 
	std::string		m_text;							//!< 
	bool			m_visible;						//!< 

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

					//!
	void			SetPosition(
						int x,						//!< 
						int y						//!< 
					);

                    //! Create the text object
    void            Render(
                        SDL_Surface *surface        //!< The surface to blit our text surface too
                    );

					//! 
	void			SetVisible( 
						bool visible 
					);
};

#endif
