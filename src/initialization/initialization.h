#pragma once

#include<vector>
#include <tuple>
#include <string>

std::vector<std::tuple<uint32_t, uint32_t, uint32_t>> readColorsFile(std::string filename);

std::vector<std::string> readWorldsFile(std::string filename);