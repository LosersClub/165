
#include "window.h"
#include "suffix_array.h"
#include <iostream>
#include <string>
#include <tuple>

// TODO: The filename should be a command-line arg, as well as the parameters
int main() {
  std::string str = "ABBABABBABBABAAAABABABA";
  //std::string str = "ABA";
  //Window window = Window(5, 2, str.c_str(), 3);
  //window.shift(2);
  //window.print();
  //std::cout << std::endl;
  int strIndex = 6;
  Window window = Window(8, 5, str.substr(0, 6).c_str(), 6);
  SuffixArray sa(&window);

  
  std::cout << "String: " << str << std::endl;
  std::cout << "(0,1," << window.getFromDict(0) << ")" << std::endl; // First
  while (strIndex < str.length()) {
    std::pair<int, int> result = sa.getMatch();
    if (result.first < 2) {
      std::cout << "(0,1," << window.getFromLab(0) << ")" << std::endl;
      window.add(str.at(strIndex++));
    } else {
      std::cout << "(" << result.first << "," << result.second << ")" << std::endl;
      int end = strIndex + result.first > str.length() ? str.length() - strIndex : result.first;
      for (int i = 0; i < end; i++) {
        window.add(str.at(strIndex + i));
      }
      if (strIndex + result.first > str.length()) {
        window.shift((strIndex + result.first) - str.length());
      }
      strIndex += result.first;
    }
    sa.rebuild();
  }

  while (window.getLabSize() > 0) {
    std::pair<int, int> result = sa.getMatch();
    if (result.first < 2) {
      std::cout << "(0,1," << window.getFromLab(0) << ")" << std::endl;
      window.shift(1);
    }
    else {
      std::cout << "(" << result.first << "," << result.second << ")" << std::endl;
      window.shift(result.first);
    }
    sa.rebuild();
  }
  system("PAUSE");
}