
#include "window.h"
#include "file.h"
#include "suffix_array.h"
#include "bit_stream.h"

//#include <io.h>
//#include <fcntl.h>
#include <iostream>
#include <string>
#include <tuple>
#include <cmath>
#include <bitset>

bool parseArguments(int argc, char**argv, int* N, int* L, int* S, std::string* path) {
  if (argc > 5) {
    std::cerr << "Too many arguments." << std::endl;
    return false;
  }
  if (argc < 2) {
    std::cerr << "One required argument: path to file to be compressed." << std::endl;
    return false;
  }
  for (int i = 1; i < argc; i++) {
    if (argv[i][0] != '-') {
      if (*path != "") {
        std::cerr << "Multiple candidates for file to be compressed." << std::endl;
        return false;
      }
      *path = std::string(argv[i]);
    }
    else {
      std::string arg = std::string(argv[i]);
      if (arg.length() < 4) {
        std::cerr << "Invalid argument: " << arg << "\n"
          "Must be in the form of -N=# for args N, L, S." << std::endl;
        return false;
      }
      int number = std::stoi(arg.substr(3));
      if (argv[i][1] == 'N') {
        if (number < 9 || number > 14) {
          std::cerr << "N argument out of range." << std::endl;
          return false;
        }
        *N = number;
      }
      else if (argv[i][1] == 'L') {
        if (number < 3 || number > 4) {
          std::cerr << "L argument out of range." << std::endl;
          return false;
        }
        *L = number;
      }
      else if (argv[i][1] == 'S') {
        if (number < 1 || number > 5) {
          std::cerr << "S argument out of range." << std::endl;
          return false;
        }
        *S = number;
      }
      else {
        std::cerr << "Unrecognized argument: " << arg.substr(0,3) << std::endl;
        return false;
      }
    }
  }

}

// TODO: The filename should be a command-line arg, as well as the parameters
int main(int argc, char** argv) {
 // _setmode(_fileno(stdout), _O_BINARY);
  // default values
  int N = 11;
  int L = 4;
  int S = 3;
  std::string path = "";
  // update args
  if (!parseArguments(argc, argv, &N, &L, &S, &path)) {
    return 0;
  }
  if (path == "") {
    std::cerr << "No file path set." << std::endl;
    return 0;
  }

  int windowSize = pow(2, N);
  int labSize = pow(2, L);
  int maxTripleLength = pow(2, S) - 1;

  File* file = nullptr;
  try {
    file = new File(path);
  }
  catch (const std::exception& e) {
    std::cerr << e.what() << std::endl;
    return 0;
  }
  std::string init_string;
  int index = 0;

  while (index < labSize + 1 && file->hasNextChar()) {
    init_string += file->readChar();
    index++;
  }

  BitStreamWriter writer = BitStreamWriter(N, L, S);
  std::cout << writer.writeHeader();

  Window window = Window(windowSize, labSize, init_string.c_str(), index);
  SuffixArray sa(&window);

  std::pair<int, std::vector<char>> triple = { 1, {window.getFromDict(0)} };
  bool lastTriple = true;
  while (window.getLabSize() > 0) {
    if (lastTriple && triple.first == maxTripleLength) {
      writer.writeTriple(triple.first, triple.second);
      lastTriple = false;
    }

    std::pair<int, int> result = sa.getMatchBS();
    if (result.first < 2) {
      if (lastTriple) {
        triple.first += 1;
        triple.second.push_back(window.getFromLab(0));
      } else {
        triple = { 1, {window.getFromLab(0)} };
        lastTriple = true;
      }
      if (file->hasNextChar()) {
        index++;
        unsigned char c = file->readChar();
        window.add(c);
      }
      else {
        window.shift(1);
      }
    } else {
      if (lastTriple) {
        writer.writeTriple(triple.first, triple.second);
        lastTriple = false;
      }
      writer.writeDouble(result.first, result.second);
      int len = result.first;
      while (len > 0 && file->hasNextChar()) {
        unsigned char c = file->readChar();
        window.add(c);
        len--;
      }
      window.shift(len);
    }
    sa.rebuild();
  }
  if (lastTriple) {
    writer.writeTriple(triple.first, triple.second);
  }
  writer.writeEOF();
  file->close();
  return 1;
}