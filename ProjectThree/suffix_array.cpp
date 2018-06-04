#include "suffix_array.h"

#include <iomanip>
#include <algorithm>

SuffixArray::SuffixArray(const Window* window) : window(window) {
  this->suffixes.reserve(this->window->getDictSize() + 1);
  for (int i = 0; i <= this->window->getDictCap(); i++) {
    this->suffixes.push_back(Suffix(this->window, -1));
  }

  this->lcp_lr = new int[100000];
  memset(this->lcp_lr, 10000, 100000);

  rebuild();
}

void SuffixArray::rebuild() {
  induceSort(0, this->window->getDictSize() + 1, 256);

  for (int i = 1; i <= this->window->getDictSize(); i++) {
    this->suffixes[i].lcp = lcp(this->suffixes.at(i), this->suffixes.at(i - 1));
  }
}

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

#define comp(i) (start > 0 ? this->suffixes[start + i].index : \
                    (i >= this->window->getDictSize() ? '\0' : this->window->getFromDict(i)))
void SuffixArray::induceSort(int start, int size, int alphabet) {
  bool* types = new bool[size];
  types[size - 1] = true; // S_TYPE
  types[size - 2] = false; // L_TYPE
  for (int i = size - 3; i >= 0; i--) {
    types[i] = comp(i) < comp(i + 1) || (comp(i) == comp(i + 1) && types[i + 1]);
  }

  int* buckets = new int[alphabet + 1];
  getBuckets(buckets, start, size, alphabet, true);
  for (int i = 0; i < size; i++) {
    this->suffixes[i].index = -1;
  }
  for (int i = 1; i < size; i++) {
    if (isLMS(i, types)) {
      this->suffixes[--buckets[comp(i)]].index = i;
    }
  }
  induceSortL(start, size, alphabet, buckets, types);
  induceSortS(start, size, alphabet, buckets, types);

  int newSize = 0;
  for (int i = 0; i < size; i++) {
    if (isLMS(this->suffixes[i].index, types)) {
      this->suffixes[newSize++].index = this->suffixes[i].index;
    }
  }
  for (int i = newSize; i < size; i++) {
    this->suffixes[i].index = -1;
  }

  int name = 0, prev = -1;
  for (int i = 0; i < newSize; i++) {
    int pos = this->suffixes[i].index;
    bool diff = false;
    for (int j = 0; j < size; j++) {
      if (prev == -1 || comp(pos + j) != comp(prev + j) || types[pos + j] != types[prev + j]) {
        diff = true;
        break;
      } else if (j > 0 && (isLMS(pos + j, types) || isLMS(prev + j, types))) {
        break;
      }
    } 
    if (diff) {
      name++;
      prev = pos;
    }
    pos = (pos % 2 == 0) ? pos / 2 : (pos - 1) / 2;
    suffixes[pos + newSize].index = name - 1;
  }

  for (int i = size - 1, j = size - 1; i >= newSize; i--) {
    if (this->suffixes[i].index >= 0) {
      this->suffixes[j--].index = suffixes[i].index;
    }
  }

  if (name < newSize) {
    induceSort(size - newSize, newSize, name - 1);
  } else {
    for (int i = 0; i < newSize; i++) {
      this->suffixes[this->suffixes[size - newSize + i].index].index = i;
    }
  }

  getBuckets(buckets, start, size, alphabet, true);
  for (int i = 1, j = 0; i < size; i++) {
    if (isLMS(i, types)) {
      this->suffixes[size - newSize + j].index = i;
      j += 1;
    }
  }

  for (int i = 0; i < newSize; i++) {
    this->suffixes[i].index = this->suffixes[size - newSize + this->suffixes[i].index].index;
  }
  for (int i = newSize; i < size; i++) {
    this->suffixes[i].index = -1;
  }
  for (int i = newSize - 1; i >= 0; i--) {
    int j = this->suffixes[i].index;
    this->suffixes[i].index = -1;
    this->suffixes[--buckets[comp(j)]].index = j;
  }
  induceSortL(start, size, alphabet, buckets, types);
  induceSortS(start, size, alphabet, buckets, types);
  free(buckets);
  free(types);
}

void SuffixArray::getBuckets(int* buckets, int start, int size, int alphabet, bool tails) {
  for (int i = 0; i <= alphabet; i++) {
    buckets[i] = 0;
  }
  for (int i = 0; i < size; i++) {
    buckets[comp(i)] += 1;
  }
  int sum = 0;
  for (int i = 0; i <= alphabet; i++) {
    sum += buckets[i];
    buckets[i] = tails ? sum : sum - buckets[i];
  }
}

void SuffixArray::induceSortL(int start, int size, int alphabet, int* buckets, bool* types) {
  getBuckets(buckets, start, size, alphabet, false);
  for (int i = 0; i < size; i++) {
    int j = this->suffixes[i].index - 1;
    if (j >= 0 && !types[j]) {
      this->suffixes[buckets[comp(j)]++].index = j;
    }
  }
}

void SuffixArray::induceSortS(int start, int size, int alphabet, int* buckets, bool* types) {
  getBuckets(buckets, start, size, alphabet, true);
  for (int i = size - 1; i >= 0; i--) {
    int j = this->suffixes[i].index - 1;
    if (j >= 0 && types[j]) {
      this->suffixes[--buckets[comp(j)]].index = j;
    }
  }
}

