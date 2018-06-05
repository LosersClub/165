#include "window.h"

#include <algorithm>
#include <iostream>

// initChars contains at index 0 the first char in the file, which should have been handled
// and which belongs to the dict. The rest of the characters will fill the LAB.
Window::Window(int windowCap, int labCap, std::string initChars)  {
  this->windowCap = windowCap;
  this->dictCap = windowCap - labCap;
  this->head = 0;
  this->tail = 0;
  array = new char[windowCap];

  for (int i = 0; i < initChars.length(); i++) {
    this->array[i] = initChars[i];
  }
  this->split = 1;
  this->windowSize = initChars.length();
  this->dictSize = 1;
  this->labSize = initChars.length() - 1;
  this->tail = initChars.length();
}

Window::Window(int windowCap) {
  this->windowCap = windowCap;
  this->dictCap = windowCap;
  this->head = 0;
  this->tail = 0;
  array = new char[windowCap];

  this->split = 1;
  this->windowSize = 0;

  this->dictSize = 0;
  this->labSize = 0;
}

Window::~Window() {
  delete array;
}

int Window::getDictSize() const {
  return this->dictSize;
}

int Window::getLabSize()  const{
  return this->labSize;
}

int Window::getDictCap() const {
  return this->dictCap;
}

char* Window::getFromDict(int index) {
  return this->array + ((index + this->head) % this->windowCap);
}
char* Window::getFromLab(int index) {
  return this->array + ((index + this->split) % this->windowCap);
}

char* Window::getNext(char* c) {
  char* temp = c;
  if (++temp >= this->array + this->windowSize) {
    temp = this->array;
  }
  return temp;
}

bool Window::atEndOfDict(char*& c) const {
  return c == this->array + (this->split == 0 ? this->windowSize : this->split) - 1;
}

int Window::getOffset(char*& c) const {
  char* splitLoc = this->array + this->split;
  if (splitLoc > c) {
    return splitLoc - c;
  }
  else {
    return ((this->array + this->windowSize) - c) + this->split + (this->head - this->tail);
  }
}

void Window::add(char c) {
  if (this->tail >= windowCap) {
    this->tail = 0;
  }

  this->array[this->tail++] = c;
  this->labSize++;
  this->shift(1);
}

void Window::addNoLab(char c) {
  if (this->tail >= windowCap) {
    this->tail = 0;
  }
  this->array[this->tail++] = c;

  if (this->dictSize + 1 > this->dictCap) {
    this->head = ((this->head + 1) % this->windowCap);
  }
  this->split = ((this->split + 1) % this->windowCap);
  this->dictSize = std::min(dictCap, dictSize + 1);
  this->windowSize = dictSize;
}

void Window::shift(int numSpots) {
  if (this->dictSize + numSpots > this->dictCap) {
    this->head = ((this->head + numSpots) % this->windowCap);
  }

  this->split = ((this->split + numSpots) % this->windowCap);
  this->labSize -= numSpots;
  this->dictSize = std::min(dictCap, dictSize + numSpots);
  this->windowSize = labSize + dictSize;
}