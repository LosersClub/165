#include "file.h"
#include "bit_stream.h"
#include "window.h"

#include <iostream>
#include <bitset>

void printTriple(std::vector<char>& triple) {
  std::cout << "(0," << triple.size() << ",";
  for (char& c : triple) {
    std::cout << c;
  }
  std::cout << ")" << std::endl;
}

void printDouble(std::pair<int, int>& token) {
  std::cout << "(" << token.first << "," << token.second << ")" << std::endl;
}


int main(int argc, char** argv) {
  std::istream* stream = &std::cin;
  File* file = nullptr;
  if (argc > 2) {
    std::cerr << "One parameter allowed: path to file to be decompressed." << std::endl;
    return 1;
  }

  if (argc == 2) {
    try {
      file = new File(argv[1]);
    }
    catch (const std::exception& e) {
      std::cerr << e.what() << std::endl;
      return 1;
    }
    stream = &(file->stream);
  }
  unsigned char byte = stream->get();

  if (stream->eof()) {
    std::cerr << "No input received." << std::endl;
    return 1;
  }
  BitStreamReader reader = BitStreamReader(byte);

  if (reader.getN() < 512 || reader.getN() > 16384 ||
    reader.getS() - 1 < 1 || reader.getS() - 1 > 31) {
    std::cerr << "Read arguments out of range: invalid input file." << std::endl;
    return 1;
  }

  Window window = Window(reader.getN());

  bool done = false;
  while (!done && stream->good()) {
    switch (reader.read(stream->get())) {
    case BitStream::Triple: {
      std::pair<int, std::vector<char>> triple = reader.getTriple();
      printTriple(triple.second);
      for (char& c : reader.getTriple().second) {
        //std::cout << c;
        window.addNoLab(c);
      }
      break;
    }
    case BitStream::Double: {
      std::pair<int, int> info = reader.getDouble();
      printDouble(info);
      int dictIndex = window.getDictSize() - info.second;
      for (int i = 0; i < info.first; i++) {
        char c = window.getFromDict(dictIndex + i);
        //std::cout << c;
        window.addNoLab(c);
      }
      break;
    }
    case BitStream::None:
      continue;
    case BitStream::End:
      done = true;
      break;
    }
  }
  system("PAUSE");
  return 0;
}