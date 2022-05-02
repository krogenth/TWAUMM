#include "./initialization.h"

#include <fstream>

std::vector<std::tuple<uint32_t, uint32_t, uint32_t>> readColorsFile(std::string filename) {
	std::vector<std::tuple<uint32_t, uint32_t, uint32_t>> list;
	std::ifstream file(filename);
	if (!file.is_open()) {
		return list;
	}

	uint32_t r, g, b;
	while (!file.eof()) {
		file >> r >> g >> b;
		list.emplace_back(std::make_tuple(r, g, b));
	}
	file.close();
	return list;
}

std::vector<std::string> readWorldsFile(std::string filename) {
	std::vector<std::string> list;
	std::ifstream file(filename);
	if (!file.is_open()) {
		return list;
	}

	std::string world;
	while (!file.eof()) {
		file >> world;
		if (world.at(0) != ';') {
			list.emplace_back(world);
		}
	}
	file.close();
	return list;
}