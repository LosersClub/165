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
    int size();


    std::fstream stream;
    bool hasNextChar();
};