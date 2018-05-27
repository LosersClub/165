#pragma once
#include "window.h"

#include <vector>
#include <algorithm>

class SuffixArray {
  private:
    int prevDictSize;
    const Window* window;
    class Suffix;
    std::vector<Suffix> suffixes;

    int lcp(const Suffix& a, const Suffix& b) const;
  public:
    SuffixArray(const Window* window);
    std::pair<int, int> getMatch();
    void print() const;
    void rebuild();
};

class SuffixArray::Suffix {
  public:
    const Window* window;
    int index;
    int lcp;
    Suffix(const Window*& window, int index) : window(window), index(index), lcp(0) {}

    int length() const {
      return this->window->getDictSize() - index;
    }

    const char& operator[](const int i) const {
      return this->window->getFromDict(index + i);
    }

    bool operator<(const Suffix& other) const {
      int n = std::min(this->length(), other.length());
      for (int i = 0; i < n; i++) {
        if ((*this)[i] != other[i]) {
          return (*this)[i] < other[i];
        }
      }
      return this->length() > other.length();
    }
};