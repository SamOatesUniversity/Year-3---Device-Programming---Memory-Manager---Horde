// See top of Tests.cpp for version changes

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <unistd.h>
#include <SDL.h>

#include <sstream>
#include <iostream>
#include <fstream>

#include "TextOutput.h"
#include "Tests.h"

template <class T>
std::string ToString(const T& t)
{
    std::ostringstream sstr;

    sstr << t;

    return sstr.str();
}

/* GP2X button mapping */
enum MAP_KEY
{
	VK_UP         , // 0
	VK_UP_LEFT    , // 1
	VK_LEFT       , // 2
	VK_DOWN_LEFT  , // 3
	VK_DOWN       , // 4
	VK_DOWN_RIGHT , // 5
	VK_RIGHT      , // 6
	VK_UP_RIGHT   , // 7
	VK_START      , // 8
	VK_SELECT     , // 9
	VK_FL         , // 10
	VK_FR         , // 11
	VK_FA         , // 12
	VK_FB         , // 13
	VK_FX         , // 14
	VK_FY         , // 15
	VK_VOL_UP     , // 16
	VK_VOL_DOWN   , // 17
	VK_TAT          // 18
};

void Terminate(void)
{
	SDL_Quit();
#ifdef GP2X
	chdir("/usr/gp2x");
	execl("/usr/gp2x/gp2xmenu", "/usr/gp2x/gp2xmenu", NULL);
#endif
}

int main (int argc, char *argv[])
{
	/* Initialize SDL */
	if (SDL_Init (SDL_INIT_VIDEO | SDL_INIT_JOYSTICK) < 0) {
		fprintf (stderr, "Couldn't initialize SDL: %s\n", SDL_GetError ());
		exit (1);
	}
	atexit (Terminate);

    // Text output for harness and MM
    CTextOutput *textOutputHarness=new CTextOutput();
    if (!textOutputHarness->Initialise(11,0,"Memory Manager Test Harness V0.7"))
        exit(1);

    CTextOutput *textOutputMM=new CTextOutput();
    if (!textOutputMM->Initialise(11,12,"MM Output"))
        exit(1);

    textOutputMM->SetStatusText("Not instantiated");

	SDL_ShowCursor(SDL_DISABLE);

	/* Set 320x240 16-bits video mode */
	SDL_Surface *screen = SDL_SetVideoMode (320, 240, 16, SDL_SWSURFACE);
	if (screen == NULL) {
		fprintf (stderr, "Couldn't set 320x240x16 video mode: %s\n", SDL_GetError ());
		exit (2);
	}

	/* Check and open joystick device */
    SDL_Joystick *joy = NULL;
	if (SDL_NumJoysticks() > 0) {
		joy = SDL_JoystickOpen(0);
		if(!joy) {
			fprintf (stderr, "Couldn't open joystick 0: %s\n", SDL_GetError ());
		}
	}

#ifdef GP2X
	/* Only use GP2X code here */
	textOutputHarness->SetStatusText("Ready. Press Start Button to begin. Select to close.");
#endif

#ifdef WIN32
	/* Only use Windows code here */
	textOutputHarness->SetStatusText("Ready. Press Return Key to begin.");
#endif

    CTests *tests=new CTests(textOutputHarness,textOutputMM);

    bool done = false;
	while (!done)
	{
        /* Check for events */
		SDL_Event event;
		while (SDL_PollEvent (&event))
		{
			switch (event.type)
			{
				case SDL_KEYDOWN:
					/* if press Ctrl + C, terminate program */
					if ( (event.key.keysym.sym == SDLK_c) && (event.key.keysym.mod & (KMOD_LCTRL | KMOD_RCTRL)) )
						done = 1;
                    else if ((event.key.keysym.sym==SDLK_RETURN))
                        tests->Start();
					break;
				case SDL_KEYUP:
					break;
				case SDL_JOYBUTTONDOWN:
                    if ( event.jbutton.button == VK_SELECT )
                        done=1;
					/* if press Start button, start program */
					else if ( event.jbutton.button == VK_START )
						tests->Start();
					break;
				case SDL_JOYBUTTONUP:
					break;
				case SDL_QUIT:
					done = 1;
                    textOutputHarness->SetStatusText("Quit");
					break;
				default:
					break;
			}
		}

        // Clear screen
        SDL_FillRect( SDL_GetVideoSurface(), NULL, SDL_MapRGB(SDL_GetVideoSurface()->format,220,255,220));

        // Update tests
        tests->Update();

        // Render text
        textOutputHarness->Render(screen);
        textOutputMM->Render(screen);

		SDL_Flip(screen);
	}

    // Clean up
    delete tests;
    delete textOutputHarness;
    delete textOutputMM;
	SDL_FreeSurface(screen);
	return 0;
}
