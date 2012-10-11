
#include "lasagne.h"

/*
 * \brief Class constructor
*/
CLasagne::CLasagne() :
    m_screen(NULL),
    m_joystick(NULL)
{

}

/*
 * \brief Class destructor
*/
CLasagne::~CLasagne()
{

}

/*
 * \brief Create our lasagne instance, initialising sdl and other requirements
*/
const bool CLasagne::Create()
{
    // try to initialize sdl video, audio and joystick
    if (SDL_Init (SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_JOYSTICK) < 0)
    {
        // failed to initialize SDL

        return false;
    }

    // try and create a screen surface
    m_screen = SDL_SetVideoMode (320, 240, 16, SDL_SWSURFACE);
    if (m_screen == NULL)
    {
        // failed to create the screen
        return false;
    }

    // try and get the joystick object
    if (SDL_NumJoysticks() > 0)
    {
        m_joystick = SDL_JoystickOpen(0);
        if (m_joystick == NULL)
        {
            // failed to get the joystick object
            return false;
        }
    }

    return true;
}

/*
 * \brief Renders the scene
*/
const bool CLasagne::Render()
{
    SDL_Event event;
    while (SDL_PollEvent (&event))
    {
        switch (event.type)
        {
            case SDL_QUIT:
            {
                return false;
            }
            break;
        }
    }

    return true;
}

/*
 * \brief Destroys all resources and SDL
*/
void CLasagne::Release()
{
    SDL_FreeSurface(m_screen);
    SDL_Quit();
}
