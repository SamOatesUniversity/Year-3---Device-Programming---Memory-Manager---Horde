#ifndef LASAGNE_HEADER
#define LASAGNE_HEADER

#include "../SOGI/SOGI.h"
#include "lasagne-text.h"

/* Header Includes */
#include <SDL.h>
#include <SDL_ttf.h>

#include <vector>

class CLasagne {
private:

    SDL_Surface                 *m_screen;              //!< A pointer to the sdl screen surface

    SDL_Joystick                *m_joystick;            //!< A pointer to the sdl joystick

    std::vector<CLasagneText*>  m_errorText;            //!<

public:
                                //! Class constructor
                                CLasagne();

                                //! Class destructor
                                ~CLasagne();

                                //! Create our lasagne instance, initialising sdl and other requirements
    const bool                  Create();

                                //! Renders the scene
    const bool                  Render();

                                //! Destroys all resources and SDL
    void                        Release();

                                //! Shows an error message to the user
    void                        DisplayError(
                                    const char *errorMessage                    //!< The error message to display to screen
                                );
};

/*!
 * \brief delete memory and null the pointer
 */
template<typename T>
void SafeFreeSurface(T*& ptr) {
	if (ptr)
	{
	    SDL_FreeSurface(ptr);
		ptr = NULL;
	}
}

#endif
