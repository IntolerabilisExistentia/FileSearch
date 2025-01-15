#include "PostingsFile.hpp"

 std::string toLowerCase(const std::string& str) {
  std::string lowerStr = str;
  std::transform(lowerStr.begin(), lowerStr.end(), lowerStr.begin(), ::tolower);
  return lowerStr;
}

std::vector<std::string> splitWords(const std::string& text) {
  std::vector<std::string> words;
  std::string word;

  for (char ch : text) {
    if (std::isalnum(ch)) {
      word += ch;
    } else if (!word.empty()) {
      words.push_back(word);
      word.clear();
    }
  }
  
  if (!word.empty()) {
    words.push_back(word); 
  }

  return words;
}

std::map<std::string, std::vector<int>> createPostingsList(const std::string& filePath) {
  std::ifstream inputFile(filePath);
  std::map<std::string, std::vector<int>> postingsList;
  
  if (!inputFile.is_open()) {
    std::cerr << "Error: Could not open file " << filePath << '\n';
    return postingsList;
  }

  std::string line;
  int position = 0;

  while (std::getline(inputFile, line)) {
    std::vector<std::string> words = splitWords(line);

    for (const std::string& word : words) {
      std::string lowerWord = toLowerCase(word);

      if (postingsList[lowerWord].empty()) {
        postingsList[lowerWord].push_back(position);
      } else {
        int lastPosition = postingsList[lowerWord].back();
        postingsList[lowerWord].push_back(position - lastPosition);
      }
      position++;
    }
  }

  inputFile.close();
  return postingsList;
}

void createPostingsForDirectory(FileReader& fileReader, const std::string& outputFileName) {
  std::ofstream outputFile(outputFileName);

  if (!outputFile.is_open()) {
    std::cerr << "Error: Could not open output file." << '\n';
    return;
  }

  auto files = fileReader.GetFilesIndex();

  for (const auto& filePair : files) {
    const std::string& directoryPath = filePair.first;
    const std::vector<std::string>& fileList = filePair.second;

    for (const std::string& filePath : fileList) {
      std::string fullPath = directoryPath + filePath;
      outputFile << "File: " << fullPath << '\n';

      auto postingsList = createPostingsList(fullPath);
      for (const auto& entry : postingsList) {
        outputFile << entry.first << ": ";
        for (size_t i = 0; i < entry.second.size(); ++i) {
          outputFile << entry.second[i];
          if (i != entry.second.size() - 1) {
            outputFile << ", ";
          }
        }
        outputFile << '\n';
      }
      outputFile << '\n';
    }
  }

  outputFile.close();
}
