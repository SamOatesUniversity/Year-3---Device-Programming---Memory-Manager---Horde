#ifndef LASAGNE_AUDIO_H_INCLUDED
#define LASAGNE_AUDIO_H_INCLUDED

#include "SDL.h"
#include "SDL_mixer.h"
#include "../BASS/bass.h"

struct AudioEngine {
    enum Enum {
        SDL,
        BASS
    };
};

class CLasagneAudioFile {

private:
    Mix_Chunk           *m_sdlAudio;            //!<
    HSTREAM             m_bassAudio;            //!<
    AudioEngine::Enum   m_engine;               //!<

public:
                        //! Class constructor
                        CLasagneAudioFile(
                            AudioEngine::Enum engine = AudioEngine::SDL
                        );

                        //! Class destructor
                        ~CLasagneAudioFile();

                        //! Try to create the music file object
    const bool          Create(
                            const char *audioFile           //!<
                        );

                        //! Play the music file
    void                Play()
                        {
                            if (m_engine == AudioEngine::SDL) {
                                Mix_PlayChannel(-1, m_sdlAudio, 0);
                            }
                            else if (m_engine == AudioEngine::BASS) {
                                BASS_ChannelPlay(m_bassAudio, FALSE);
                            }
                        }
};

#endif // LASAGNE_AUDIO_H_INCLUDED
