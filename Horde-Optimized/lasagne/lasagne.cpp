
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

#if defined(SHOW_FPS)
    m_fps.text = new CLasagneText();
    m_fps.text->Create("FPS: 0");
    m_fps.fps = 0;
    m_fps.count = 0;
    m_fps.time = SDL_GetTicks();
	m_fps.text->SetPosition(4, 222);
#endif

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

#ifdef WIN32
            case SDL_KEYDOWN:
            {
                if (event.key.keysym.sym == SDLK_ESCAPE)
                {
                    return false;
                }
            }
            break;
#else
            case SDL_JOYBUTTONUP:
            {
                if (event.jbutton.button == 6)
                {
                    return false;
                }
            }
            break;
#endif
            case SDL_MOUSEMOTION:
            {
                m_mousePosition.Set(event.motion.x, event.motion.y);
            }
            break;
        }
    }


	const std::vector<CLasagneEntity*>::iterator end = m_entity.end();
	for (int depth = 0; depth < 10; ++depth)
	{
		std::vector<CLasagneEntity*>::iterator entity = m_entity.begin();
		for (entity = entity; entity != end; ++entity)
		{
			CLasagneEntity *const thisEntity = (*entity);
			if (thisEntity->GetDepth() == depth)
				thisEntity->Render(m_screen);
		}
	}

	for (unsigned int textIndex = 0; textIndex < m_textEntity.size(); ++textIndex)
	{
		CLasagneText *const text = m_textEntity[textIndex];
		text->Render(m_screen);
	}

#if defined(SHOW_FPS)
    // show error messages
    for (unsigned int textIndex = 0; textIndex < m_errorText.size(); ++textIndex)
    {
		m_errorText[textIndex]->SetPosition(2, 2 + (textIndex * 14));
        m_errorText[textIndex]->Render(m_screen);
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

    m_fps.text->Render(m_screen);
#endif

    SDL_Flip(m_screen);

    return true;
}

/*
 * \brief Destroys all resources and SDL
*/
void CLasagne::Release()
{
	for (unsigned int entityIndex = 0; entityIndex < m_entity.size(); ++entityIndex)
	{
		SafeDelete(m_entity[entityIndex]);
	}
	m_entity.clear();

	for (unsigned int musicIndex = 0; musicIndex < m_music.size(); ++musicIndex)
	{
		SafeDelete(m_music[musicIndex]);
	}
	m_music.clear();

	for (unsigned int audioIndex = 0; audioIndex < m_audio.size(); ++audioIndex)
	{
		SafeDelete(m_audio[audioIndex]);
	}
	m_audio.clear();

#if defined(SHOW_FPS)
	SafeDelete(m_fps.text);
#endif

    TTF_Quit();
    SafeFreeSurface(m_screen);
    SDL_Quit();

    delete this;
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

/*
 * \brief Load a music file
*/
CLasagneMusicFile *CLasagne::LoadMusicFile(
         const char* musicFile,						//!< The OGG music file to create the object around
         MusicEngine::Enum engine					//!< Which music engine to use
    )
{
    CLasagneMusicFile *music = new CLasagneMusicFile(engine);
    if (!music->Create(musicFile)) {
		SafeDelete(music);
        return NULL;
    }

	m_music.push_back(music);

    return music;
}

/*
 * \brief Load a audio file
*/
CLasagneAudioFile *CLasagne::LoadAudioFile(
         const char* audioFile,						//!< The Wav file of the audio sample to load
         AudioEngine::Enum engine					//!< Which audio engine to use
    )
{
    CLasagneAudioFile *audio = new CLasagneAudioFile(engine);
    if (!audio->Create(audioFile)) {
		SafeDelete(audio);
        return NULL;
    }

	m_audio.push_back(audio);

    return audio;
}

/*
 * \brief Load an image
*/
CLasagneEntity *CLasagne::LoadImage(
        const char* imageFile                         //!< A path to an image to load
    )
{
    CLasagneEntity *entity = new CLasagneEntity(imageFile, m_screenSize);
	if (entity->GetImage() == NULL)
	{
		SafeDelete(entity);
		return NULL;
	}

    m_entity.push_back(entity);
    return entity;
}

/*
 * \brief Load an animated image
*/
CLasagneEntity *CLasagne::LoadAnimatedImage(
        const char* imageFile,                        //!< A path to an image to load
        const TVector<int, 2> &noofFrames             //!< The frame layout of the animated image (e.g. 4x2)
    )
{
    CLasagneEntity *entity = new CLasagneEntity(imageFile, m_screenSize, noofFrames);
    m_entity.push_back(entity);
    return entity;
}

/*
 * \brief Create a text entity to be drawn to the screen
*/
CLasagneText* CLasagne::CreateText(
		char *text,									//!< Default text to display
		int x,										//!< The x coord of the text
		int y										//!< The y coord of the text
	)
{
	CLasagneText *const newText = new CLasagneText();
	newText->Create(text);
	newText->SetPosition(x, y);
	m_textEntity.push_back(newText);
	return newText;
}

/*
 * \brief Remove an entity form the engine and safely destroy it
*/
void CLasagne::Destroy( 
		CLasagneEntity **entity						//!< A pointer to the entity pointer to be destroyed
	)
{
	std::vector<CLasagneEntity*>::iterator iter = m_entity.begin();
	std::vector<CLasagneEntity*>::iterator endIter = m_entity.end();

	for (iter = iter; iter != endIter; iter++)
	{
		if ((*iter) == (*entity))
		{
			m_entity.erase(iter);
			break;
		}
	}

	SafeDelete(*entity);
}

/*
 * \brief Remove a text entity form the engine and safely destroy it
*/
void CLasagne::Destroy( 
		CLasagneText **text								//!< A pointer to the text entity pointer to be destroyed
	)
{
	std::vector<CLasagneText*>::iterator iter = m_textEntity.begin();
	std::vector<CLasagneText*>::iterator endIter = m_textEntity.end();

	for (iter = iter; iter != endIter; iter++)
	{
		if ((*iter) == (*text))
		{
			m_textEntity.erase(iter);
			break;
		}
	}

	SafeDelete(*text);
}

/*
 * \brief Remove an audio sample form the engine and safely destroy it
*/
void CLasagne::Destroy(
		CLasagneAudioFile **audio						//!< A pointer to the audio sample pointer to be destroyed
	)
{
	std::vector<CLasagneAudioFile*>::iterator iter = m_audio.begin();
	std::vector<CLasagneAudioFile*>::iterator endIter = m_audio.end();

	for (iter = iter; iter != endIter; iter++)
	{
		if ((*iter) == (*audio))
		{
			m_audio.erase(iter);
			break;
		}
	}

	SafeDelete(*audio);
}
