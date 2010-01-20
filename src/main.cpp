#include <iostream>
#include <SDL/SDL.h>
#include "opengl.hpp"
#include "engine.hpp"

#ifdef __APPLE__
int SDL_main(int argc, char *argv[])
#else
int main(int argc, char *argv[])
#endif
{
	Engine engine(1024, 768, false);

	try
	{
		while(engine.isRunning())
		{
			engine.handleInput();
			engine.update();
			engine.draw();
		}
	}
	catch(const char* msg)
	{
		std::cout << msg << "\n";
		return 1;
	}

	SDL_Quit();

	return 0;
}

