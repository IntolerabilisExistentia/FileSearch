#include "../lib/IndexLib/PostingsFile.hpp"

#include <iostream>

int main(int argc, char** argv) {
   FileReader fileReader;

  if (argc < 3) {
    std::cerr << "Usage: " << argv[0] << " <output-file> <directory-path> [<directory-path> ...]" << '\n';
    return 1;
  }

  if (!fileReader.GetFiles(argc, argv)) {
    return 1;
  }

  std::string outputFileName = argv[1];
  createPostingsForDirectory(fileReader, outputFileName);

  std::cout << "Postings lists have been written to " << outputFileName << '\n';

  return 0;
}
