#pragma once
#include "Window.h"
#include <list>
#define MIN_MATCH_LENGTH 2
#define HASH_SIZE 1024
class HashEncoder {
  private:
    Window* window;
    bool maxMatch;
    std::list<char*> map[HASH_SIZE];
    //std::unordered_map<int, std::forward_list<char*>> map;
    void addString(char* string, int size);
    void removeString(char* string, int size);
    int hash(char* stringStart);
  public:
    HashEncoder(Window* window);
    std::pair<int, int> getMatch();
    void modifyTable(char* string, int len);
    void repair();
};