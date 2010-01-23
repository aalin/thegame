#ifndef COLOR_HPP
#define COLOR_HPP

#include "opengl.hpp"

struct Color
{
	Color(float r, float g, float b) : r(r), g(g), b(b)
	{ }

	Color() : r(0.0), g(0.0), b(0.0)
	{ }

	void draw() { glColor3f(r, g, b); }

	float r, g, b;
};

#endif

