#ifndef LASAGNE_HEADER
#define LASAGNE_HEADER

#include "../SOGI/SOGI.h"
#include "lasagne-text.h"
#include "lasagne-entity.h"
#include "lasagne-music.h"
#include "lasagne-audio.h"

/* Header Includes */
#include <SDL.h>
#include <SDL_ttf.h>

#include <vector>

class CLasagne {
private:

    SDL_Surface                         *m_screen;              //!< A pointer to the sdl screen surface

    SDL_Joystick                        *m_joystick;            //!< A pointer to the sdl joystick

    TVector<int, 2>                     m_screenSize;           //!<
    TVector<int, 2>                     m_mousePosition;        //!<

    std::vector<CLasagneText*>          m_errorText;            //!<

    std::vector<CLasagneEntity*>        m_entity;               //!<
	std::vector<CLasagneText*>			m_textEntity;           //!<

#if defined(SHOW_FPS)
    struct {
        CLasagneText                    *text;                  //!<
        int                             fps;                    //!<
        int                             count;                  //!<
        int                             time;                   //!<
    } m_fps;
#endif

private:
                                        //! Class constructor
                                        CLasagne();

                                        //! Class constructor
                                        CLasagne(CLasagne const&);

                                        //! Assignment operator
	void					            operator=(CLasagne const&);

public:
                                        //! Class destructor
                                        ~CLasagne();

                                        //! Get the instance of the singlton
	static CLasagne			            *GetInstance()
                                        {
                                            static CLasagne *instance = new CLasagne();
                                            return instance;
                                        }

                                        //! Create our lasagne instance, initialising sdl and other requirements
    const bool                          Create();

                                        //! Renders the scene
    const bool                          Render();

                                        //! Destroys all resources and SDL
    void                                Release();

                                        //! Shows an error message to the user
    void                                DisplayError(
                                            const char *errorMessage                    //!< The error message to display to screen
                                        );

                                        //! Load a music file
    CLasagneMusicFile                   *LoadMusicFile(
                                            const char* musicFile,                      //!< The OGG music file to create the object around
                                            MusicEngine::Enum engine = MusicEngine::SDL //!<
                                        );

                                        //! Load a audio file
    CLasagneAudioFile                   *LoadAudioFile(
                                            const char* audioFile,                      //!<
                                            AudioEngine::Enum engine = AudioEngine::SDL //!<
                                        );

                                        //! Load an image
    CLasagneEntity                      *LoadImage(
                                             const char* imageFile                       //!<
                                        );

                                        //! Load an animated image
    CLasagneEntity                      *LoadAnimatedImage(
                                             const char* imageFile,                       //!<
                                             const TVector<int, 2> &noofFrames             //!<
                                        );

										//! 
	CLasagneText*						CreateText(
											char *text,
											int x,
											int y
										);

                                        //! Get the stored mouse position
    const TVector<int, 2>               *GetMousePosition() const
                                        {
                                            return &m_mousePosition;
                                        }
};

/*!
 * \brief delete memory and null the pointer
 */
template<typename T>
void SafeFreeSurface(T*& ptr) {
	if (ptr)
	{
	    SDL_FreeSurface(ptr);
		ptr = NULL;
	}
}

#endif
