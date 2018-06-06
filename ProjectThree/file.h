#pragma once
#include <fstream>
#include <string>

/*
** File is a utility class that makes reading from a file easier.
*/

class File {
  private:
    std::string path;
    bool isOpen;

  public:
    File(std::string path);
    void open();
    void close();
    // Reads one character from the file
    char readChar();
    bool isFileOpen() const;
    // Returns the size of the file
    int size();

    std::fstream stream;
    bool hasNextChar();
};