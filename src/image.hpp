#ifndef IMAGE_HPP
#define IMAGE_HPP

#include <SDL/SDL.h>
#include <string>
#include "color.hpp"

class Image
{
	public:
		Image(std::string filename);
		~Image();

		Color at(unsigned int x, unsigned int y) const;

		unsigned int getBpp() const { return _image->format->BytesPerPixel; }
		unsigned int getWidth() const { return _image->w; }
		unsigned int getHeight() const { return _image->h; }
		unsigned int getPitch() const { return _image->pitch; }

		void setTexture() const;

	protected:
		SDL_Surface* _image;
		const uint8_t* getPixelData() const
		{
			return reinterpret_cast<uint8_t*>(_image->pixels);
		}
};

#endif

