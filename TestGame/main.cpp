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

    CLasagneMusicFile *genesis = lasagne->LoadMusicFile("music.ogg", MusicEngine::SDL);
    if (genesis == NULL)
    {
        lasagne->DisplayError("Failed to load the audio file!");
        LOGERROR("Failed to load the audio file!");
        return 0;
    }
    genesis->Play();

    static const int noofSamples = 0;
    CLasagneAudioFile *shaun[noofSamples];

    for (int shaunIndex = 0; shaunIndex < noofSamples; ++shaunIndex)
    {
        shaun[shaunIndex] = lasagne->LoadAudioFile("shaun.wav", AudioEngine::SDL);
        if (shaun[shaunIndex] == NULL)
        {
            lasagne->DisplayError("Failed to load the shaun audio file!");
            LOGERROR("Failed to load the shaun audio file!");
            return 0;
        }
    }

    lasagne->DisplayError("Before Draw Loop");

    int shaunIndex = 0;
    // whilst the engine is running loop
	do
	{

        if (rand() % 2000 == 0 && noofSamples) {
            if (shaunIndex >= noofSamples) shaunIndex = 0;
            shaun[shaunIndex]->Play();
        }
        shaunIndex++;


	} while (lasagne->Render());

    // free up resoures and allocated memory
    lasagne->Release();
    delete lasagne;

	return 0;
}
