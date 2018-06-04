#pragma once
#include "window.h"

#include <vector>
#include <algorithm>
#include <iostream>

class SuffixArray {
  private:
    const Window* window;
    class Suffix;
    std::vector<Suffix> suffixes;
    
    void induceSort(int start, int size, int alphabet);
    void induceSortL(int start, int size, int alphabet, int* buckets, bool* types);
    void induceSortS(int start, int size, int alphabet, int* buckets, bool* types);
    void getBuckets(int* buckets, int start, int size, int alphabet, bool tails);

    int lcp(const Suffix& a, const Suffix& b) const;
  public:
    SuffixArray(const Window* window);
    std::pair<int, int> getMatch();
    std::pair<int, int> getMatchBS();

    void moveM_end(int* oldMiddle, int* lcpIndex, int* left, int* right);
    void moveM_M(int* oldMiddle, int* lcpIndex, int* left, int* right);
    void moveRight(int* lcpIndex, int* middle, int* left);
    void moveLeft(int* lcpIndex, int* middle, int* right);
    int* lcp_lr;
    void buildLcp_Lr(int index, int left, int right);
    std::pair<int, int> getMatchBinarySearch();

    void print() const;
    void show(int pos = -1) const;
    void rebuild();

    friend std::ostream& operator<<(std::ostream& os, const SuffixArray::Suffix& obj);
};

class SuffixArray::Suffix {
  public:
    int lcp;
    const Window* window;
    int index;
    Suffix(const Window*& window, int index) : window(window), index(index), lcp(0) {}

    int length() const {
      return this->window->getDictSize() - index;
    }

    int getIndex() const {
      return index;
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
      return this->length() < other.length();
    }

    friend std::ostream& operator<<(std::ostream& os, const Suffix& obj);
};