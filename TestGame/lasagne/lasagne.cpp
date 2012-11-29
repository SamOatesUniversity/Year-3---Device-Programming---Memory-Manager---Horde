
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
    m_screenSize.Set(320, 240);
    m_screen = SDL_SetVideoMode(m_screenSize.x(), m_screenSize.y(), 16, SDL_SWSURFACE);
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

    int audio_rate = 22050;
    Uint16 audio_format = AUDIO_S16; /* 16-bit stereo */
    int audio_channels = 2;
    int audio_buffers = 4096;

    #ifdef GP2X
        audio_rate = MIX_DEFAULT_FREQUENCY;
        audio_format = AUDIO_S16;
        audio_channels = MIX_DEFAULT_CHANNELS;
        audio_buffers = 256;
    #endif


    if (Mix_OpenAudio(audio_rate, audio_format, audio_channels, audio_buffers)) {
        // failed to open the audio from SDL mixer
        std::stringstream errorMessage;
        errorMessage << "Failed to open the audio device. Function Mix_OpenAudio(audio_rate, audio_format, audio_channels, audio_buffers) returned value '0'.";
        errorMessage << "\nSDL_GetError: " << SDL_GetError();
        SHOWLOGERROR(errorMessage.str().c_str());
        return false;
    }

#ifndef GP2X
    BASS_Init(-1, 44100, 0, NULL, 0);
#endif

#if defined(_DEBUG)
    CSOGI::GetInstance().CreateConsoleWindow();

    m_fps.text = new CLasagneText();
    m_fps.text->Create("FPS: 0");
    m_fps.fps = 0;
    m_fps.count = 0;
    m_fps.time = SDL_GetTicks();
#endif

    return true;
}

/*
 * \brief Renders the scene
*/
const bool CLasagne::Render()
{
    SDL_FillRect(m_screen, NULL, 0);

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

    for (unsigned int starIndex = 0; starIndex < m_3DEntity.size(); ++starIndex)
    {
        m_3DEntity[starIndex]->Render(m_screen);
    }

#if defined(_DEBUG)
    // show error messages
    for (unsigned int textIndex = 0; textIndex < m_errorText.size(); ++textIndex)
    {
        m_errorText[textIndex]->Render(m_screen, 2, 2 + (textIndex * 14));
    }

    m_fps.count++;
    int ticks = SDL_GetTicks();
    if (ticks - m_fps.time >= 1000)
    {
        m_fps.fps = m_fps.count;
        m_fps.time = ticks;
        m_fps.count = 0;

        std::stringstream buf;
        buf << "FPS :" << m_fps.fps;

        m_fps.text->SetText(buf.str());
    }

    m_fps.text->Render(m_screen, 2, m_screenSize.y() - 16);
#endif

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
#if defined(_DEBUG)
    CLasagneText *text = new CLasagneText();
    text->Create(errorMessage);
    m_errorText.push_back(text);

    if (m_errorText.size() > 10)
    {
        std::vector<CLasagneText*>::iterator it = m_errorText.begin();
        SafeDelete(*it);
        m_errorText.erase(it);
    }
#endif
}

CLasagneMusicFile *CLasagne::LoadMusicFile(
         const char* musicFile,
         MusicEngine::Enum engine
    )
{
    CLasagneMusicFile *music = new CLasagneMusicFile(engine);
    if (!music->Create(musicFile)) {
        return NULL;
    }

    return music;
}

CLasagneAudioFile *CLasagne::LoadAudioFile(
         const char* audioFile,
         AudioEngine::Enum engine
    )
{
    CLasagneAudioFile *audio = new CLasagneAudioFile(engine);
    if (!audio->Create(audioFile)) {
        return NULL;
    }

    return audio;
}

