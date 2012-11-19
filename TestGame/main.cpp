#include "lasagne/lasagne.h"

int main (int argc, char *argv[])
{

    // Create our instance of the lasagne engine
    CLasagne *const lasagne = new CLasagne();

    // if we failed to create (setup sdl ect...) then exit
    if (!lasagne->Create())
    {
        lasagne->Release();
        delete lasagne;
        return 0;
    }

    //for (int starIndex = 0; starIndex < 1000; ++starIndex)
    //    lasagne->AddEntity("star.bmp");

    CLasagneMusicFile *mario = lasagne->CreateMusicFile("music.ogg");
    if (mario == NULL)
    {
        SHOWLOGERROR("Failed to load the audio file!");
    }
    mario->Play();

    // whilst the engine is running loop
	do
	{

	} while (lasagne->Render());

    // free up resoures and allocated memory
    lasagne->Release();
    delete lasagne;

	return 0;
}
