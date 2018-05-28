#include "suffix_array.h"

#include <iomanip>

bool isLMS(const int i, bool*& types) {
  return i > 0 && types[i] && !types[i - 1];
}

void showTypeMap(bool*& types, int size) {
  for (int i = 0; i < size; i++) {
    std::cout << (types[i] ? "S" : "L");
  }
  std::cout << std::endl;
  for (int i = 0; i < size; i++) {
    std::cout << (isLMS(i, types) ? "^" : " ");
  }
  std::cout << std::endl;
}

SuffixArray::SuffixArray(const Window* window) : window(window) {
  for (int i = 0; i <= this->window->getDictCap(); i++) {
    this->suffixes.push_back(Suffix(this->window, -1));
  }
  rebuild();
}

void SuffixArray::rebuild() {
  for (Suffix& s : suffixes) {
    s.index = -1;
  }

  int size = this->window->getDictSize();
  // Build the type Map
  bool* types = new bool[size + 1];
  types[size] = true; // S_TYPE
  types[size - 1] = false; // L_TYPE
  for (int i = size - 2; i >= 0; i--) {
    types[i] = this->window->getFromDict(i) < this->window->getFromDict(i + 1) ||
      (this->window->getFromDict(i) == this->window->getFromDict(i + 1) && types[i + 1]);
  }

  showTypeMap(types, size + 1);

  // Build the buckets array
  int* buckets = new int[257];

  // Peform a simple LMS sort guessing positions
  getBuckets(0, size, 256, buckets, true);
  for (int i = 0; i < size; i++) {
    if (isLMS(i, types)) {
      int bucketIndex = this->window->getFromDict(i);
      this->suffixes[buckets[bucketIndex]--].index = i;
      this->show();
    }
  }
  this->suffixes[0].index = size;
  this->show();

  // Sort non-LMS strings
  induceSortL(0, size, 256, buckets, types);
  induceSortS(0, size, 256, buckets, types);


  // Summary of relative order to recur on

  // Sort non-LMS strings
  //induceSortL(start, end, alphabet, buckets, types);
  //induceSortS(start, end, alphabet, buckets, types);

  free(buckets);
  free(types);


  //induceSort(0, window->getDictSize(), 256);

  //for (std::size_t i = 1; i < this->suffixes.size(); i++) {
  //  this->suffixes[i].lcp = lcp(this->suffixes.at(i), this->suffixes.at(i - 1));
  //}
}

// end is exclusive, start inclusive
void SuffixArray::induceSort(int start, int end, int alphabet) {
  int size = end - start;
  // Build the type Map
  bool* types = new bool[size + 1];
  types[size] = true; // S_TYPE
  types[size - 1] = false; // L_TYPE
  for (int i = size - 2; i >= 0; i--) {
    types[i] = this->window->getFromDict(start + i) < this->window->getFromDict(start + i + 1) ||
      (this->window->getFromDict(start + i) == this->window->getFromDict(start + i + 1) && types[i+1]);
  }

  showTypeMap(types, size + 1);

  // Build the buckets array
  int* buckets = new int[alphabet + 1];

  // Peform a simple LMS sort guessing positions
  getBuckets(start, end, alphabet, buckets, true);
  for (int i = 0; i < size; i++) {
    if (isLMS(i, types)) {
      int bucketIndex = this->window->getFromDict(start + i);
      this->suffixes[buckets[bucketIndex]--].index = start + i;
      this->show();
    }
  }
  this->suffixes[0].index = size;
  this->show();

  // Sort non-LMS strings
  induceSortL(start, end, alphabet, buckets, types);
  induceSortS(start, end, alphabet, buckets, types);


  // Summary of relative order to recur on
  int newSize = 0;
  for (int i = start; i < end; i++) {
    if (isLMS(this->suffixes[i].index, types)) {
      this->suffixes[newSize++].index = this->suffixes[i].index;
    }
  }

  free(buckets);
  free(types);
}

void SuffixArray::getBuckets(int start, int end, int alphabet, int* buckets, bool tails) {
  for (int i = 0; i <= alphabet; i++) {
    buckets[i] = 0;
  }
  for (int i = start; i < end; i++) {
    buckets[this->window->getFromDict(i)] += 1;
  }
  int sum = 1;
  for (int i = 0; i <= alphabet; i++) {
    sum += buckets[i];
    buckets[i] = tails ? sum - 1 : sum - buckets[i];
  }
}

void SuffixArray::induceSortL(int start, int end, int alphabet, int* buckets, bool* types) {
  getBuckets(start, end, alphabet, buckets, false);
  int size = end - start;
  for (int i = 0; i < size; i++) {
    int j = this->suffixes[i].index - 1;
    if (j >= 0 && !types[j]) {
      int bucketIndex = this->window->getFromDict(start + j);
      this->suffixes[buckets[bucketIndex]++].index = start + j;
      this->show(i);
    }
  }
}

void SuffixArray::induceSortS(int start, int end, int alphabet, int* buckets, bool* types) {
  getBuckets(start, end, alphabet, buckets, true);
  int size = end - start;
  for (int i = size; i >= 0; i--) {
    int j = this->suffixes[i].index - 1;
    if (j >= 0 && types[j]) {
      int bucketIndex = this->window->getFromDict(start + j);
      this->suffixes[buckets[bucketIndex]--].index = start + j;
      this->show(i);
    }
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
        offset = this->window->getDictSize() - current.getIndex();
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

std::ostream& operator<<(std::ostream& os, const SuffixArray::Suffix& obj) {
  os << std::setw(2) << obj.index << "  " << std::setw(2) << obj.lcp << "  ";
  for (int i = obj.index; i < obj.window->getDictSize(); i++) {
    os << obj.window->getFromDict(i);
  }
  return os;
}

void SuffixArray::print() const {
  std::cout << " i  lcp  str" << std::endl;
  std::cout << "----------------" << std::endl;
  for (Suffix s : this->suffixes) {
    std::cout << s << std::endl;
  }
}

void SuffixArray::show(int pos) const {
  for (Suffix s : this->suffixes) {
    std::cout << std::setfill('0') << std::setw(2) << s.index << " ";
  }
  std::cout << std::endl;
  if (pos >= 0) {
    for (int i = 0; i < this->suffixes.size(); i++) {
      std::cout << (i == pos ? "^^" : "  ") << " ";
    }
    std::cout << std::endl;
  }
}