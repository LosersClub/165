#pragma once
#include <fstream>
#include <string>

class File {
  private:
    std::fstream stream;
    std::string path;
    bool isOpen;

  public:
    File(std::string path);
    void open();
    void close();
    char readChar();
    bool isFileOpen() const;
    std::string getPath() const;
};