#include "lasagne-music.h"

CLasagneMusicFile::CLasagneMusicFile() :
    m_music(NULL)
{

}


CLasagneMusicFile::~CLasagneMusicFile()
{

}

const bool CLasagneMusicFile::Create(
        const char *musicFile
    )
{
    if (m_music != NULL) {
        Mix_FreeMusic(m_music);
        m_music = NULL;
    }

    m_music = Mix_LoadMUS("music.ogg");

    return m_music != NULL;
}
