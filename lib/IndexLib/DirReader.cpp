#include "DirReader.hpp"
#include <filesystem>

bool FileReader::GetFiles(int argc, char** argv) {
  for(int i = 2; i < argc; ++i) {
    if (!std::filesystem::exists(argv[i])) {
      std::cerr << argv[1] << " doesn't exist.\n";
      files_.clear();

      return false;
    }
    files_.push_back({argv[i], std::vector<std::string> ()});
    GetFilesFromDirectory(argv[i]);
  }
  return true;
}

std::vector<std::pair<std::string, std::vector<std::string>>>& FileReader::GetFilesIndex() {
  return files_;
}

void FileReader::GetFilesFromDirectory(const std::string& path) {
  for (const auto& file_name : std::filesystem::directory_iterator(path)) {
    if (std::filesystem::is_directory(file_name)) {
      GetFilesFromDirectory(file_name.path().string());
    } else {
      files_.back().second.push_back(file_name.path().string().substr(
        files_.back().first.size(), file_name.path().string().size()
      ));
    }
  }
}

void FileReader::WriteToFile(std::ofstream& out, std::vector<uint64_t>& words_count) {
  out << words_count[0] << ' ' << files_.size() << '\n';
  uint64_t cur_ind = 0;
  for (auto& i : files_) {
    out << i.first << ' ' << i.second.size() << '\n';
    for (auto& j : i.second) {
      out << j << ' ' << words_count[cur_ind + 1] << '\n';
      ++cur_ind;
    }
  }
}
