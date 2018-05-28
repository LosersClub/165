#include "suffix_array.h"
#include <algorithm>
#include <iomanip>

SuffixArray::SuffixArray(const Window* window) : window(window) {
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
    std::cout << s << std::endl;
  }
}

void SuffixArray::buildLcp_Lr(int index, int left, int right) {
  if (left == right) {
    this->lcp_lr[index] = this->suffixes[left].lcp;
    return;
  }
  int middle = (left + right) / 2;

  buildLcp_Lr(2 * index, left, middle);
  buildLcp_Lr(2 * index + 1, middle + 1, right);

  this->lcp_lr[index] = std::min(this->lcp_lr[2 * index], this->lcp_lr[2 * index + 1]);
}
//https://stackoverflow.com/questions/11373453/how-does-lcp-help-in-finding-the-number-of-occurrences-of-a-pattern/11374737#11374737
std::pair<int, int> SuffixArray::getMatchBinarySearch() {
  int offset = 0;
  int len = 0;
  this->buildLcp_Lr(1, 1, this->suffixes.size() - 1);
  int left = 0;
  int right = this->suffixes.size() - 1;
  while (left <= right) {
    int middle = (left + right) / 2;
    Suffix current = this->suffixes[middle];
    int k = 0;
    while (k < current.length() && k < this->window->getLabSize()) {
      if (!(current[k] == this->window->getFromLab[k])) {
        break;
      }
      k++;
    }
    if (k == this->window->getLabSize()) {
      offset = this->window->getDictSize() - current.getIndex();
      return std::pair<int, int>{k, offset};
    }
    if (this->window->getFromLab(k) > current[k]) {
      //left = middle;
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
    os << obj[i];
  }
  return os;
}