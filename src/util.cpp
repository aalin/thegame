#include "util.hpp"
#include <fstream>

std::vector<char> Util::loadFile(std::string filename)
{
	std::ifstream f(filename.c_str());

	if(!f.good())
		throw "could not open file";

	f.seekg(0, std::ios::end);
	int size = f.tellg();
	f.seekg(0, std::ios::beg);

	std::vector<char> buffer(size + 1, 0);

	f.read(&buffer[0], size);
	f.close();

	return buffer;
}
