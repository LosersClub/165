#pragma once

#include <string>

// Uses a circular array
// aka, One ring to rule them all
class Window {
  public:
    Window(int windowCap, int labCap, std::string initChars);
    Window(int windowCap);
    ~Window();
   
    int getDictSize() const;
    int getLabSize() const;
    int getDictCap() const;

    void add(char c);
    void addNoLab(char c);
    void shift(int numSpots);

    bool atEndOfDict(char*& c) const;
    int getOffset(char*& c) const;

    char* getFromDict(int index);
    char* getFromLab(int index);
    char* getNext(char* c);

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