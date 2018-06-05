#pragma once
#include "window.h"
#include <list>

#define MIN_MATCH_LENGTH 2
#define HASH_SIZE 1024
class HashEncoder {
  private:
    Window* window;
    bool maxMatch;
    std::list<char*> map[HASH_SIZE];
    int hash(char* stringStart);
    
    void addString(char* string, int size);
    void removeString(char* string, int size);
    void modifyTable(char* string, int size);

  public:
    HashEncoder(Window* window);
    std::pair<int, int> getMatch();
    void repair();
};