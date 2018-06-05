#include "hash_encoder.h"


HashEncoder::HashEncoder(Window* window) : window(window), maxMatch(false){
  if (this->window->getLabSize() > 0) {
    char* dictEnd = this->window->getFromDictPtr(0);
    int key = hash(dictEnd);
    this->map[key].push_back(dictEnd);
  }
}

std::pair<int, int> HashEncoder::getMatch() {
  if (this->window->getLabSize() < 2) {
    modifyTable(nullptr, 0);
    return std::pair<int, int>{0, 0};
  }
  char* labString = this->window->getFromLabPtr(0);
  int key = hash(labString);
  // The bucket is empty, no match
  if (this->map[key].empty()) {
    modifyTable(this->window->getFromLabPtr(0), 1);
    return std::pair<int, int>{0, 0};
  }
  auto iterator = this->map[key].begin();
  int bestMatchLen = 0;
  char* bestMatch = 0;
  char* bucketEntry;
  while (iterator != this->map[key].end()) {
    // Loop through the strings in the bucket
    int matchLen = 0;
    bucketEntry = *iterator;
    char* temp = bucketEntry;

    // Compare the current string to the LAB
    while (matchLen < this->window->getLabSize()) {
      if (*temp != this->window->getFromLab(matchLen)) {
        break;
      }
      matchLen++;
      // Move to the next character in the char sequence
      if (this->window->atEndOfDict(temp)) {
        temp = bucketEntry;
      }
      else {
        temp = this->window->getNextInDict(temp);
      }

    }
    // We've found a better match, so update our trackers
    if (matchLen > bestMatchLen && matchLen > 1) {
      bestMatchLen = matchLen;
      bestMatch = bucketEntry;
    }

    if (bestMatchLen == this->window->getLabSize()) {
      break;
    }
    iterator++;
  }
  modifyTable(this->window->getFromLabPtr(0), bestMatchLen);
  
  return std::pair<int, int> {bestMatchLen, this->window->getOffset(bestMatch)};
}

void HashEncoder::modifyTable(char* string, int len) {
  this->removeString(this->window->getFromDictPtr(0), len > 0 ? len : 1);
  this->addString(string, len);
}

int HashEncoder::hash(char* string) {
  unsigned int key = 0;
  // note: got rid of +1 on for condition; would cause indexing issues
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
  //min size = 1
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