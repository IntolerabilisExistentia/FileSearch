#pragma once

#include <cstdint>
#include <fstream>
#include <map>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>
#include <algorithm>
#include "DirReader.hpp"

std::string toLowerCase(const std::string& str);

std::vector<std::string> splitWords(const std::string& text);

std::map<std::string, std::vector<int>> createPostingsList(const std::string& filePath);

void createPostingsForDirectory(FileReader& fileReader, const std::string& outputFileName);
