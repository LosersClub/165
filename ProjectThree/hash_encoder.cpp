#include "hash_encoder.h"

/*
** The HashEncoder implements a hash table that is used to
** quickly find a match between the look-ahead buffer and a
** string in the dictionary.
*/

HashEncoder::HashEncoder(Window* window) : window(window), maxMatch(true) {
  this->repair();
}

// Gets the best match for the contents of the look-ahead buffer (LAB)
std::pair<int, int> HashEncoder::getMatch() {
  // Only one character left in LAB. No possible match and no
  // entry to add.
  if (this->window->getLabSize() < 2) {
    modifyTable(nullptr, 0);
    return std::pair<int, int>{0, 0};
  }

  int key = hash(this->window->getFromLab(0));
  // The relevant bucket is empty; no match exists
  if (this->map[key].empty()) {
    modifyTable(this->window->getFromLab(0), 1);
    return std::pair<int, int>{0, 0};
  }
  // Stores our best match so far
  int bestMatchLen = 0;
  char* bestMatch = nullptr;
  // Loop through the bucket entries
  for (char*& bucketEntry : this->map[key]) {
    int matchLen = 0;
    char* temp = bucketEntry;

    // Compare the current bucket entry to the LAB
    while (matchLen < this->window->getLabSize()) {
      if (*temp != *this->window->getFromLab(matchLen)) {
        break;
      }
      ++matchLen;
      // Move to the next character in the current entry's char sequence
      temp = this->window->atEndOfDict(temp) ? bucketEntry : this->window->getNext(temp);
    }

    // We've found a better match, so update our trackers
    if (matchLen > bestMatchLen) {
      bestMatchLen = matchLen;
      bestMatch = bucketEntry;
    }
    // We've found a maximum-length match, so quit early
    if (bestMatchLen == this->window->getLabSize()) {
      break;
    }
  }
  // Update our table according to the match we found, so as to stay consistent
  // with the Window
  modifyTable(this->window->getFromLab(0), bestMatchLen > 0 ? bestMatchLen : 1);
  return std::pair<int, int> {bestMatchLen, this->window->getOffset(bestMatch)};
}

void HashEncoder::modifyTable(char* string, int len) {
  this->removeString(this->window->getFromDict(0), len > 0 ? len : 1);
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
  if (size == this->window->getLabSize()) {
    this->maxMatch = true;
    size--;
  }
  for (int i = 0; i < size; i++, string = this->window->getNext(string)) {
    this->map[hash(string)].push_back(string);
  }
}

void HashEncoder::removeString(char* string, int size) {
  while (this->window->getDictSize() + size > this->window->getDictCap()) {
    this->map[hash(string)].remove(string);
    string = this->window->getNext(string);
    size--;
  }
}

void HashEncoder::repair() {
  if (this->maxMatch && this->window->getLabSize() > 0) {
    char* dictEnd = this->window->getFromDict(this->window->getDictSize() - 1);
    this->map[hash(dictEnd)].push_back(dictEnd);
  }
  this->maxMatch = false;
}