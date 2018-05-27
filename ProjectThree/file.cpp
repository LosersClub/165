#include "file.h"

File::File(std::string path) {
  if (this->path == "") {
    throw std::invalid_argument("No path specified.");
  }
  this->stream.open(this->path.c_str(), std::fstream::in);
  if (!this->stream.is_open()) {
    throw std::invalid_argument("Path not found: " + this->path);
  }
  this->isOpen = true;
}

void File::close() {
  this->stream.close();
  this->isOpen = false;
}

bool File::isFileOpen() const {
  return this->isOpen;
}

char File::readChar() {
  if (!this->isOpen) {
    throw std::logic_error("The file is not open and cannot be read.");
  }
  if (this->stream.peek() == EOF) {
    return NULL;
  }
  return (char)(this->stream.get());
}

std::string File::getPath() const {
  return this->path;
}