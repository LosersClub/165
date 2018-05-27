
#include "window.h"
#include "suffix_array.h"
#include <iostream>
#include <string>
#include <tuple>

// TODO: The filename should be a command-line arg, as well as the parameters
int main() {
  std::string str = "yabbadabbadoo";
  int strIndex = 5;
  Window window = Window(12, 4, str.substr(0, 5).c_str(), 5);
  SuffixArray sa(&window);

  std::cout << "(0,1," << window.getFromDict(0) << ")" << std::endl; // First
  while (strIndex < str.length()) {
    std::pair<int, int> result = sa.getMatch();
    if (result.first < 2) {
      std::cout << "(0,1," << window.getFromLab(0) << ")" << std::endl;
      window.add(str.at(strIndex++));
    } else {
      window.print();
      std::cout << std::endl;
      std::cout << "(" << result.first << "," << result.second << ")" << std::endl;
      int end = strIndex + result.first > str.length() ? str.length() - strIndex : result.first;
      for (int i = 0; i < end; i++) {
        window.add(str.at(strIndex + i));
      }
      if (strIndex + result.first > str.length()) {
        window.shift(str.length() - (strIndex + result.first));
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

  //std::cout << "\nDict: " << window.getDictSize() << "  LAB: " << window.getLabSize() << std::endl;
  //for (int i = 5; i < str.length(); i++) {
  //  window.add(str.at(i));
  //  window.print();
  //  std::cout << "\nDict: " << window.getDictSize() << std::endl;
  //  std::cout << "LAB: " << window.getLabSize() << std::endl;
  //  std::cout << std::endl;
  //}



  //std::string str = "abcdefghijkl";
  //Window window = Window(8, 4, str.substr(0, 5).c_str(), 5);
  //window.print();
  //std::cout << std::endl;
  //std::cout << "\nDict: " << window.getDictSize() << std::endl;
  //std::cout << "LAB: " << window.getLabSize() << std::endl;
  //for (int i = 5; i < str.length(); i++) {
  //  window.add(str.at(i));
  //  window.print();
  //  std::cout << "\nDict: " << window.getDictSize() << std::endl;
  //  std::cout << "LAB: " << window.getLabSize() << std::endl;
  //  std::cout << std::endl;
  //}
  //std::cout << "\n\n";
  //for (int i = 0; i < 4; i++) {
  //  window.shift(1);
  //  window.print();
  //  std::cout << "\nDict: " << window.getDictSize() << std::endl;
  //  std::cout << "LAB: " << window.getLabSize() << std::endl;
  //  std::cout << ::std::endl;
  //}
  //std::tuple<int, int, int> triple;

  //SuffixArray sa(&window);
  //sa.print();

 /* SuffixArray sa("testtest");
  sa.print();
  std::pair<int, int> result = sa.getMatch("tttt");

  std::cout << "Match - len: " << result.first << ", "
    << " offset: " << result.second << std::endl;*/
  system("PAUSE");
}