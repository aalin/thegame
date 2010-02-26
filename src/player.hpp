#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "path.hpp"

class Player
{
	public:
		Player();

		void setPath(Path path) { _path = path; }
		void update();
		void draw();

		Vector3 positionAt(float offset = 0) const
		{
			return _path.positionAt(_path_position + offset);
		}

	private:
		Path _path;
		float _path_position;

};

#endif

