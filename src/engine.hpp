#ifndef ENGINE_HPP
#define ENGINE_HPP

#include <SDL/SDL.h>
#include "frame_counter.hpp"
#include "cube_scene.hpp"

class Engine
{
	public:
		Engine(unsigned int width, unsigned int height, bool fullscreen = false);
		~Engine();

		void handleInput();
		void update();
		void draw();

		void quit() { _running = false; }
		bool isRunning() { return _running; }

	protected:
		FrameCounter _frame_counter;
		CubeScene _scene;
		bool _running;
		SDL_Surface* _surface;
};

#endif

