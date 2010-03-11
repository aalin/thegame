#ifndef CUBE_SCENE_HPP
#define CUBE_SCENE_HPP

#include "player.hpp"
#include "path.hpp"
#include "sky.hpp"
#include "heightmap.hpp"
#include "stone.hpp"
#include <chipmunk.h>

class CubeScene
{
	public:
		CubeScene();
		~CubeScene();
		void update();
		void draw();

		void keyDown(unsigned int key);
		void keyUp(unsigned int key);
		cpVect _player_ground_normal;
		cpArray* _player_ground_shapes;

	private:
		Path _path;
		Sky _sky;
		Heightmap _heightmap;

		void setupSpace();
		void drawCamera();
		void drawPlayer();
		
		void updateArrowDirection(unsigned int key, int multiplier);

		cpSpace* _space;
		cpBody* _static_body;
		cpBody* _player_body;
		cpShape* _player_shape;
		cpVect _arrow_direction;

		bool _last_jump_state;

		std::vector<boost::shared_ptr<Stone> > _stones;
};

#endif

