#include "lasagne-music.h"

CLasagneMusicFile::CLasagneMusicFile(
        MusicEngine::Enum engine
    )
{
    m_sdlMusic = NULL;
    m_engine = engine;
}


CLasagneMusicFile::~CLasagneMusicFile()
{
	Mix_FreeMusic(m_sdlMusic);
    m_sdlMusic = NULL;
}

const bool CLasagneMusicFile::Create(
        const char *musicFile
    )
{
    if (m_engine == MusicEngine::SDL) {
        if (m_sdlMusic != NULL) {
            Mix_FreeMusic(m_sdlMusic);
            m_sdlMusic = NULL;
        }

        m_sdlMusic = Mix_LoadMUS(musicFile);
        return m_sdlMusic != NULL;
    }

    return NULL;
}
