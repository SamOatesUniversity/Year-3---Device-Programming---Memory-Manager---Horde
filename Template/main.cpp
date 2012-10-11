#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <SDL.h>

#define IMG_NAME    "001.bmp"

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

/* The screen surface, joystick device */
SDL_Surface *screen = NULL;
SDL_Joystick *joy = NULL;

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
	int done;
	SDL_Rect rcRect;
	SDL_Surface *bmp;

	/* Initialize SDL */
	if (SDL_Init (SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_JOYSTICK) < 0) {
		fprintf (stderr, "Couldn't initialize SDL: %s\n", SDL_GetError ());
		exit (1);
	}
	atexit (Terminate);

	SDL_ShowCursor(SDL_DISABLE);

	/* Set 320x240 16-bits video mode */
	screen = SDL_SetVideoMode (320, 240, 16, SDL_SWSURFACE);
	if (screen == NULL) {
		fprintf (stderr, "Couldn't set 320x240x16 video mode: %s\n", SDL_GetError ());
		exit (2);
	}

	/* Check and open joystick device */
	if (SDL_NumJoysticks() > 0) {
		joy = SDL_JoystickOpen(0);
		if(!joy) {
			fprintf (stderr, "Couldn't open joystick 0: %s\n", SDL_GetError ());
		}
	}

#ifdef GP2X
	/* Only use GP2X code here */
#endif

#ifdef WIN32
	/* Only use Windows code here */
#endif

    // image loading...
    bmp = SDL_LoadBMP(IMG_NAME);
	if (bmp == NULL) {
		fprintf (stderr, "Not found image. %s\n", SDL_GetError ());
		exit (3);
	}
	rcRect.x = 0;
	rcRect.y = 0;

	done = 0;
	while (!done)
	{
		SDL_Event event;

		/* Check for events */
		while (SDL_PollEvent (&event))
		{
			switch (event.type)
			{
				case SDL_KEYDOWN:
					/* if press Ctrl + C, terminate program */
					if ( (event.key.keysym.sym == SDLK_c) && (event.key.keysym.mod & (KMOD_LCTRL | KMOD_RCTRL)) )
						done = 1;
					break;
				case SDL_KEYUP:
					break;
				case SDL_JOYBUTTONDOWN:
					/* if press Start button, terminate program */
					if ( event.jbutton.button == VK_START )
						done = 1;
					break;
				case SDL_JOYBUTTONUP:
					break;
				case SDL_QUIT:
					done = 1;
					break;
				default:
					break;
			}
		}

		/* Processing */
		SDL_BlitSurface(bmp, NULL, screen, &rcRect);
		SDL_Flip(screen);
		SDL_Delay(100);
	}
	SDL_FreeSurface(bmp);
	SDL_FreeSurface(screen);
	SDL_Quit();

	return 0;
}
