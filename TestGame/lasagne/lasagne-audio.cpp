#include "lasagne-audio.h"

CLasagneAudioFile::CLasagneAudioFile(
        AudioEngine::Enum engine
    )
{
    m_sdlAudio = NULL;
    m_engine = engine;
}

CLasagneAudioFile::~CLasagneAudioFile()
{
    Mix_FreeChunk(m_sdlAudio);
    m_sdlAudio = NULL;
}

//! Try to create the music file object
const bool CLasagneAudioFile::Create(
        const char *audioFile           //!<
    )
{
    if (m_engine == AudioEngine::SDL) {
        if (m_sdlAudio != NULL) {
            Mix_FreeChunk(m_sdlAudio);
            m_sdlAudio = NULL;
        }

        m_sdlAudio = Mix_LoadWAV(audioFile);
        return m_sdlAudio != NULL;
    }
    else if (m_engine == AudioEngine::BASS) {
        m_bassAudio = BASS_StreamCreateFile(FALSE, audioFile, 0, 0, 0);
        return true;
    }
    return NULL;
}
