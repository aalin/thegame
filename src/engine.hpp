#ifndef ENGINE_HPP
#define ENGINE_HPP

#include "frame_counter.hpp"

class Engine
{
	public:
		Engine(unsigned int width, unsigned int height, bool fullscreen = false)
		{ }

		void handleInput() { }
		void update() { }
		void draw()
		{
			_frame_counter.update();
		}

		void quit() { }

		bool isRunning() { return true; }

	protected:
		FrameCounter _frame_counter;
};

#endif

