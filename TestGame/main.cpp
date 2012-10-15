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

#if defined(_DEBUG)
    CSOGI::GetInstance().CreateConsoleWindow();
#endif

    //SHOWERROR("Theres a Potatoe in the freezer!");
    lasagne->DisplayError("Theres a Potatoe in the freezer!");

    // whilst the engine is running loop
	do
	{

	} while (lasagne->Render());

    // free up resoures and allocated memory
    lasagne->Release();
    delete lasagne;

	return 0;
}
