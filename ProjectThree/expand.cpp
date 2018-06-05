#include "file.h"
#include "bit_stream.h"
#include "window.h"

#ifdef _WIN32
#include <io.h>
#include <fcntl.h>
#endif

#include <iomanip>
#include <iostream>
#include <chrono>

// DELETE
void printTriple(std::vector<char>& triple) {
  std::cout << "(0," << triple.size() << ",";
  for (char& c : triple) {
    std::cout << c;
  }
  std::cout << ")" << std::endl;
}

// DELETE
void printDouble(std::pair<int, int>& token) {
  std::cout << "(" << token.first << "," << token.second << ")" << std::endl;
}


int main(int argc, char** argv) {
#ifdef _WIN32
  _setmode(_fileno(stdin), _O_BINARY);
#endif

  std::istream* stream = &std::cin;
  File* file = nullptr;
  if (argc > 2) {
    std::cerr << "One parameter allowed: path to file to be decompressed." << std::endl;
    return 1;
  }

  if (argc == 2) {
    try {
      file = new File(argv[1]);
      stream = &(file->stream);
    }
    catch (const std::exception& e) {
      std::cerr << e.what() << std::endl;
      return 1;
    }
  }

  unsigned char byte = stream->get();

  if (stream->eof()) {
    std::cerr << "No input received." << std::endl;
    return 1;
  }
  BitStreamReader reader = BitStreamReader(byte);

  if (reader.getMaxN() < 512 || reader.getMaxN() > 16384 ||
    reader.getMaxS() - 1 < 1 || reader.getMaxS() - 1 > 31) {
    std::cerr << "Read arguments out of range: invalid input file." << std::endl;
    return 1;
  }

  std::cerr << "EXPAND INFO" << std::endl;
  std::cerr << "N: " << reader.getN() << ", L: " << reader.getL() << ", S: " << reader.getS() << std::endl;

  std::chrono::high_resolution_clock::time_point startTime = std::chrono::high_resolution_clock::now();
  Window window = Window(reader.getMaxN());

  long uncompressedSize = 0;
  long compressedSize = 1;
  bool done = false;
  while (!done && stream->good()) {
    compressedSize++;
    switch (reader.read(stream->get())) {
    case BitStream::Triple: {
      std::pair<int, std::vector<char>> triple = reader.getTriple();
      for (char& c : reader.getTriple().second) {
        std::cout << c;
        uncompressedSize++;
        window.addNoLab(c);
      }
      break;
    }
    case BitStream::Double: {
      std::pair<int, int> info = reader.getDouble();
      int dictIndex = window.getDictSize() - info.second;
      char* c = window.getFromDictPtr(dictIndex);
      for (int i = 0; i < info.first; i++) {
        std::cout << *c;
        uncompressedSize++;
        window.addNoLab(*c);
        c = window.getNext(c);
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
  std::chrono::high_resolution_clock::time_point endTime = std::chrono::high_resolution_clock::now();
  if (file != nullptr) {
    file->close();
  }

  // Info Output
  std::cerr << "Input Size: " << compressedSize << " bytes" << std::endl;
  std::cerr << "Output Size: " << uncompressedSize << " bytes" << std::endl;

  float spaceSavings = (1 - (float)compressedSize / uncompressedSize) * 100;
  std::cerr << "Space Savings: " << std::fixed << std::setprecision(2) << spaceSavings << "%" << std::endl;
  
  auto runTime = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();
  long totalTimeS = runTime / 1000.0;
  std::cerr << "Total Time: ";
  if (totalTimeS > 0) {
    std::cerr << totalTimeS << "s ";
  }
  std::cerr << std::setprecision(0) << runTime - totalTimeS * 1000.0 << "ms" << std::endl;

  auto avgTime = std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - startTime).count() / uncompressedSize;
  auto micro = avgTime / 1000.0;
  std::cerr << "Average time per byte: ";
  if (micro >= 1) {
    std::cerr << std::fixed << std::setprecision(2) << micro << " microseconds" << std::endl;
  } else {
    std::cerr << avgTime << " nanoseconds" << std::endl;
  }
  return 0;
}