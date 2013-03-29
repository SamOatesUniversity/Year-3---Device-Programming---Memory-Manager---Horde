#ifndef LASAGNE_HEADER
#define LASAGNE_HEADER

/************************************************************************/
/* HEADER FILES                                                         */
/************************************************************************/

#include <ProFy.h>

#include "../SOGI/SOGI.h"
#include "lasagne-text.h"
#include "lasagne-entity.h"
#include "lasagne-music.h"
#include "lasagne-audio.h"
#include "lasagne-debug-stats.h"

#include <SDL.h>
#include <SDL_ttf.h>

#include <vector>

/************************************************************************/
/* DEFINES		                                                        */
/************************************************************************/

// Make sure show FPS is on
#define SHOW_DEBUG_STATS

#define DEPTH_LEVELS	10

/************************************************************************/
/* CLASS DESCRIPTION													*/
/************************************************************************/

//! Main engine factory class
class CLasagne {
private:

    SDL_Surface                         *m_screen;								//!< A pointer to the sdl screen surface

    SDL_Joystick                        *m_joystick;							//!< A pointer to the sdl joystick

    TVector<int, 2>                     m_screenSize;							//!< The size of the screen
    TVector<int, 2>                     m_mousePosition;						//!< The current mouse/cursor position

    std::vector<CLasagneText*>          m_errorText;							//!< A list of on screen error messages (for debugging)

    std::vector<CLasagneEntity*>        m_entity[DEPTH_LEVELS];					//!< The list of entities loaded in the engine
	std::vector<CLasagneText*>			m_textEntity;							//!< The list of text entities loaded in the engine
	std::vector<CLasagneMusicFile*>		m_music;								//!< The list of music loaded in the engine
	std::vector<CLasagneAudioFile*>		m_audio;								//!< The list of audio samples loaded in the engine
	
	bool								m_isPaused;								//!< Is the renderer paused
	bool								m_isRunning;							//!< Is the renderer running
	unsigned char						m_audioVolume;							//!< The volume of the audio

#if defined(SHOW_DEBUG_STATS)
    struct {
        CLasagneText                    *text;									//!< The text used to display the fps on screen
        int                             fps;									//!< The current FPS of the application
        int                             count;									//!< The number of frames since the last second
        int                             time;									//!< The time of the last FPS querry
    } m_fps;

	bool								m_showTimerGraphs;
	CLasagneDebugStats					*m_debugStats;

#endif


	struct Timer {
		enum Enum {
			Render,
			Noof
		};
	};

	ProFy::TimerID						m_timer[Timer::Noof];

	std::string							m_currentMethod;
	bool								m_methodLock;

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

                                        //! Get the instance of the singleton
	static CLasagne			            *GetInstance()
                                        {
                                            static CLasagne *instance = new CLasagne();
                                            return instance;
                                        }

                                        //! Create our lasagne instance, initializing sdl and other requirements
    const bool                          Create();

                                        //! Renders the scene
    const bool                          Render();

                                        //! Destroys all resources and SDL
    void                                Release();

                                        //! Shows an error message to the user
    void                                DisplayError(
                                            const char *errorMessage						//!< The error message to display to screen
                                        );

                                        //! Load a music file
    CLasagneMusicFile                   *LoadMusicFile(
                                            const char* musicFile,							//!< The OGG music file to create the object around
                                            MusicEngine::Enum engine = MusicEngine::SDL		//!< Which music engine to use
                                        );

                                        //! Load a audio file
    CLasagneAudioFile                   *LoadAudioFile(
                                            const char* audioFile,							//!< The Wav file of the audio sample to load
                                            AudioEngine::Enum engine = AudioEngine::SDL		//!< Which audio engine to use
                                        );

                                        //! Load an image
    CLasagneEntity                      *LoadImage(
                                             const char* imageFile,							//!< A path to an image to load
											 unsigned int depth								//!< 
                                        );

                                        //! Load an animated image
    CLasagneEntity                      *LoadAnimatedImage(
                                             const char* imageFile,							//!< A path to an image to load
                                             const TVector<int, 2> &noofFrames,				//!< The frame layout of the animated image (e.g. 4x2)
											 unsigned int depth
                                        );

										//! Create a text entity to be drawn to the screen
	CLasagneText*						CreateText(
											char *text,										//!< Default text to display
											int x,											//!< The x coord of the text
											int y											//!< The y coord of the text
										);

										//! Remove an entity form the engine and safely destroy it
	void								Destroy(
											CLasagneEntity **entity							//!< A pointer to the entity pointer to be destroyed
										);

										//! Remove a text entity form the engine and safely destroy it
	void								Destroy(
											CLasagneText **text								//!< A pointer to the text entity pointer to be destroyed
										);

										//! Remove an audio sample form the engine and safely destroy it
	void								Destroy(
											CLasagneAudioFile **audio						//!< A pointer to the audio sample pointer to be destroyed
										);	

                                        //! Get the stored mouse position
    const TVector<int, 2>               *GetMousePosition() const
                                        {
                                            return &m_mousePosition;
                                        }

	void								SetEntityDepth(
											CLasagneEntity *entity,
											unsigned int oldDepth,
											unsigned int newDepth
										);

	#if defined(SHOW_DEBUG_STATS)

	void								ShowTimers(
											const bool show
										)
										{
											m_showTimerGraphs = show;
										}

	#endif

	const bool							IsPaused() const
										{
											return m_isPaused;
										}

	const bool							IsRunning() const
										{
											return m_isRunning;
										}

	const std::string					GetCurrentMethod() const
										{
											if (m_methodLock) {
												return "";
											}
											return m_currentMethod;
										}

	void								SetCurrentMethod(const std::string methodName)
										{
											m_methodLock = true;
											m_currentMethod = methodName;
											m_methodLock = false;
										}
};

/************************************************************************/
/* INLINE METHODS														*/
/************************************************************************/

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
