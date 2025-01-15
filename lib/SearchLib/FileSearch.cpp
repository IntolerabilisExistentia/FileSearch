#include "FileSearch.hpp"

double calculateIDF(int totalDocs, int docFrequency) {
  return log((totalDocs - docFrequency + 0.5) / (docFrequency + 0.5));
}

double calculateBM25(int termFreq, int docLength, double avgDocLength, double idf, double k1, double b) {
  double tf = ((termFreq * (k1 + 1)) / (termFreq + k1 * (1 - b + b * (docLength / avgDocLength))));
  return tf * idf;
}

std::vector<Document> loadPostingsList(const std::string& fileName) {
  std::ifstream file(fileName);
  std::vector<Document> documents;
  std::string line;
  Document currentDoc;

  while (getline(file, line)) {
    if (line.substr(0, 5) == "File:") {
      if (!currentDoc.filePath.empty()) {
        documents.push_back(currentDoc);
      }
      currentDoc = Document();
      currentDoc.filePath = line.substr(6);
    } else if (!line.empty()) {
      std::stringstream ss(line);
      std::string word;
      getline(ss, word, ':');

      TermInfo termInfo;
      int position;
      if (ss >> position) {
        termInfo.firstAppearance = position;
        char comma;
        while (ss >> comma >> position) {
          termInfo.deltas.push_back(position);
        }
    }

    currentDoc.termInfo[word] = termInfo;
    }
  }

  if (!currentDoc.filePath.empty()) {
      documents.push_back(currentDoc);
  }

  return documents;
}

std::vector<DocScore> combineAnd(const std::vector<DocScore>& first, const std::vector<DocScore>& second) {
  std::unordered_map<std::string, double> firstMap;
  std::vector<DocScore> result;

  for (const auto& f : first) {
    firstMap[f.filePath] = f.score;
  }

  for (const auto& s : second) {
    if (firstMap.find(s.filePath) != firstMap.end()) {
      double combinedScore = firstMap[s.filePath] + s.score;
      result.push_back({s.filePath, combinedScore});
    }
  }

  return result;
}

std::vector<DocScore> combineOr(const std::vector<DocScore>& first, const std::vector<DocScore>& second) {
  std::unordered_map<std::string, double> combinedScores;
  std::vector<DocScore> result;

  for (const auto& f : first) {
    combinedScores[f.filePath] = f.score;
  }

  for (const auto& s : second) {
    combinedScores[s.filePath] += s.score;
  }

  for (const auto& entry : combinedScores) {
    result.push_back({entry.first, entry.second});
  }

  return result;
}

std::vector<DocScore> searchSingleTerm(const std::vector<Document>& documents, const std::string& query) {
  int totalDocs = documents.size();
  double avgDocLength = 0;
  std::unordered_map<std::string, int> docFrequency;

  for (const auto& doc : documents) {
    int docLength = 0;
    for (const auto& term : doc.termInfo) {
      docLength += term.second.deltas.size() + 1;
      docFrequency[term.first]++;
    }
    avgDocLength += docLength;
  }

  avgDocLength /= totalDocs;

  std::vector<DocScore> scores;

  for (const auto& doc : documents) {
    auto it = doc.termInfo.find(query);
    if (it != doc.termInfo.end()) {
      int termFreq = it->second.deltas.size() + 1;
      int docLength = 0;
      for (const auto& term : doc.termInfo) {
        docLength += term.second.deltas.size() + 1;
      }

      double idf = calculateIDF(totalDocs, docFrequency[query]);
      double bm25Score = calculateBM25(termFreq, docLength, avgDocLength, idf);
      
      scores.push_back({doc.filePath, bm25Score, });
    }
  }

  sort(scores.begin(), scores.end(), [](const DocScore& a, const DocScore& b) {
    return a.score > b.score;
  });

  return scores;
}

std::vector<DocScore> search(const std::vector<Document>& documents, const std::string& query) {
  std::vector<std::string> terms;
  std::stringstream ss(query);
  std::string term;
  std::string operatorType = "AND";

  while (ss >> term) {
    if (term == "AND" || term == "OR") {
      operatorType = term;
    } else {
      terms.push_back(term);
    }
  }

  if (terms.empty()) {
    return {};
  }

  std::vector<DocScore> currentResults = searchSingleTerm(documents, terms[0]);
  for (size_t i = 1; i < terms.size(); ++i) {
    std::vector<DocScore> nextResults = searchSingleTerm(documents, terms[i]);

    if (operatorType == "AND") {
      currentResults = combineAnd(currentResults, nextResults);
    } else if (operatorType == "OR") {
      currentResults = combineOr(currentResults, nextResults);
    }
  }

  sort(currentResults.begin(), currentResults.end(), [](const DocScore& a, const DocScore& b) {
    return a.score > b.score;
  });

  return currentResults;
}
