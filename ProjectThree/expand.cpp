#include "file.h"
#include "bit_stream.h"
#include "window.h"

#include <cstdlib>
#include <string>
#include <iostream>
#include <stdexcept>
#include <bitset>

int main(int argc, char** argv) {
  std::istream* stream = &std::cin;
  File* file = nullptr; 
  if (argc > 2) {
    std::cerr << "One parameter allowed: path to file to be decompressed." << std::endl;
    return 0;
  }

  if (argc == 2) {
    try {
      file = new File(argv[1]);
    }
    catch (const std::exception& e) {
      std::cerr << e.what();
      return 0;
    }
    stream = &(file->stream);
  }
  unsigned char byte = stream->get();
  if (byte == NULL) {
    std::cerr << "No input received." << std::endl;
    return 0;
  }
  BitStreamReader reader = BitStreamReader(byte);
  Window window = Window(reader.getN());

  while (stream->peek() != EOF) {
    byte = stream->get();
    BitStream::Type type = reader.read(byte);
    while (type == BitStream::Type::None) {
      type = reader.read(stream->get());
    }
    if (type == BitStream::Type::Double) {
      std::pair<int, int> info = reader.getDouble();
      int dictIndex = window.getDictSize() - info.second;
      for (int i = 0; i < info.first; i++) {
        char c = window.getFromDict(dictIndex + i);
        std::cout << c;
        window.addNoLab(c);
      }
    }
    else if (type == BitStream::Type::Triple) {
      std::vector<char> chars = reader.getTriple().second;
      for (char &c : chars) {
        std::cout << c;
        window.addNoLab(c);
      }
    }
    else {
      break;
    }
  }
  if (file != nullptr) {
    file->close();
  }
   
  return 1;
}