#pragma once
#include "window.h"
#include <list>

#define MIN_MATCH_LENGTH 2
#define HASH_SIZE 1024

/*
** The HashEncoder implements a hash table that is used to 
** quickly find a match between the look-ahead buffer and a
** string in the dictionary.
**
** Buckets contain pointers to characters in the dictionary.
** The hash is computed by observing the first two characters
** of a given string. This was chosen because 2 is the minimum
** match length.
*/

class HashEncoder {
  private:
    Window* window;
    // Whether the last search produced a maximum match. Needed
    // because we will need to repair one of the table entries.
    bool maxMatch;
    std::list<char*> map[HASH_SIZE];
    int hash(char* stringStart);
    // Methods used to modify the hash table
    void addString(char* string, int size);
    void removeString(char* string, int size);
    void modifyTable(char* string, int size);

  public:
    HashEncoder(Window* window);
    // Gets the best match for the contents of the look-ahead buffer
    std::pair<int, int> getMatch();
    // Repairs the last table entry if the last match was a max
    // match
    void repair();
};