#pragma once
#include <vector>
#include <climits>

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
    unsigned char writeHeader();
    void writeDouble(int len, int offset);
    void writeTriple(int size, std::vector<char> bytes);
    void writeEOF();
    void reset();
  private:
    Mask nMask, lMask, sMask;
    unsigned char buffer;
    unsigned char pos;

    void write(int value, Mask mask);
};

class BitStreamReader : public BitStream {
  public:
    BitStreamReader(unsigned char header);
    ~BitStreamReader();

    void readHeader(const unsigned char& header);
    Type read(unsigned char input);

    std::pair<int, int> getDouble();
    std::pair<int, std::vector<char>> getTriple();
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