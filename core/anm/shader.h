//shader.h
#pragma once
#include <sstream>
#include <fstream>

namespace anm {
	std::string loadShaderSourceFromFile(const std::string& filePath);
}