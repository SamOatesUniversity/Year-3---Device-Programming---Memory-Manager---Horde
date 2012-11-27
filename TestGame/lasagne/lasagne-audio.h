#ifndef LASAGNE_AUDIO_H_INCLUDED
#define LASAGNE_AUDIO_H_INCLUDED

#include "SDL.h"
#include "SDL_mixer.h"

#ifndef GP2X
    #include "../BASS/bass.h"
#endif

struct AudioEngine {
    enum Enum {
        SDL,
        BASS
    };
};

class CLasagneAudioFile {

private:
    Mix_Chunk           *m_sdlAudio;            //!<
    #ifndef GP2X
    HSTREAM             m_bassAudio;            //!<
    #endif
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
                            #ifndef GP2X
                            else if (m_engine == AudioEngine::BASS) {
                                BASS_ChannelPlay(m_bassAudio, FALSE);
                            }
                            #endif
                        }
};

#endif // LASAGNE_AUDIO_H_INCLUDED
