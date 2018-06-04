#pragma once
#include <fstream>
#include <string>

class File {
  private:
    std::string path;
    bool isOpen;

  public:
    File(std::string path);
    void open();
    void close();
    char readChar();
    bool isFileOpen() const;
    std::fstream stream;
    std::string getPath() const;
    bool hasNextChar();
};