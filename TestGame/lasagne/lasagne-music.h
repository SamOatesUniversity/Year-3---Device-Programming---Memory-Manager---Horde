#ifndef LASAGNE_MUSIC_H_INCLUDED
#define LASAGNE_MUSIC_H_INCLUDED

#include "SDL.h"
#include "SDL_mixer.h"

class CLasagneMusicFile {
private:

    Mix_Music           *m_music;                           //!< The SDL_Mixer representation of the music file

public:
                        //! Class constructor
                        CLasagneMusicFile();

                        //! Class destructor
                        ~CLasagneMusicFile();

                        //! Try to create the music file object
    const bool          Create(
                            const char *musicFile           //!<
                        );

                        //! Play the music file
    void                Play()
                        {
                            Mix_PlayMusic(m_music, 0);
                        }

};

#endif // LASAGNE-MUSIC_H_INCLUDED
