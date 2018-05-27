// Uses a circular array
// aka, One ring to rule them all

class Window {
  private:
    char* array;
    int head;
    int tail;
    int split;
    int windowCap;
    int dictCap;
    int labCap;
    int windowSize;
    int dictSize;
    int labSize;
  public:
    Window(int windowCap, int labCap, const char* initChars, int initCharsSize);
    int getSize() const;
    int getDictSize() const;
    int getLabSize() const;
    void add(char c);
    void add(const char* c, int size);
    void shift(int numSpots);
    int getSplitIndex() const;
    int getHead() const;
    int getTail() const;
    const char& getFromDict(int index) const;
    const char& getFromLab(int index) const;
    void print();
    ~Window();
};