#include "file.h"

#include <sys/stat.h>
#include <iostream>
#include <stdexcept>

/*
** File is a utility class that makes reading from a file easier.
*/

File::File(std::string path) {
  this->isOpen = false;
  this->path = path;
  if (path == "") {
    throw std::invalid_argument("No path specified.");
  }
  this->open();
}

void File::open() {
  if (!this->isOpen) {
    this->stream.open(this->path.c_str(), std::fstream::in | std::fstream::binary);
    if (!this->stream.is_open()) {
      throw std::invalid_argument("File path not found: " + this->path);
    }
    this->isOpen = true;
  }
}

int File::size() {
  struct stat stats;
  stat(this->path.c_str(), &stats);
  return stats.st_size;
}

void File::close() {
  this->stream.close();
  this->isOpen = false;
}

bool File::isFileOpen() const {
  return this->isOpen;
}

char File::readChar() {
  return (char)(this->stream.get());
}

bool File::hasNextChar() {
  return this->stream.peek() != EOF;
}