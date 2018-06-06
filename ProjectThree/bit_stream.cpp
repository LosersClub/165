#include "bit_stream.h"

#include <iostream>

/*
** The BitStreamWriter class handles encoding match tokens into bytes, and
** the BitStreamReader class handles decoding bytes into match tokens.
*/

BitStreamWriter::BitStreamWriter(unsigned char n, unsigned char l, unsigned char s)
  : nMask({n}), lMask({l}), sMask({s}), buffer(0), pos(8), byteCount(0) { }

BitStreamReader::BitStreamReader(unsigned char header[3])
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
  this->byteCount++;
  this->pos = 8;
  this->buffer = 0x0;
}

long BitStreamWriter::size() {
  return this->byteCount;
}

void BitStreamWriter::writeDouble(int len, int offset) {
  this->write(len - 1, this->lMask);
  this->write(offset - 1, this->nMask);
}

void BitStreamWriter::writeTriple(std::vector<char> bytes) {
  this->write(0x00, this->lMask);
  this->write(bytes.size(), this->sMask);
  for (char& byte : bytes) {
    this->write(byte, BitStream::bitMask);
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
    } else {
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
  // Double Token
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

std::vector<char> BitStreamReader::getTriple() {
  return this->backup;
}

void BitStreamWriter::writeHeader() {
  std::putchar(nMask.size & 0xF);
  std::putchar(lMask.size & 0x7);
  std::putchar(sMask.size & 0xF);
  this->byteCount += 3;
}

void BitStreamReader::readHeader(const unsigned char header[3]) {
  this->nMask = new Mask(header[0] & 0xF);
  this->lMask = new Mask(header[1] & 0x7);
  this->sMask = new Mask(header[2] & 0xF);
}

unsigned int BitStreamReader::getMaxN() {
  return 2 << (this->nMask->size - 1);
}

int BitStreamReader::getN() {
  return this->nMask->size;
}

int BitStreamReader::getL() {
  return this->lMask->size;
}

int BitStreamReader::getS() {
  return this->sMask->size;
}