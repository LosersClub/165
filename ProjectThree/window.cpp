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
  if (this->split == this->head) {
    throw new std::invalid_argument("Dictionary is empty.");
  }
  if (index < 0 || index >= windowCap) {
    throw new std::invalid_argument("Index our of bounds for dictionary.");
  }
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

void Window::print() {
  int count = 0;
  int i = this->head;
  do {
    if (i == this->split) {
      std::cout << "| ";
    }
    if ((i < this->head && i >= this->tail) || (this->head < this->tail && i > this->head && i >= this->tail)) {
      std::cout << "- ";
    }
    else {
      std::cout << this->array[i] << " ";
    }
    i++;
    if (i == this->windowCap) {
      i = 0;
    }
  } while (i != this->head);
}

Window::~Window() {
  delete array;
}