#include <SDL/SDL_image.h>
#include <iostream>
#include "image.hpp"

Image::Image(std::string filename)
{
	std::cout << "Loading image: " << filename << std::endl;
	_image = IMG_Load(filename.c_str());

	if(!_image)
	{
		std::string msg = "IMG_Load: " + std::string(IMG_GetError());
		throw msg.c_str();
	}

	std::cout << "Image loaded: " << filename << std::endl;
	std::cout << "\tDimensions: " << getWidth() << "x" << getHeight() << std::endl;
	std::cout << "\tbpp: " << getBpp() << std::endl;
	std::cout << "\tpitch: " << getPitch() << std::endl;
}

Image::~Image()
{
	if(_image)
		SDL_FreeSurface(_image);
}

void Image::setTexture() const
{
	if(getBpp() == 4)
		gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, getWidth(), getHeight(), GL_RGBA, GL_UNSIGNED_BYTE, _image->pixels);
	else
		gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, getWidth(), getHeight(), GL_RGB, GL_UNSIGNED_BYTE, _image->pixels);
}

Color Image::at(unsigned int x, unsigned int y) const
{
	// TODO: RGBA
	const uint8_t *p = getPixelData() + y * getPitch() + x * getBpp();
	uint32_t rgba = *reinterpret_cast<const uint32_t*>(p);
	float r = ((rgba & 0x00ff0000) >> 16) / 255.0;
	float g = ((rgba & 0x0000ff00) >> 8) / 255.0;
	float b = ((rgba & 0xff000000) >> 24) / 255.0;
	return Color(r, g, b);
}


