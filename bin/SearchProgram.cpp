#include "../lib/SearchLib/FileSearch.hpp"

int main(int argc, char* argv[]) {
  if (argc != 2) {
    std::cerr << "Usage: " << argv[0] << " <postings_list_file>" << '\n';
    return 1;
  }

  std::string postingsFile = argv[1];

  std::vector<Document> documents = loadPostingsList(postingsFile);

  std::string query;
  std::cout << "Enter search query: ";
  getline(std::cin, query);

  std::vector<std::string> queryTerms;
  std::stringstream ss(query);
  std::string term;
  while (ss >> term) {
    if (term != "AND" && term != "OR") {
      queryTerms.push_back(term);
    }
  }

  std::vector<DocScore> results = search(documents, query);

  std::cout << "Search results for '" << query << "':" << '\n';
  for (const auto& result : results) {
    std::cout << "Document: " << result.filePath << " - Score: " << result.score << '\n';
    for (const auto& doc : documents) {
      if (doc.filePath == result.filePath) {
        for (const std::string& q : queryTerms) {
          auto it = doc.termInfo.find(q);
          if (it != doc.termInfo.end()) {
            const TermInfo& info = it->second;
            std::cout << q << ' ' << info.firstAppearance << ' ';
            for (const auto& i : info.deltas) {
              std::cout << i << ' ';
            }
            std::cout << '\n';
          }
        }
        break;
      }
    }
  }

  return 0;
}
