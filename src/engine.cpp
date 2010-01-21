#include "engine.hpp"
#include "opengl.hpp"

Engine::Engine(unsigned int width, unsigned int height, bool fullscreen)
	: _running(true)
{
	unsigned int flags = SDL_OPENGL | SDL_GL_DOUBLEBUFFER;

	if(fullscreen)
		flags |= SDL_FULLSCREEN;

	_surface = SDL_SetVideoMode(width, height, 32, flags);
	if(_surface == 0)
		throw "Surface could not be created";

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
}

Engine::~Engine()
{
	std::cout << "~Engine()" << std::endl;
}

void Engine::handleInput()
{
	SDL_Event event;
	while(SDL_PollEvent(&event))
	{
		if(event.type == SDL_QUIT)
			quit();
		else
		{
			if(event.type == SDL_KEYDOWN)
			{
				switch(event.key.keysym.sym)
				{
					case SDLK_ESCAPE:
						quit();
						break;
					default:
						break;
				}
			}
		}
	}
}

void Engine::update()
{
}

void Engine::draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	_frame_counter.update();
	SDL_GL_SwapBuffers();
}