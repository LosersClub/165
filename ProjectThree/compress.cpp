
#include "suffix_array.h"
#include "window.h"
#include <iostream>
#include <string>
#include <tuple>

// TODO: The filename should be a command-line arg, as well as the parameters
int main() {
  std::string str = "abcdefghijkl";
  Window window = Window(8, 4, str.substr(0, 5).c_str(), 5);
  window.print();
  std::cout << std::endl;
  std::cout << "\nDict: " << window.getDictSize() << std::endl;
  std::cout << "LAB: " << window.getLabSize() << std::endl;
  for (int i = 5; i < str.length(); i++) {
    window.add(str.at(i));
    window.print();
    std::cout << "\nDict: " << window.getDictSize() << std::endl;
    std::cout << "LAB: " << window.getLabSize() << std::endl;
    std::cout << std::endl;
  }
  std::cout << "\n\n";
  for (int i = 0; i < 4; i++) {
    window.shift(1);
    window.print();
    std::cout << "\nDict: " << window.getDictSize() << std::endl;
    std::cout << "LAB: " << window.getLabSize() << std::endl;
    std::cout << ::std::endl;
  }
  //std::tuple<int, int, int> triple;

 /* SuffixArray sa("testtest");
  sa.print();
  std::pair<int, int> result = sa.getMatch("tttt");

  std::cout << "Match - len: " << result.first << ", "
    << " offset: " << result.second << std::endl;*/
  system("PAUSE");
}