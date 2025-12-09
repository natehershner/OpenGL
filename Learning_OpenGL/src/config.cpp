#include "config.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

std::vector<std::string> split(std::string lines, std::string delimiter) {
	std::vector<std::string> splitLine;
	size_t pos = 0;
	std::string token;

	while ((pos = lines.find(delimiter)) != std::string::npos) {
		token = lines.substr(0, pos);
		splitLine.push_back(token);
		lines.erase(0, pos + delimiter.size());
	}

	splitLine.push_back(lines);

	return splitLine;
}
