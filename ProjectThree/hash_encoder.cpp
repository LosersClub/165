#include "hash_encoder.h"


HashEncoder::HashEncoder(Window* window) {
  this->window = window;
}

std::pair<int, std::forward_list> HashEncoder::getMatch() {
  if (this->window->getLabSize() < 2) {
    return std::pair<int, int>{0, 0};
  }
  char* labString = { this->window->getFromLab(0), this->window->getFromLab(1) };
  int key = hash(labString);
  if (this->map.count(key) == 0) {
    return std::pair<int, int>{0, 0};
  }
  auto iterator = this->map[key].begin();
  int bestMatchLen = 0;
  int bestOffset = 0;
  char* dictChar;
  while (iterator != this->map[key].end()) {

    int matchLen = 0;
    dictChar = *iterator;
    char* temp = dictChar;
    while (matchLen < this->window->getLabSize()) {
      if (*temp != this->window->getFromLab(matchLen)) {
        break;
      }
      matchLen++;
      if (matchLen > bestMatchLen && matchLen > 1) {
        bestMatchLen = matchLen;
        bestDictIndex = dictIndex;
      }
      if (this->window->isAtEndOfDict(temp) {
        temp = dictChar;
      }
      else {
        temp = this->window->getNextInDict(temp);
      }
    }

    if (bestMatchLen == this->window->getLabSize()) {
      // TODO: Remove ALL instances of dictChar, add the new LAB string (will need 
      // to modify addString function to split the string into 2s), and properly calculate 
      // offset
      this->removeString(dictChar);
      return std::pair<int, int> {bestMatchLen, this->window->getDictSize() - bestDictIndex};
    }
    iterator++;
  }
  // Same TODO as above
  this->removeString(dictChar);
  return std::pair<int, int> {bestMatchLen, this->window->getDictSize() - bestDictIndex};
}

int HashEncoder::hash(char* string) {
  int key = 0;
  // note: got rid of +1 on for condition; would cause indexing issues
  for (int i = 0; i < MIN_MATCH_LENGTH; i++) {
    key = ((key << 5) ^ stringStart[i]) % HASH_SIZE;
  }
  return key;
}

void HashEncoder::addString(char* string) {
  int key = hash(string);
  this->map[key].push_front(string);
}

void HashEncoder::removeString(char* string) {
  int key = hash(string);
  this->map[key].remove(string);
}