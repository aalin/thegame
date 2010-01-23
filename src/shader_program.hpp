#ifndef SHADER_PROGRAM_HPP
#define SHADER_PROGRAM_HPP

#include <vector>
#include <string>
#include "opengl.hpp"

class ShaderProgram
{
	class Shader
	{
		public:
			enum ShaderType
			{
				VERTEX_SHADER = GL_VERTEX_SHADER,
				FRAGMENT_SHADER = GL_FRAGMENT_SHADER
			};

			Shader(std::string filename, ShaderType shader_type);
			~Shader();

			unsigned int getId() const { return _id; }

		private:
			unsigned int _id;
	};

	public:
		ShaderProgram(std::string basename);
		~ShaderProgram();

		void use()
		{
			glUseProgram(_id);
		}

	private:
		void link();
		unsigned int _id;
		std::vector<Shader*> _shaders;
};

#endif

