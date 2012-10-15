
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
    ASSERT(!m_screen, "Memory Leek! Call Release() before deleting this object.");
}

/*
 * \brief Create our lasagne instance, initialising sdl and other requirements
*/
const bool CLasagne::Create()
{
    // try to initialize sdl video, audio and joystick
    const int result = SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_JOYSTICK);
    if (result != 0)
    {
        // failed to initialize SDL
        std::stringstream errorMessage;
        errorMessage << "Failed to initialise SDL. Function SDL_Init() returned value '" << result << "'.";
        errorMessage << "\nSDL_GetError: " << SDL_GetError();
        SHOWLOGERROR(errorMessage.str().c_str());
        return false;
    }

    // try and create a screen surface
    m_screen = SDL_SetVideoMode(320, 240, 16, SDL_SWSURFACE);
    if (m_screen == NULL)
    {
        // failed to create the screen
        std::stringstream errorMessage;
        errorMessage << "Failed to create a screen surface. Function SDL_SetVideoMode() returned value 'NULL'.";
        errorMessage << "\nSDL_GetError: " << SDL_GetError();
        SHOWLOGERROR(errorMessage.str().c_str());
        return false;
    }

    // try and get the joystick object
    if (SDL_NumJoysticks() > 0)
    {
        m_joystick = SDL_JoystickOpen(0);
        if (m_joystick == NULL)
        {
            // failed to get the joystick object
            std::stringstream errorMessage;
            errorMessage << "Failed to open the joystick device. Function SDL_JoystickOpen(0) returned value 'NULL'.";
            errorMessage << "\nSDL_GetError: " << SDL_GetError();
            SHOWLOGERROR(errorMessage.str().c_str());
            return false;
        }
    }

    TTF_Init();

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

    for (int textIndex = 0; textIndex < m_textObjects.size(); ++textIndex)
    {
        //m_textObjects[textIndex]->Render(m_screen);
    }

    SDL_Flip(m_screen);

    return true;
}

/*
 * \brief Destroys all resources and SDL
*/
void CLasagne::Release()
{
    TTF_Quit();

    SafeFreeSurface(m_screen);
    SDL_Quit();
}

/*
 * \brief Shows an error message to the user
*/
void CLasagne::DisplayError(
        const char *errorMessage                    //!< The error message to display to screen
    )
{
    CLasagneText *text = new CLasagneText();
    text->Create(errorMessage);
    m_textObjects.push_back(text);
}
