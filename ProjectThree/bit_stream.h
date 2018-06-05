#pragma once
#include <vector>

class BitStream {
  public:
    enum Type { None, Double, Triple, End };
  protected:
    struct Mask {
      unsigned char size;
      unsigned short mask;
      Mask(unsigned short m, unsigned char s) : size(s), mask(m) {}
      Mask(unsigned char s) : Mask((1 << s) - 1, s) {}
    };
    Mask bitMask = { 0xFF, 8 };
};

class BitStreamWriter : public BitStream {
  public:
    BitStreamWriter(unsigned char n, unsigned char l, unsigned char s);

    void writeHeader();
    void writeDouble(int len, int offset);
    void writeTriple(std::vector<char> bytes);
    void writeEOF();
    long size();
  private:
    Mask nMask, lMask, sMask;
    unsigned char buffer;
    unsigned char pos;
    long byteCount;

    void write(int value, Mask mask);
    void reset();
};

class BitStreamReader : public BitStream {
  public:
    BitStreamReader(unsigned char header[3]);
    ~BitStreamReader();

    void readHeader(const unsigned char header[3]);
    Type read(unsigned char input);

    std::pair<int, int> getDouble();
    std::vector<char> getTriple();

    unsigned int getMaxN();
    int getN();
    int getL();
    int getS();

  private:
    Mask *nMask, *lMask, *sMask;
    unsigned short overflow = 0x00;
    unsigned char pos = 8;

    bool lengthSet = false;
    int length = 0;
    bool sizeSet = false;
    int size = 0;
    int offset = 0;
    std::vector<char> bytes;
    std::vector<char> backup;

    short read(unsigned int& buffer , Mask mask, bool& flag);
};