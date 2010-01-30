#include <iostream>
#include "shader_program.hpp"
#include "util.hpp"
#include <boost/foreach.hpp>

// Shader

ShaderProgram::Shader::Shader(std::string filename, ShaderType shader_type)
{
	_id = glCreateShader(shader_type);

	std::vector<char> data(Util::loadFile(filename.c_str()));
	glShaderSource(_id, 1, reinterpret_cast<const GLchar**>(&data[0]), 0);

	glCompileShader(_id);

	int result = 0;
	glGetShaderiv(_id, GL_COMPILE_STATUS, &result);
	switch(result)
	{
		case GL_TRUE:
			std::cout << filename << " compiled successfully!" << std::endl;
			break;
		case GL_FALSE:
			std::cout << gluErrorString(glGetError()) << std::endl;
			throw "shader compilation failed";
			break;
		default:
			std::cout << "Result: " << result << std::endl;
			break;
	}
}

ShaderProgram::Shader::~Shader()
{
	glDeleteShader(_id);
}

// Shader program

ShaderProgram::ShaderProgram(std::string basename)
{
	_id = glCreateProgram();
	_shaders.push_back(new Shader(basename + ".vert", ShaderProgram::Shader::VERTEX_SHADER));
	_shaders.push_back(new Shader(basename + ".frag", ShaderProgram::Shader::FRAGMENT_SHADER));

	BOOST_FOREACH(Shader* shader, _shaders)
	{
		glAttachShader(_id, shader->getId());
	}

	glLinkProgram(_id);
}

ShaderProgram::~ShaderProgram()
{
	BOOST_FOREACH(Shader* shader, _shaders)
	{
		glAttachShader(_id, shader->getId());
		delete shader;
	}

	glDeleteProgram(_id);
}

