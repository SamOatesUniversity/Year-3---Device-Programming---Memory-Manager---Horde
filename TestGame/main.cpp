#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "lasagne/lasagne.h"

int main (int argc, char *argv[])
{

    // Create our instance of the lasagne engine
    CLasagne *const lasagne = new CLasagne();

    // if we failed to create (setup sdl ect...) then exit
    if (!lasagne->Create())
        return 0;

    // whilst the engine is running loop
	do
	{

	} while (lasagne->Render());

    // free up resoures and allocated memory
    lasagne->Release();
    delete lasagne;

	return 0;
}
