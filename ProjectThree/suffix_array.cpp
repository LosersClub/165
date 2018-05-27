#include "suffix_array.h"

#include <iostream>
#include <iomanip>

SuffixArray::SuffixArray(const Window* window) : window(window) {
  prevDictSize = 0;
  rebuild();
}

void SuffixArray::rebuild() {
  if (prevDictSize < window->getDictSize()) {
    for (int i = prevDictSize; i < this->window->getDictSize(); i++) {
      this->suffixes.push_back(Suffix(this->window, i));
    }
  }
  prevDictSize = window->getDictSize();
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
      s.lcp << "  ";
    for (int i = s.index; i < s.window->getDictSize(); i++) {
      std::cout << s[i];
    }
    std::cout << std::endl;
  }
}

std::pair<int, int> SuffixArray::getMatch() {
  int offset = 0;
  int len = 0;
  for (int i = 0; i < this->suffixes.size(); i++) {
    Suffix current = this->suffixes[i];

    if ((len > 0 && current.lcp == 0) || len == this->window->getLabSize()) {
      break;
    }

    if (len == current.lcp) {
      int match = this->window->getLabSize();
      for (int j = len; j < this->window->getLabSize(); j++) {
        int suffixIndex = j % current.length();
        if (this->window->getFromLab(j) != current[suffixIndex]) {
          match = j;
          break;
        }
      }
      if (match > len) {
        offset = this->window->getDictSize() - current.index;
        len = match;
      }
    }
  }
  return std::pair<int, int>{len, offset};
}

int SuffixArray::lcp(const Suffix& a, const Suffix& b) const {
  int n = std::min(a.length(), b.length());
  for (int i = 0; i < n; i++) {
    if (a[i] != b[i]) {
      return i;
    }
  }
  return n;
}