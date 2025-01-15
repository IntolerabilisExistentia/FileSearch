#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>
#include <cmath>
#include <algorithm>

struct TermInfo {
  int firstAppearance;
  std::vector<int> deltas;
};

struct Document {
  std::string filePath;
  std::unordered_map<std::string, TermInfo> termInfo;
};

struct DocScore {
  std::string filePath;
  double score;
};

double calculateIDF(int totalDocs, int docFrequency);

double calculateBM25(int termFreq, int docLength, double avgDocLength, double idf, double k1 = 1.5, double b = 0.75);

std::vector<Document> loadPostingsList(const std::string& postingsFile);

std::vector<DocScore> combineAnd(const std::vector<DocScore>& first, const std::vector<DocScore>& second);

std::vector<DocScore> combineOr(const std::vector<DocScore>& first, const std::vector<DocScore>& second);

std::vector<DocScore> searchSingleTerm(const std::vector<Document>& documents, const std::string& query);

std::vector<DocScore> search(const std::vector<Document>& documents, const std::string& query);
