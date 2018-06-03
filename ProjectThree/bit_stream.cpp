#include "bit_stream.h"

#include <bitset>
#include <iostream>

BitStreamWriter::BitStreamWriter(unsigned char n, unsigned char l, unsigned char s)
  : buffer(0), pos(8), nMask({n}), lMask({l}), sMask({s}) { }

BitStreamReader::BitStreamReader(unsigned char header)
  : nMask(nullptr), lMask(nullptr), sMask(nullptr) {
  this->readHeader(header);
}

BitStreamReader::~BitStreamReader() {
  delete nMask;
  delete lMask;
  delete sMask;
}

void BitStreamWriter::reset() {
  std::cout << this->buffer;
  this->pos = 8;
  this->buffer = 0x0;
}

void BitStreamWriter::writeDouble(int len, int offset) {
  this->write(len - 1, this->lMask);
  this->write(offset - 1, this->nMask);
}

void BitStreamWriter::writeTriple(int size, std::vector<char> bytes) {
  this->write(0x00, this->lMask);
  this->write(size, this->sMask);
  for (int i = 0; i < size; i++) {
    this->write(bytes[i], BitStream::bitMask);
  }
}

void BitStreamWriter::writeEOF() {
  this->write(0x00, this->lMask);
  this->write(0x00, this->sMask);
  if (this->pos != 8) {
    this->reset();
  }
}

void BitStreamWriter::write(int value, Mask mask) {
  unsigned short temp = value & mask.mask;
  int size = mask.size;
  while (size > 0) {
    short tempPos = this->pos - size;
    if (tempPos < 0) {
      this->buffer |= temp >> (tempPos * -1);
      size -= this->pos;
      this->pos = 0;
    }
    else {
      this->buffer |= temp << (this->pos -= size);
      size = 0;
    }
    if (this->pos == 0) {
      this->reset();
    }
  }
}

BitStream::Type BitStreamReader::read(unsigned char input) {
  unsigned int buffer = (overflow << 8) | input;
  overflow = buffer & 0xFFFF;
  bool flag = true;
  if (!lengthSet) {
    this->length = this->read(buffer, *this->lMask, flag);
    if (!flag) {
      return Type::None;
    }
    lengthSet = true;
  }
  if (length == 0) {
    // Triple Token
    if (!sizeSet) {
      this->size = this->read(buffer, *this->sMask, flag);
      if (!flag) {
        return Type::None;
      }
      sizeSet = true;
    }
    if (size == 0) {
      return Type::End;
    }
    while (size > 0) {
      char temp = this->read(buffer, BitStream::bitMask, flag);
      if (!flag) {
        return Type::None;
      }
      bytes.push_back(temp);
      size -= 1;
    }
    lengthSet = false;
    sizeSet = false;
    backup = bytes;
    bytes.clear();
    this->pos += 8;
    return Type::Triple;
  }

  this->offset = this->read(buffer, *this->nMask, flag);
  if (!flag) {
    return Type::None;
  }
  this->length++, this->offset++;
  lengthSet = false;
  this->pos += 8;
  return Type::Double; 
}

short BitStreamReader::read(unsigned int& buffer, Mask mask, bool& flag) {
  if (mask.size > this->pos) {
    this->pos += 8;
    flag = false;
    return -1;
  }
  this->pos -= mask.size;
  return (buffer & (mask.mask << this->pos)) >> this->pos;
}

std::pair<int, int> BitStreamReader::getDouble() {
  return { this->length, this->offset };
}

std::pair<int, std::vector<char>> BitStreamReader::getTriple() {
  return { this->size, this->backup };
}

unsigned char BitStreamWriter::writeHeader() {
  int n = nMask.size - 9;
  int l = lMask.size - 3;
  int s = sMask.size - 1;
  return ((n & 0x7) << 5) | ((l & 0x1) << 4) | ((s & 0x7) << 1);
}

void BitStreamReader::readHeader(const unsigned char& header) {
  this->nMask = new Mask(((header & (0x7 << 5)) >> 5) + 9);
  this->lMask = new Mask(((header & (0x1 << 4)) >> 4) + 3);
  this->sMask = new Mask(((header & (0x7 << 1)) >> 1) + 1);
}

int BitStreamReader::getN() {
  return 2 << this->nMask->size;
}