
#include "window.h"
#include "suffix_array.h"
#include <iostream>
#include <string>
#include <tuple>

// TODO: The filename should be a command-line arg, as well as the parameters
int main() {
  std::string str = "yabbadabbadoodlydoomister";
  int strIndex = 6;
  Window window = Window(15, 5, str.substr(0, 6).c_str(), 6);
  SuffixArray sa(&window);

  std::pair<int, std::vector<char>> triple = { 1, {window.getFromDict(0)} };
  bool lastTriple = true;
  
  std::cout << "String: " << str << std::endl;

  while (window.getLabSize() > 0) {
    std::pair<int, int> result = sa.getMatchBinarySearch();
    if (result.first < 2) {
      if (lastTriple) {
        triple.first += 1;
        triple.second.push_back(window.getFromLab(0));
      }
      else {
        triple = { 1, {window.getFromLab(0)} };
        lastTriple = true;
      }
      if (strIndex < str.length()) {
        window.add(str.at(strIndex++));
      } else {
        window.shift(1);
      }
    } else {
      if (lastTriple) {
        std::cout << "(0," << triple.first << ",";
        for (char& c : triple.second) {
          std::cout << c;
        }
        std::cout << ")" << std::endl;
        lastTriple = false;
      }
      std::cout << "(" << result.first << "," << result.second << ")" << std::endl;
      if (strIndex < str.length()) {
        int end = strIndex + result.first > str.length() ? str.length() - strIndex : result.first;
        for (int i = 0; i < end; i++) {
          window.add(str.at(strIndex + i));
        }
        if (strIndex + result.first > str.length()) {
          window.shift((strIndex + result.first) - str.length());
        }
        strIndex += result.first;
      } else {
        window.shift(result.first);
      }
    }
    sa.rebuild();
    //sa.print();
  }
  system("PAUSE");
}