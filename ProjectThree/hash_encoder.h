#include <unordered_map>
#include "Window.h"
#include <forward_list>
#define MIN_MATCH_LENGTH = 2;
#define HASH_SIZE = 2014;
class HashEncoder {
  private:
    Window* window;
    std::unordered_map<int, std::forward_list<char*>> map;
    void addString(char* string);
    void removeString(char* string);
    int hash(char* stringStart);
  public:
    HashEncoder(Window* window);
    std::pair<int, int> getMatch();
};
//
//struct StringEntry {
//  char* string;
//  int dictIndex;
//
//  bool operator==(const StringEntry& lhs, const StringEntry& rhs) {
//    return lhs.string == rhs.string && lhs.dictIndex == rhs.dictIndex;
//  }
//} StringEntry;