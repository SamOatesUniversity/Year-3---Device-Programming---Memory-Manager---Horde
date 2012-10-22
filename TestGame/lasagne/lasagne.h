#ifndef LASAGNE_HEADER
#define LASAGNE_HEADER

#include "../SOGI/SOGI.h"
#include "lasagne-text.h"
#include "lasagne-3dentity.h"

/* Header Includes */
#include <SDL.h>
#include <SDL_ttf.h>

#include <gl/gl.h>
#include <gl/glu.h>

#include <vector>

class CLasagne {
private:

    SDL_Surface                         *m_screen;              //!< A pointer to the sdl screen surface

    SDL_Joystick                        *m_joystick;            //!< A pointer to the sdl joystick

    TVector<int, 2>                     m_screenSize;           //!<

    std::vector<CLasagneText*>          m_errorText;            //!<

    std::vector<CLasagne3DEntity*>      m_3DEntity;             //!<

public:
                                        //! Class constructor
                                        CLasagne();

                                        //! Class destructor
                                        ~CLasagne();

                                        //! Create our lasagne instance, initialising sdl and other requirements
    const bool                          Create();

                                        //! Renders the scene
    const bool                          Render();

                                        //! Destroys all resources and SDL
    void                                Release();

                                        //! Shows an error message to the user
    void                                DisplayError(
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
