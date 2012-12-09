#include "lasagne/lasagne.h"
#include "Horde/scenebase.h"
#include "Horde/player.h"

const bool isLeftPressed();
const bool isRightPressed();
const bool isUpPressed();
const bool isDownPressed();

int main (int argc, char *argv[])
{
    // if we failed to create (setup sdl ect...) then exit
    if (!CLasagne::GetInstance()->Create())
    {
        CLasagne::GetInstance()->Release();
        return 0;
    }

    CScene *currentScene = new CScene();
    currentScene->Load("media\\graphics\\level-1\\");

    CPlayer *player = new CPlayer();
    player->Load("media\\graphics\\characters\\player.png");

    static const float MOVE_SPEED = 0.1f;

    // whilst the engine is running loop
	do
	{
	    if (isLeftPressed())
	    {
            currentScene->Move(-MOVE_SPEED, 0.0f);
	    }
	    if (isRightPressed())
	    {
	        currentScene->Move(MOVE_SPEED, 0.0f);
	    }
	    if (isUpPressed())
	    {
	        currentScene->Move(0.0f, -MOVE_SPEED);
	    }
	    if (isDownPressed())
	    {
	        currentScene->Move(0.0f, MOVE_SPEED);
	    }

	} while (CLasagne::GetInstance()->Render());

    // free up resoures and allocated memory
    CLasagne::GetInstance()->Release();
    delete currentScene;
    delete player;

	return 0;
}

const bool isLeftPressed()
{
    Uint8 *keystate = SDL_GetKeyState(NULL);
    return keystate[SDLK_a] || keystate[SDLK_LEFT];
}

const bool isRightPressed()
{
    Uint8 *keystate = SDL_GetKeyState(NULL);
    return keystate[SDLK_d] || keystate[SDLK_RIGHT];
}

const bool isUpPressed()
{
    Uint8 *keystate = SDL_GetKeyState(NULL);
    return keystate[SDLK_w] || keystate[SDLK_UP];
}

const bool isDownPressed()
{
    Uint8 *keystate = SDL_GetKeyState(NULL);
    return keystate[SDLK_s] || keystate[SDLK_DOWN];
}
