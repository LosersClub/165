#include "file.h"
#include "window.h"
#include "bit_stream.h"
#include "suffix_array.h"

// DELETE THIS BEFORE SUBMISSION
#ifdef _WIN32
#include <io.h>
#include <fcntl.h>
#endif

#include <iostream>
#include <string>

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
        std::cerr << "Unrecognized argument: " << arg.substr(0, 3) << std::endl;
        return false;
      }
    }
  }
  return true;
}

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
#ifdef _WIN32
  _setmode(_fileno(stdout), _O_BINARY);
#endif

  // default values
  int N = 11;
  int L = 4;
  int S = 3;
  std::string path = "";

  if (!parseArguments(argc, argv, &N, &L, &S, &path)) {
    return 1;
  }
  if (path == "") {
    std::cerr << "No file path set." << std::endl;
    return 1;
  }

  int windowSize = 2 << (N - 1);
  int labSize = 2 << (L - 1);
  int maxTripleLength = (2 << (S - 1)) - 1;

  File* file = nullptr;
  try {
    file = new File(path);
  }
  catch (const std::exception& e) {
    std::cerr << e.what() << std::endl;
    return 1;
  }

  std::string initString;
  initString.reserve(labSize + 1);

  while (initString.length() < labSize + 1 && file->hasNextChar()) {
    initString += file->readChar();
  }

  BitStreamWriter writer = BitStreamWriter(N, L, S);
  std::cout << writer.writeHeader();

  Window window = Window(windowSize, labSize, initString.c_str(), initString.length());
  SuffixArray sa(&window);

  std::vector<char> triple = { window.getFromDict(0) };
  triple.reserve(maxTripleLength);

  bool lastTriple = true;
  while (window.getLabSize() > 0) {
    if (lastTriple && triple.size() == maxTripleLength) {
      writer.writeTriple(triple.size(), triple);
      //printTriple(triple);
      lastTriple = false;
    }

    std::pair<int, int> result = sa.getMatchBS();
    if (result.first < 2) {
      if (!lastTriple) {
        triple.clear();
        lastTriple = true;
      }
      triple.push_back(window.getFromLab(0));
      if (file->hasNextChar()) {
        window.add(file->readChar());
      }
      else {
        window.shift(1);
      }
    }
    else {
      if (lastTriple) {
        writer.writeTriple(triple.size(), triple);
        //printTriple(triple);
        lastTriple = false;
      }
      writer.writeDouble(result.first, result.second);
      //printDouble(result);
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
    writer.writeTriple(triple.size(), triple);
    //printTriple(triple);
  }
  writer.writeEOF();
  file->close();
  //system("PAUSE");
  return 0;
}