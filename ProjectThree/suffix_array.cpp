#include "suffix_array.h"

#include <iomanip>
#include <algorithm>

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
  induceSort(nullptr, window->getDictSize(), 256);

  //for (std::size_t i = 1; i < this->suffixes.size(); i++) {
  //  this->suffixes[i].lcp = lcp(this->suffixes.at(i), this->suffixes.at(i - 1));
  //}
}

int SuffixArray::get(int i, int* summary) const {
  return summary == nullptr ? this->window->getFromDict(i) : summary[i];
}

// end is exclusive, start inclusive
void SuffixArray::induceSort(int* summary, int size, int alphabet) {
  // Build the type Map
  bool* types = new bool[size + 1];
  types[size] = true; // S_TYPE
  types[size - 1] = false; // L_TYPE
  for (int i = size - 2; i >= 0; i--) {
    types[i] = this->get(i, summary) < this->get(i + 1, summary) ||
      (this->get(i, summary) == this->get(i + 1, summary) && types[i + 1]);
  }

  showTypeMap(types, size + 1);

  // Build the buckets array
  int* buckets = new int[alphabet + 1];

  // Peform a simple LMS sort guessing positions
  getBuckets(summary, size, alphabet, buckets, true);
  for (int i = 0; i < size; i++) {
    if (isLMS(i, types)) {
      int bucketIndex = this->get(i, summary);
      this->suffixes[buckets[bucketIndex]--].index = i;
      this->show();
    }
  }
  this->suffixes[0].index = size;
  this->show();

  // Sort non-LMS strings
  induceSortL(summary, size, alphabet, buckets, types);
  induceSortS(summary, size, alphabet, buckets, types);

  std::cout << "Summary" << std::endl;
  // Summary of relative order to recur on
  int newSize = 0;
  for (int i = 0; i <= size; i++) {
    if (isLMS(this->suffixes[i].index, types)) {
      this->suffixes[newSize++].index = this->suffixes[i].index;
    }
  }
  for (int i = newSize; i <= size; i++) {
    this->suffixes[i].index = -1;
  }
  this->show();

  int* summaryString = new int[size - newSize + 1];
  int* offsets = new int[size - newSize + 1];
  summaryString[size - newSize] = 0;
  offsets[size - newSize] = this->suffixes[0].index;
  int name = 1, prev = this->suffixes[0].index; // switch to set first?
  int j = 0;
  for (int i = 1; i < newSize; i++) {
    int pos = this->suffixes[i].index;
    bool diff = false;
    for (int d = 0; d < size; d++) {
      if (this->get(pos + d, summary) != this->get(prev + d, summary) ||
        types[pos + d] != types[prev + d]) {
        diff = true;
        break;
      }
      else if (d > 0 && (isLMS(pos + d, types) || isLMS(prev + d, types))) {
        break;
      }
    }
    if (diff) {
      name++;
      prev = pos;
    }
    int index = pos % 2 == 0 ? pos / 2 : (pos - 1) / 2;
    summaryString[index] = name - 1;
    offsets[index] = pos;
  }
  for (int i = 0, j = 0; i <= size - newSize; i++) {
    if (offsets[i] >= 0) {
      offsets[j] = offsets[i];
      summaryString[j++] = summaryString[i];
    }
  }

  for (int i = 0; i < newSize; i++) {
    std::cout << offsets[i] << " ";
  }
  std::cout << std::endl;

  for (int i = 0; i < newSize; i++) {
    std::cout << summaryString[i] << " ";
  }
  std::cout << std::endl;

  //for (int i = 0; i < newSize; i++) {
  //  offsets[i] = this->suffixes[i].index;
  //}

  // Recur if newAlphabet < newSize (non-unique vals)
  if (name != newSize) {
    induceSort(summaryString, newSize, name);
  } else {
    this->suffixes[0].index = newSize;
    for (int i = 0; i < newSize; i++) {
      this->suffixes[summaryString[i]+1].index = i;
    }
  }
  this->show();

  std::cout << "FINAL" << std::endl;
  int* summarySuffix = new int[newSize + 1];
  for (int i = 0; i <= newSize; i++) {
    summarySuffix[i] = this->suffixes[i].index;
  }

  // Accurate/final LMS sort
  getBuckets(summary, size, alphabet, buckets, true);
  for (int i = newSize; i > 1; i--) {
    int stringIndex = offsets[summarySuffix[i]];
    int bucketIndex = this->get(stringIndex, summary);
    this->suffixes[buckets[bucketIndex]--].index = stringIndex;
    this->show();
  }
  this->suffixes[0].index = size;
  this->show();

  // Sort non-LMS strings
  induceSortL(summary, size, alphabet, buckets, types);
  induceSortS(summary, size, alphabet, buckets, types);

  free(summaryString);
  free(offsets);
  free(buckets);
  free(types);
}

void SuffixArray::getBuckets(int* summary, int size, int alphabet, int* buckets, bool tails) {
  for (int i = 0; i <= alphabet; i++) {
    buckets[i] = 0;
  }
  for (int i = 0; i < size; i++) {
    buckets[this->get(i, summary)] += 1;
  }
  int sum = 1;
  for (int i = 0; i <= alphabet; i++) {
    sum += buckets[i];
    buckets[i] = tails ? sum - 1 : sum - buckets[i];
  }
}

void SuffixArray::induceSortL(int* summary, int size, int alphabet, int* buckets, bool* types) {
  getBuckets(summary, size, alphabet, buckets, false);
  for (int i = 0; i < size; i++) {
    int j = this->suffixes[i].index - 1;
    if (j >= 0 && !types[j]) {
      int bucketIndex = this->get(j, summary);
      this->suffixes[buckets[bucketIndex]++].index = j;
      this->show(i);
    }
  }
}

void SuffixArray::induceSortS(int* summary, int size, int alphabet, int* buckets, bool* types) {
  getBuckets(summary, size, alphabet, buckets, true);
  for (int i = size; i >= 0; i--) {
    int j = this->suffixes[i].index - 1;
    if (j >= 0 && types[j]) {
      int bucketIndex = this->get(j, summary);
      this->suffixes[buckets[bucketIndex]--].index = j;
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