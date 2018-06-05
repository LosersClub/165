#include "hash_encoder.h"

HashEncoder::HashEncoder(Window* window) : window(window), maxMatch(false) {
  if (this->window->getLabSize() > 0) {
    char* dictEnd = this->window->getFromDictPtr(0);
    int key = hash(dictEnd);
    this->map[key].push_back(dictEnd);
  }
}

std::pair<int, int> HashEncoder::getMatch() {
  // Only one character left in LAB. No possible match and no
  // entry to add.
  if (this->window->getLabSize() < 2) {
    modifyTable(nullptr, 0);
    return std::pair<int, int>{0, 0};
  }

  int key = hash(this->window->getFromLabPtr(0));
  // The bucket is empty, no match
  if (this->map[key].empty()) {
    modifyTable(this->window->getFromLabPtr(0), 1);
    return std::pair<int, int>{0, 0};
  }

  //auto iterator = this->map[key].begin();
  int bestMatchLen = 0;
  char* bestMatch = nullptr;
  for (char*& bucketEntry : this->map[key]) {
    int matchLen = 0;
    char* temp = bucketEntry;

    // Compare the current string to the LAB
    while (matchLen < this->window->getLabSize()) {
      if (*temp != this->window->getFromLab(matchLen)) {
        break;
      }
      ++matchLen;
      // Move to the next character in the char sequence
      temp = this->window->atEndOfDict(temp) ? bucketEntry : this->window->getNextInDict(temp);
    }

    // We've found a better match, so update our trackers
    if (matchLen > bestMatchLen) {
      bestMatchLen = matchLen;
      bestMatch = bucketEntry;
    }
    if (bestMatchLen == this->window->getLabSize()) {
      break;
    }
  }

  modifyTable(this->window->getFromLabPtr(0), bestMatchLen > 0 ? bestMatchLen : 1);
  return std::pair<int, int> {bestMatchLen, this->window->getOffset(bestMatch)};
}

void HashEncoder::modifyTable(char* string, int len) {
  this->removeString(this->window->getFromDictPtr(0), len > 0 ? len : 1);
  this->addString(string, len);
}

int HashEncoder::hash(char* string) {
  unsigned int key = 0;
  char* temp = string;
  for (int i = 0; i < MIN_MATCH_LENGTH; i++, temp = this->window->getNext(temp)) {
    key = ((key << 5) ^ *temp) % HASH_SIZE;
  }
  return key;
}

void HashEncoder::addString(char* string, int size) {
  char* temp = string;
  if (size == this->window->getLabSize()) {
    this->maxMatch = true;
    size--;
  }
  for (int i = 0; i < size; i++, temp = this->window->getNext(temp)) {
    int key = hash(temp);
    this->map[key].push_back(temp);
  }
}

void HashEncoder::removeString(char* string, int size) {
  char* temp = string;
  while (this->window->getDictSize() + size > this->window->getDictCap()) {
    int key = hash(temp);
    this->map[key].remove(temp);
    temp = this->window->getNextInDict(temp);
    size--;
  }
}

void HashEncoder::repair() {
  if (this->maxMatch && this->window->getLabSize() > 0) {
    char* dictEnd = this->window->getFromDictPtr(this->window->getDictSize() - 1);
    int key = hash(dictEnd);
    this->map[key].push_back(dictEnd);
  }
  this->maxMatch = false;
}