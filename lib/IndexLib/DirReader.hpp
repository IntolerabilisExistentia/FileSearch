#pragma once

#include <cstdint>
#include <fstream>
#include <string>
#include <utility>
#include <vector>
#include <iostream>
#include <filesystem>

class FileReader {
public:
  bool GetFiles(int argc, char** argv);

  std::vector<std::pair<std::string, std::vector<std::string>>>& GetFilesIndex();

  void WriteToFile(std::ofstream& out, std::vector<uint64_t>& words_count);

private:
  void GetFilesFromDirectory(const std::string& path);

  std::vector<std::pair<std::string, std::vector<std::string>>> files_;
};
