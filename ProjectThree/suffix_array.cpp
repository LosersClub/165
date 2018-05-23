#include "suffix_array.h"

#include <iostream>
#include <iomanip>

SuffixArray::SuffixArray(const std::string& str) {
  for (std::string::size_type i = 0; i < str.size(); i++) {
    this->suffixes.push_back(Suffix(str.substr(i), str.size() - i));
  }
  std::sort(this->suffixes.begin(), this->suffixes.end());
  for (std::size_t i = 1; i < this->suffixes.size(); i++) {
    this->suffixes[i].lcp = lcp(this->suffixes.at(i), this->suffixes.at(i - 1));
  }
}

void SuffixArray::print() const {
  std::cout << " i  lcp  str" << std::endl;
  std::cout << "----------------" << std::endl;
  for (Suffix s : this->suffixes) {
    std::cout << std::setw(2) << s.index << "  " << std::setw(2) <<
      s.lcp << "  " << s.val << std::endl;
  }
}

std::pair<int, int> SuffixArray::getMatch(const std::string& query) {
  int offset = 0;
  int len = 0;
  for (int i = 0; i < this->suffixes.size(); i++) {
    Suffix current = this->suffixes[i];

    if ((len > 0 && current.lcp == 0) || len == query.size()) {
      break;
    }

    if (len == current.lcp) {
      int match = lcp(current.val, query, len);
      if (match > len) {
        offset = current.index;
        len = match;
      }
    }
  }
  return std::pair<int, int>{len, offset};
}

int SuffixArray::lcp(const Suffix& a, const Suffix& b) const {
  return lcp(a.val, b.val, 0);
}

int SuffixArray::lcp(const std::string& a, const std::string& b, const int& index) const {
  int n = std::min(a.size(), b.size());
  if (index >= n) {
    return 0;
  }
  for (std::string::size_type i = index; i < n; i++) {
    if (a[i] != b[i]) {
      return i;
    }
  }
  return n;
}