void SuffixArray::buildLcp_Lr(int index, int left, int right) {
  int middle = (left + right) / 2;
  if (left == right) {
    if (left == 1 && this->window->getDictSize() > 1) {
      this->lcp_lr[index] = this->suffixes[left + 1].lcp;
    }
    else {
      this->lcp_lr[index] = this->suffixes[left].lcp;
    }
    return;
  }

  buildLcp_Lr(2 * index, left, middle);
  buildLcp_Lr(2 * index + 1, middle + 1, right);
  this->lcp_lr[index] = std::min(this->lcp_lr[2 * index], this->lcp_lr[2 * index + 1]);
}

void SuffixArray::moveRight(int* lcpIndex, int* middle, int* left) {
  *left = *middle + 1;
  *lcpIndex = *lcpIndex * 2 + 1;
}

void SuffixArray::moveLeft(int* lcpIndex, int* middle, int* right) {
  *right = *middle;
  *lcpIndex = *lcpIndex * 2;
}

void SuffixArray::moveM_end(int* oldMiddle, int* lcpIndex, int* left, int* right) {
  int newMiddle = (*left + *right - 1) / 2;
  if (newMiddle < *oldMiddle) {
    *right = newMiddle;
    *lcpIndex = *lcpIndex * 2;
  }
  else {
    *left = newMiddle + (*right - *left == 1 ? 1 : 0);
    *lcpIndex = *lcpIndex * 2 + 1;
  }
}

void SuffixArray::moveM_M(int* oldMiddle, int* lcpIndex, int* left, int* right) {
  int newMiddle = (*left + *right - 1) / 2;
  if (newMiddle < *oldMiddle) {
    *left = newMiddle + (*right - *left == 1 ? 1 : 0);
    *lcpIndex = *lcpIndex * 2 + 1;
  }
  else {
    *right = newMiddle;
    *lcpIndex = *lcpIndex * 2;
  }

}

std::pair<int, int> SuffixArray::getMatchBinarySearch() {
  buildLcp_Lr(1, 1, this->window->getDictSize());
  int left = 1;
  int right = this->window->getDictSize();
  int lcp = 0, len = 0, lcpIndex = 1;
  Suffix best = this->suffixes[0];
  while (left <= right) {
    // Get the current suffix to observe
    int middle = (left + right) / 2;
    Suffix current = this->suffixes[middle];
    // Compare as many characters as possible, updating best
    if (len == lcp) {
      while (len < this->window->getLabSize() &&
        this->window->getFromLab(len) == current[len % current.length()]) {
        ++len;
        best = current;
      }
    }
    // If we found a perfect match, stop.
    if (len == this->window->getLabSize() || left == right) {
      break;
    }
    // If P is greater than M, move right.
    if (len == current.length() || this->window->getFromLab(len) > current[len % current.length()]) {
      moveRight(&lcpIndex, &middle, &left);
    }
    // Else, move left
    else {
      moveLeft(&lcpIndex, &middle, &right);
    }

    // BEGIN CASES
    while (left < right && len != (lcp = this->lcp_lr[lcpIndex])) {
      // Case One: want to move between M' and either L or R
      if (len < lcp) {
        moveM_end(&middle, &lcpIndex, &left, &right);
      }
      // Case Two: want to move between M and M'
      else if (len > lcp) {
        moveM_M(&middle, &lcpIndex, &left, &right);
      }
      lcp = this->lcp_lr[lcpIndex];
    }
    if (left == right) {
      lcp = this->window->getDictSize() == left ? this->lcp_lr[lcpIndex] : this->suffixes[left + 1].lcp;
    }
  }
  return std::pair<int, int>{len, this->window->getDictSize() - best.getIndex()};
}

std::pair<int, int> SuffixArray::getMatchBS() {
  Suffix out = this->suffixes[0];
  int len = 0;
  int left = 1, right = this->window->getDictSize();
  while (left <= right) {
    int mid = (left + right) / 2;
    Suffix middle = this->suffixes[mid];

    int k = 0;
    while (k < this->window->getLabSize() &&
      this->window->getFromLab(k) == middle[k % middle.length()]) {
      ++k;
    }
    if (k > len) {
      out = middle;
      len = k;
    }

    if (left == right || len == this->window->getLabSize()) {
      break;
    }

    if (k == middle.length() || this->window->getFromLab(k) > middle[k % middle.length()]) {
      left = mid + 1;
    }
    else {
      right = mid;
    }

  }
  return { len, this->window->getDictSize() - out.index };
}


std::pair<int, int> SuffixArray::getMatch() {
  int offset = 0;
  int len = 0;
  for (int i = 1; i <= this->window->getDictSize(); i++) {
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
  for (int i = 1; i <= this->window->getDictSize(); i++) {
    std::cout << this->suffixes[i] << std::endl;
  }
}

void SuffixArray::show(int pos) const {
  for (int i = 0; i <= this->window->getDictSize(); i++) {
    std::cout << std::setfill('0') << std::setw(2) << suffixes[i].index << " ";
  }
  std::cout << std::endl;
  if (pos >= 0) {
    for (int i = 0; i <= this->window->getDictSize(); i++) {
      std::cout << (i == pos ? "^^" : "  ") << " ";
    }
    std::cout << std::endl;
  }
}