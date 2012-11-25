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
    else if (m_engine == MusicEngine::BASS) {
        m_bassMusic = BASS_StreamCreateFile(FALSE, musicFile, 0, 0, 0);
        return true;
    }

    return NULL;
}
