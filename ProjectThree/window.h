#pragma once
#include <string>

/*
** The Window class implements a circular character buffer of a fixed.
** capacity. The Window is used for the sliding window in the Lempel-Ziv
** sliding window algorithm.
*/

class Window {
  public:
    // Used for LZ
    Window(int windowCap, int labCap, std::string initChars);
    // Used for EXPAND; look-ahead buffer is kept at size 0
    Window(int windowCap);
    ~Window();
   // Getters for the sizes and capacities of the dictionary and
   // look-ahead buffer
    int getDictSize() const;
    int getLabSize() const;
    int getDictCap() const;
    // Add a character to the end of the look-ahead buffer, and
    // shift one character from the look-ahead buffer into the
    // dictionary. If the dictionary is full, remove the oldest
    // character in the dictionary.
    void add(char c);
    // Add a character to the end of the window. If the window is
    // full, remove the oldest character in the window.
    void addNoLab(char c);
    // Shift the window by one character without adding anything.
    void shift(int numSpots);
    // Given a character pointer, determine if the character is at
    // the end of the dictionary
    bool atEndOfDict(char*& c) const;
    // Get the offset of the given character from the start of the
    // look-ahead buffer
    int getOffset(char*& c) const;

    char* getFromDict(int index);
    char* getFromLab(int index);
    // Get the next character in the window
    char*& getNext(char*& c);

  private:
    char* array;
    int head;
    int tail;
    int split;
    int windowCap;
    int dictCap;
    int windowSize;
    int dictSize;
    int labSize;
};