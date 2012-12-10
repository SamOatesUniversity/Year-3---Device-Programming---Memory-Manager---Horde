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
    CLasagne *const engine = CLasagne::GetInstance();

    if (!engine->Create())
    {
        engine->Release();
        return 0;
    }

    CScene *currentScene = new CScene();
    currentScene->Load("media/graphics/level-1/");

    CPlayer *player = new CPlayer();
    player->Load("media/graphics/characters/player.png");

    // whilst the engine is running loop
	do
	{

        currentScene->Move((engine->GetMousePosition()->x() - 160.0f), (engine->GetMousePosition()->y() - 120.0f));

	} while (engine->Render());

    // free up resoures and allocated memory
    delete currentScene;
    delete player;
    engine->Release();

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
