#ifndef LASAGNE_AUDIO_H_INCLUDED
#define LASAGNE_AUDIO_H_INCLUDED

#include "SDL.h"
#include "SDL_mixer.h"

struct AudioEngine {
    enum Enum {
        SDL,
        BASS
    };
};

class CLasagneAudioFile {

private:
    Mix_Chunk           *m_sdlAudio;            //!<
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
                        }
};

#endif // LASAGNE_AUDIO_H_INCLUDED
