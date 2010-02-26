#include "player.hpp"
#include "opengl.hpp"
#include <cmath>

Player::Player()
{
	_path_position = 0.0;
}

void Player::update()
{
	_path_position += 0.1;
}

void Player::draw()
{
	Vector3 player_position(positionAt(0));

	glPointSize(50.0);
	glBegin(GL_POINTS);
		glVertex3f(player_position.x, player_position.y, player_position.z);
	glEnd();

	_path.draw();
}

