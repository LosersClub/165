#include "window.h"
#include <stdexcept>
#include <algorithm>
#include <iostream>

// initChars contains at index 0 the first char in the file, which should have been handled
// and which belongs to the dict. The rest of the characters will fill the LAB.
Window::Window(int windowCap, int labCap, const char* initChars, int initCharsSize) {
  this->windowCap = windowCap;
  this->labCap = labCap;
  this->dictCap = windowCap - labCap;
  this->head = 0;
  this->tail = 0;
  array = new char[windowCap];
  if (initCharsSize > labCap + 1) {
    throw new std::invalid_argument("Input char array was too big!");
  }
  for (int i = 0; i < initCharsSize; i++) {
    this->array[i] = initChars[i];
  }
  this->split = 1;
  this->windowSize = initCharsSize;
  this->dictSize = 1;
  this->labSize = initCharsSize - 1;
  this->tail = initCharsSize;
}

Window::Window(int windowCap) {
  this->windowCap = windowCap;
  this->labCap = 0;
  this->dictCap = windowCap;
  this->head = 0;
  this->tail = 0;
  array = new char[windowCap];
  this->split = 1;
  this->windowSize = 0;
  this->dictSize = 0;
  this->labSize = 0;
}

int Window::getSize() const {
  return this->windowSize;
}

int Window::getDictSize() const {
  return this->dictSize;
}

int Window::getLabSize()  const{
  return this->labSize;
}

int Window::getSplitIndex() const {
  return this->split;
}

int Window::getDictCap() const {
  return this->dictCap;
}

int Window::getHead() const {
  return this->head;
}

int Window::getTail() const {
  return this->tail;
}

const char& Window::getFromDict(int index) const {
  index = (index + this->head) % this->windowCap;
  //if (this->split == this->head) {
  //  throw new std::invalid_argument("Dictionary is empty.");
  //}
  //if (index < 0 || index >= windowCap) {
  //  throw new std::invalid_argument("Index our of bounds for dictionary.");
  //}
  //verify these bounds are right
  //if ((index < this->head && index >= this->split) || (this->head < this->split && index > this->head && index >= this->split)) {
  //  throw new std::invalid_argument("Index out of bounds for dictionary.");
  //}
  return this->array[index];
}

const char& Window::getFromLab(int index) const {
  index = (index + this->split) % this->windowCap;
  if (index < 0 || index >= windowCap) {
    throw new std::invalid_argument("Index our of bounds for look-ahead buffer.");
  }
  //verify these bounds are right
  if ((index < this->split && index >= this->tail) || (this->split < this->tail && index > this->split && index >= this->tail)) {
    throw new std::invalid_argument("Index out of bounds for look-ahead buffer.");
  }
  return this->array[index];
}


char* Window::getFromDictPtr(int index) {
  index = (index + this->head) % this->windowCap;
  return this->array + index;
}
char* Window::getFromLabPtr(int index) {
  index = (index + this->split) % this->windowCap;
  return this->array + index;
}

void Window::shift(int numSpots) {
  // safety check?
  if (labSize <= 0) {
    throw new std::invalid_argument("Tried to exceed the end of the input string!");
  }
  if (this->dictSize + numSpots > this->dictCap) {
    this->head = ((this->head + numSpots) % this->windowCap);
  }
  this->split = ((this->split + numSpots) % this->windowCap);
  this->labSize -= numSpots;
  this->dictSize = std::min(dictCap, dictSize + numSpots);
  this->windowSize = labSize + dictSize;
  //TODO: modify other sizes
}

void Window::add(const char* chars, int charsSize) {
  for (int i = 0; i < charsSize; i++) {
    add(chars[i]);
  }
}

void Window::add(char c) {
  if (this->tail >= windowCap) {
    this->tail = 0;
  }
  this->array[this->tail] = c;
  this->windowSize++;
  this->labSize++;
  this->tail++;
  this->shift(1);
}

void Window::addNoLab(char c) {
  if (this->tail >= windowCap) {
    this->tail = 0;
  }
  this->array[this->tail] = c;
  this->windowSize++;
  this->tail++;
  if (this->dictSize + 1 > this->dictCap) {
    this->head = ((this->head + 1) % this->windowCap);
  }
  this->split = ((this->split + 1) % this->windowCap);
  this->dictSize = std::min(dictCap, dictSize + 1);
  this->windowSize = dictSize;
}

void Window::print() {

  for (int i = 0; i < this->getDictSize(); i++) {
    std::cout << this->getFromDict(i) << " ";
  }
  std::cout << "| ";
  for (int i = 0; i < this->getLabSize(); i++) {
    std::cout << this->getFromLab(i) << " ";
  }
  std::cout << std::endl;
}

char* Window::getNextInDict(char* c) {
  char* temp = c;
  if (++temp >= this->array + this->windowSize) {
    temp = this->array;
  }
  if ((this->head < this->split &&
        (temp >= (this->array + this->head) && temp < (this->array + this->split))) ||
    (this->head > this->split &&
        (temp >= (this->array + this->head) || temp < (this->array + this->split)))) {
    return temp;
  }
  return nullptr;
}

char* Window::getNext(char* c) {
  char* temp = c;
  if (++temp >= this->array + this->windowSize) {
    temp = this->array;
  }
  return temp;
}

bool Window::atEndOfDict(char* c) {
  if (this->array + this->split == this->array) {
    return c == this->array + this->windowSize - 1;
  }
  return c == this->array + this->split - 1;
}

int Window::getOffset(char* c) {
  char* splitLoc = this->array + this->split;
  if (splitLoc > c) {
    return splitLoc - c;
  }

  else {
    return ((this->array + this->windowSize) - c) + this->split + (head - tail);
  }
}

Window::~Window() {
  delete array;
}

int Window::getLabCap() const {
  return this->labCap;
}