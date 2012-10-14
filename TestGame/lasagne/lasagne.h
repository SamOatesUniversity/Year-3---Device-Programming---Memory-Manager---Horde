#ifndef LASAGNE_HEADER
#define LASAGNE_HEADER

/* Header Includes */
#include <SDL.h>
#include "entity/entity3D.h"
#include "../SOGI/SOGI.h"

class CLasagne {
private:

    SDL_Surface                 *m_screen;              //!< A pointer to the sdl screen surface

    SDL_Joystick                *m_joystick;            //!< A pointer to the sdl joystick

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
