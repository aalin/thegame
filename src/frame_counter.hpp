#ifndef FRAME_COUNTER_HPP
#define FRAME_COUNTER_HPP

#include <SDL/SDL.h>

class FrameCounter
{
	public:
		FrameCounter()
		{
			resetCounts();
		}

		void update()
		{
			_frames++;
			int end_time = SDL_GetTicks();
			if(end_time - _start_time >= 5000)
			{
				float seconds = (end_time - _start_time) / 1000.0;
				float fps = _frames / seconds;
				std::cout << _frames << " frames in " << seconds << " seconds: " << fps << " fps" << std::endl;
				resetCounts();
			}
		}

	protected:
		int _start_time;
		int _frames;

		void resetCounts()
		{
			_start_time = SDL_GetTicks();
			_frames = 0;
		}
};

#endif

