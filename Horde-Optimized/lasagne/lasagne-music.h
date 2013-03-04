#ifndef LASAGNE_MUSIC_H_INCLUDED
#define LASAGNE_MUSIC_H_INCLUDED

#include "SDL.h"
#include "SDL_mixer.h"

struct MusicEngine {
    enum Enum {
        SDL,
        BASS,
    };
};

class CLasagneMusicFile {
private:

    Mix_Music           *m_sdlMusic;                        //!< The SDL_Mixer representation of the music file
    MusicEngine::Enum   m_engine;                           //!<

public:
                        //! Class constructor
                        CLasagneMusicFile(
                            MusicEngine::Enum engine = MusicEngine::SDL
                        );

                        //! Class destructor
                        ~CLasagneMusicFile();

                        //! Try to create the music file object
    const bool          Create(
                            const char *musicFile           //!<
                        );

                        //! Play the music file
    void                Play()
                        {
                            if (m_engine == MusicEngine::SDL) {
                                Mix_PlayMusic(m_sdlMusic, -1);
                            }
                        }

};

#endif // LASAGNE-MUSIC_H_INCLUDED
